namespace calc

open System

type Action = ADigit | AOp | ACalc
type CalcRes = InvOp | DivByZero | Ok of float

type Calculator () =
  class
    let eps = 1e-12

    let mutable buf = 0.0
    let mutable oper = '+'
    let mutable la = ADigit
    let mutable mem = 0.0

    let doOperation num op =
      match op with
        | '+' ->
          buf <- buf + num
          true
        | '-' ->
          buf <- buf - num
          true
        | '*' -> 
          buf <- buf * num
          true
        | '/' ->
          match compare num eps with
          | x when x < 0 -> false
          | _ ->
            buf <- buf / num
            true
        | '^' -> 
          buf <- buf ** num
          true
        | _ -> false

    member this.Reset () =
      buf <- 0.0
      oper <- '+'
      la <- ADigit
      mem <- 0.0

    member this.MemoryRead () =
      mem;

    member this.MemoryClean() =
      mem <- 0.0;

    member this.MemoryAdd num =
      mem <- mem + num

    member this.DigitPressed () =
      la <- ADigit

    member this.Operation num newOp =
      match la with
      | ADigit ->
        match doOperation num oper with
        | true ->
          oper <- newOp
          la <- AOp
          true
        | false -> false
      | _ -> false

    member this.Calculate num =
      match la with
      | ADigit ->
        match doOperation num oper with
        | true ->
          let r = buf
          buf <- 0.0
          oper <- '+'
          la <- ACalc
          Ok(r)
        | false -> DivByZero
      | AOp -> InvOp
      | ACalc -> Ok(num)
  end
// Данные функции использовались для сбора статистики

module Program

open System
open Maximum
open Integral

let testMax () =
  printfn "Testing Maximum:\n"
  printfn "10^4 elems:"
  tryMax (seq { 1 .. 10000 } |> Seq.toArray)
  printfn "\n10^7 elems:"
  tryMax (seq { 1 .. 10000000 } |> Seq.toArray)
  printfn "\n10^7 elems backwards:"
  tryMax (seq { 10000000 .. -1 .. 1 } |> Seq.toArray)
  printf "\n"

let testIntegral () =
  printfn "Testing Definite Integral\n"
  printfn "Int y=x (0 .. 10), eps=1e-5\t\tAns = 50.0"
  tryIntegral (fun x -> x) 0.0 10.0 1e-5
  printfn "\nInt y=cos(x)+2 (0 .. 10), eps=1e-5\t Ans = 19.456"
  tryIntegral (fun x -> Math.Cos(x) + 2.0) 0.0 10.0 1e-5
  printfn "\nInt y=sin(x)+2 (0 .. 56), eps=1e-5\t Ans = 112.1468"
  tryIntegral (fun x -> Math.Sin(x) + 2.0) 0.0 56.0 1e-5
  printf "\n"

[<EntryPoint>]
let main argv = 
  testMax()
  testIntegral()
  0

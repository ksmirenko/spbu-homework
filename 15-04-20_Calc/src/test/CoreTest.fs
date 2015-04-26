module test

open NUnit.Framework
open calc

[<TestFixture>]
type CalcTest() =
  let c = new Calculator ()

  [<TestCase ('+', Result = 6.0)>]
  [<TestCase ('-', Result = -2.0)>]
  [<TestCase ('*', Result = 8.0)>]
  [<TestCase ('/', Result = 0.5)>]
  [<TestCase ('^', Result = 16.0)>]
  member this.testBasicOperations op =
    c.Reset()
    c.DigitPressed ()
    c.Operation 2.0 op |> ignore
    c.DigitPressed ()
    match c.Calculate 4.0 with
    | Ok(r) -> r
    | _ -> 0.0

  [<Test>]
  member this.testMultipleOperations () =
    c.Reset()
    c.DigitPressed ()
    c.Operation 6.5 '+' |> ignore
    c.DigitPressed ()
    c.Operation -3.0 '*' |> ignore
    c.DigitPressed ()
    c.Operation 5.0 '-' |> ignore
    c.DigitPressed ()
    Assert.AreEqual(c.Calculate 8.0, Ok(9.5))

  [<Test>]
  member this.testDivByZero () =
    c.Reset()
    c.DigitPressed ()
    c.Operation 1.0 '/' |> ignore
    c.DigitPressed ()
    Assert.AreEqual(c.Calculate 0.0, DivByZero)

  [<Test>]
  member this.testMemory () =
    c.Reset()
    c.Reset ()
    c.MemoryAdd 14.89
    Assert.AreEqual(c.MemoryRead(), 14.89)
    c.MemoryAdd 0.11
    Assert.AreEqual(c.MemoryRead(), 15.0)
    c.MemoryClean ()
    Assert.AreEqual(c.MemoryRead(), 0.0)
    c.MemoryAdd -42.0
    Assert.AreEqual(c.MemoryRead(), -42.0)

module InterpretTest

open System
open NUnit.Framework
open IOProviders
open Interpreter
open ParseStmtTest

let parseContext (str : string) =
  match str with
  | "" -> []
  | _ -> str.Split(' ', '\n') |> Array.toList |> (List.map Int32.Parse)

// write
[<TestCase("write\n42", "", Result = "42\n")>]
// read, seq, multiple read to one var
[<TestCase(";\nread\nx\n;\nwrite\n+\nx\n1\n;\nread\nx\nwrite\n+\nx\n1",
  "-5 97", Result = "-4\n98\n")>]
// assign, assign expr
[<TestCase(";\n:=\nx\n5\n;\n:=\ny\n-\n1\nx\nwrite\ny", "", Result = "-4\n")>]
// if
[<TestCase(";\nread\nx\nif\nx\nwrite\n+\nx\n1\nwrite\nx", "3", Result = "4\n")>]
[<TestCase(";\nread\nx\nif\nx\nwrite\n+\nx\n1\nwrite\nx", "0", Result = "0\n")>]
// assign, while
[<TestCase(";\n:=\nx\n3\nwhile\nx\n;\nwrite\nx\n:=\nx\n-\nx\n1",
  "", Result = "3\n2\n1\n")>]
// t.in (pow)
[<TestCase(
  "; read x ; read n ; := res 1 ; while n ; := res * res x := n - n 1 write res",
  "5 0", Result = "1\n")>]
[<TestCase(
  "; read x ; read n ; := res 1 ; while n ; := res * res x := n - n 1 write res",
  "3 4", Result = "81\n")>]
// factorial
[<TestCase(
  "; read n ; := res 1 ; while n ; := res * res n := n - n 1 write res",
  "5", Result = "120\n")>]
let test code input =
  let op = new StringOutputProvider()
  ignore (interpret code (new ListInputProvider(input |> parseContext)) op)
  op.GetString()

// 
let testErr code input =
  Assert.AreNotEqual(ErrNo,
    interpret code (new ListInputProvider(input |> parseContext))
      (new StringOutputProvider()))
module ParseStmtTest

open System
open NUnit.Framework
open Interpreter

let parseStmtStr (str : string) =
  fst (str.Split(' ', '\n') |> Array.toList |> parseStmt)

// тесты здесь не объединил в TestCase, потому что пытаться сравнить строки жутко муторно
[<Test>]
let testWrite () =
  Assert.AreEqual(parseStmtStr "write 42", Write(Num 42))

[<Test>]
let testRead () =
  Assert.AreEqual(parseStmtStr "read a", Read("a"))

[<Test>]
let testAssign () =
  Assert.AreEqual(parseStmtStr ":= a -282", Assign("a", Num -282))

[<Test>]
let testSeq () =
  Assert.AreEqual(parseStmtStr "; read a write + a 1",
    Seq(Read("a"), Write(BinOp('+', Var "a", Num 1))))

[<Test>]
let testIf () =
  Assert.AreEqual(parseStmtStr "if % xx 2 write 1 write 0",
    If(BinOp('%', Var "xx", Num 2), Write(Num 1), Write(Num 0)))

[<Test>]
let testWhile () =
  Assert.AreEqual(parseStmtStr "while v ; write v := v - v 1",
    While(Var "v", Seq(Write(Var "v"), Assign("v", BinOp('-', Var "v", Num 1)))))
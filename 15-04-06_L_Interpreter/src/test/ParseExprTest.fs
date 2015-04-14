module ParseExprTest

open System
open NUnit.Framework
open Interpreter

let parseExprStr (str : string) =
  fst (str.Split(' ', '\n') |> Array.toList |> parseExpr)

// тесты здесь не объединил в TestCase, потому что пытаться сравнить строки жутко муторно
[<Test>]
let test1 () =
  Assert.AreEqual(parseExprStr "-42", Num -42)

[<Test>]
let test2 () =
  Assert.AreEqual(parseExprStr "+ 1 2 ",
    BinOp('+',Num 1,Num 2))

[<Test>]
let test3 () =
  Assert.AreEqual(parseExprStr "+ * -148 var1 % - Pet00h ^ var1 0 / 14 87",
    BinOp('+', BinOp('*', Num -148, Var "var1"),
      BinOp ('%', BinOp('-', Var "Pet00h", BinOp ('^', Var "var1", Num 0)),
        BinOp('/', Num 14, Num 87))))
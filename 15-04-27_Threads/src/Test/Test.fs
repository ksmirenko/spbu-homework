module Test

open System
open NUnit.Framework
open Maximum
open Integral

[<Test>]
let ``Max of a few elems`` () =
  let arr = [| -5; 6; 0; 8; -10; 7; 5 |]
  for n in [1; 2; 4; 8] do
    Assert.AreEqual(maxMT n arr, 8)

[<Test>]
let ``Max of 10^4 elems`` () =
  let arr = seq { 1 .. 10000 } |> Seq.toArray
  for n in [1; 2; 4; 8] do
    Assert.AreEqual(maxMT n arr, 10000)

[<Test>]
let ``Max of 10^7 elems`` () =
  let arr = seq { 1 .. 10000000 } |> Seq.toArray
  for n in [1; 2; 4; 8] do
    Assert.AreEqual(maxMT n arr, 10000000)

[<Test>]
let ``Int y=x (0 .. 10)`` () =
  let f x = x
  for n in [1; 2; 4; 8] do
    Assert.Less(Math.Abs((defIntegralMT n f 0.0 10.0 1e-5) - 50.0), 1e-4)

[<Test>]
let ``Int y=cos(x)+2 (0 .. 10)`` () =
  let f x = Math.Cos(x) + 2.0
  for n in [1; 2; 4; 8] do
    Assert.Less(Math.Abs((defIntegralMT n f 0.0 10.0 1e-5) - 19.456), 1e-4)

[<Test>]
let ``Int y=sin(x)+2 (0 .. 56)`` () =
  let f x = Math.Sin(x) + 2.0
  for n in [1; 2; 4; 8] do
    Assert.Less(Math.Abs((defIntegralMT n f 0.0 56.0 1e-5) - 112.1468), 1e-4)
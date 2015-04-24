module Test41

open NUnit.Framework
open Task41

// Тесты для задания 41

[<Test>]
let beMan () =
  Assert.AreEqual(
    N("son", N("house", E, E), N("tree", E, E)),
    (makeTree "son" "tree" "house")
  )

[<Test>]
let testMap () =
  Assert.AreEqual(
    N(7, N(5, N(4, E, E), N(6, E, E)), N(9, E, N(10, E, E))),
    (treeMap ((+) 2) (N(5, N(3, N(2, E, E), N(4, E, E)), N(7, E, N(8, E, E)))))
  )

[<Test>]
let testFilter () =
  Assert.AreEqual(
    N(2, E, N(4, E, N(10, E, E))),
    (treeFilter (fun x -> x % 2 = 0)
      (N(7, N(2, N(1, E, E), N(4, E, E)), N(9, E, N(10, E, E)))))
  )

[<Test>]
let testToBst () =
  Assert.AreEqual(
    N("mess",
      N("I", E, N("am", E, N("clean", E, E))),
        N("which", N("supposed", E, N("to", E, E)), E)),
    (treeToBST
      (N("mess",
        N("which", N("I", E, E),
          N("am", N("supposed", E, E), N("to", E, E))), N("clean", E, E))) )
  )

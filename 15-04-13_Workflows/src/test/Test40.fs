module Test40

open NUnit.Framework
open Task40

// Тесты для задания 40

// корректность return
[<Test>]
let test40_1() =
  Assert.AreEqual(Some 2,
    ring 5 {
      return! 7
    })

// из условия - корректность нескольких let!
[<Test>]
let test40_2 () =
  Assert.AreEqual(Some 0,
    ring 5 {
      let! a = (( * ) 2 3)
      let! b = 4
      return a + b
    })

// деление на ноль
[<Test>]
let test40_3 () =
  Assert.AreEqual(None,
    ring 5 {
      let! a = (( / ) 2 0)
      let! b = 5
      return a + b
    })

// -------------------------------------------------------------------------
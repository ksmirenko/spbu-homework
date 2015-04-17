// Задача 40 от 13 апреля
// Workflow, выполняющий математические вычисления в кольце вычетов по модулю n
// Автор: Кирилл Смиренко, группа 171
// Расчётное время выполнения: 2 часа
// Действительное время выполнения: 2 часа

module Task40

open NUnit.Framework

let ( + ) a b = Some (a + b)
let ( - ) a b = Some (a - b)
let ( * ) a b = Some (a * b)
let ( / ) a b =
  match b with
  | 0 -> None
  | _ -> Some (a / b)

type RingBuilder(n : int) =
  member this.Return m =
    match m with
    | None -> None
    | Some x -> Some (x % n)
  member this.ReturnFrom x = Some (x % n)
  member this.Bind (x : int) = Some x
  member this.Bind (x : int, f) = f x
  member this.Bind (m : Option<int>, f) =
    match m with
    | None -> None
    | Some x -> f (x % n)
let ring n = RingBuilder n

// -------------------------------------------------------------------------
// Тесты

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
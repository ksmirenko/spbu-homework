// Задача 41 от 13 апреля
// Workflow на бинарных деревьях
// Автор: Кирилл Смиренко, группа 171
// Расчётное время выполнения: 2 часа
// Действительное время выполнения: 3 часа

module Task41

open NUnit.Framework

type Tree<'T> = E | N of 'T * Tree<'T> * Tree<'T>

let rec insert t i =
  match i, t with
  | i, E -> N(i, E, E)
  | i, N(c, l, r) ->
    if i = c then N(c, l, r)
    else if i < c then N(c, insert l i, r)
    else N(c, l, insert r i)

// fold с предварительным заворачиванием значений узлов
let rec fold2 fa fe acc tree  =
  match tree with
  | E -> acc
  | N(c, l, r) -> fold2 fa fe (fold2 fa fe (fa (acc) (fe c)) l) r

let merge tTo tFrom = fold2 insert (fun x -> x) tTo tFrom

type TreeBuilder () =
  member this.Bind (m, f)     = fold2 merge f E m
  member this.Combine (a, b)  = merge a b
  member this.Delay f         = f ()
  member this.For (m, f)      = this.Bind (m, f)
  member this.Return x        = N(x, E, E)
  member this.ReturnFrom x    = x

let treeWorkflow  = TreeBuilder()

// -------------------------------------------------------------------------
// Примеры функций на данном Workflow

// строит дерево из аргументов; эта функция проверяет combine
let makeTree a b c =
  treeWorkflow {
    return a
    return b
    return c
  }

// map на дереве
let treeMap f t =
  treeWorkflow {
    for n in t do
      return f n
  }

// filter на дереве
let treeFilter cond t =
  treeWorkflow {
    for n in t do
      if cond n then return n else return! E
  }

// перестраивает дерево, превращая его в BST
let treeToBST t =
  treeWorkflow {
    for n in t do
      return n
  }

// -------------------------------------------------------------------------
// Тесты

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
let testFilter1 () =
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
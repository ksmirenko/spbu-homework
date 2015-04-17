// Задача 41 от 13 апреля
// Workflow на бинарных деревьях
// Автор: Кирилл Смиренко, группа 171
// Расчётное время выполнения: 2 часа
// Действительное время выполнения: 3 часа

module Task41

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

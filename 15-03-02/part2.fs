﻿// Задачи 14-19 от 2 марта
// Автор: Кирилл Смиренко, группа 171
// Расчётное время выполнения: 2 часа
// Действительное время выполнения: 2 часа

// задание 14 (полиморфное дерево)
type Tree<'A> =
  | Empty
  | Node of 'A * Tree<'A> * Tree<'A>

// вставка элемента в дерево
let rec insert i t =
  match i, t with
  | i, Empty -> Node(i, Empty, Empty)
  | i, Node(c, l, r) ->
    match compare i c with
    | x when x < 0 -> Node(c, insert i l, r)
    | x when x > 0 -> Node(c, l, insert i r)
    | _ -> t

// создание дерева из списка
let makeTree l =
  List.fold (fun t n -> insert n t) Empty l

// вывод дерева на экран
let rec printTreeCLRZ t =
  match t with
  | Empty ->
      printf "X "
  | Node(c, l, r) ->
      printf "%A " c
      printTreeCLRZ l
      printTreeCLRZ r

// задание 15 (полиморфный map для дерева)
let rec map f tree =
  match tree with
  | Empty -> Empty
  | Node(v, l, r) -> Node(f v, map f l, map f r)

// задание 16 (полиморфный fold для дерева)
let rec fold f acc tree  =
  match tree with
  | Empty -> acc
  | Node(v, l, r) -> fold f (fold f (f acc v) l) r

// сравнение произвольных значений (используя Option)
let minElem a b =
  match a with
  | None -> Some b
  | Some a -> Some (min a b)

[<EntryPoint>]
let main argv =
  let t1 = makeTree [6; 2; 7; 4; 3; 1; 5]
  let t2 = makeTree ["bbb"; "bab"; "c"; "abb"; "aa"; "abc"; "bbbc"; "bba"; "ca"]
  printfn "Тестовые деревья (все выводятся C-L-R с пустыми листьями): "
  printTreeCLRZ t1
  printfn ""
  printTreeCLRZ t2
  printfn "\n"

  printfn "15. Замена элементов численного дерева на противоположные:"
  printTreeCLRZ (map (fun a -> -a) t1)
  printfn "\n"

  printfn "16. Суммарная длина всех символов строкового дерева: %A\n"
    (fold (fun acc elem -> acc + String.length elem) 0 t2)

  // задание 17: сумма значений в дереве (fold)
  printfn "17. Сумма значений в численном дереве: %A\n"
    (fold (fun acc elem -> acc + elem) 0 t1)

  // задание 18: поиск минимума в дереве (fold)
  printfn "18. Минимум в строковом дереве: %A\n"
    (fold minElem None t2)

  // задание 18: копирование дерева (fold)
  printfn "19. Копирование численного дерева:"
  printTreeCLRZ (fold (fun cln node -> insert node cln) Empty t1)
  printfn "\n"

  0
// Задачи 5-8 от 16 февраля (про двоичное дерево)
// Автор: Кирилл Смиренко, группа 171

// Описание:
//    Инструкции для программы передаются в параметрах запуска в формате:
//      "i <число>" - вставить число
//      "r <число>" - удалить число, если оно есть
//      "p <формат> [Z]" -
//          вывести текущее дерево в строку на экран в указанном формате (LCR/CLR/LRC)
//          можно добавить через пробел букву Z, тогда пустые узлы будут напечатаны в виде 'X'
//    Пример параметров запуска:
//      i 6 i 2 i 7 i 4 i 3 i 1 i 5 p CLR Z r 2 p LRC

open System

type bsTree =
  | EmptyTree
  | TreeNode of int * bsTree * bsTree

let rec insert i t =
  match i, t with
  | i, EmptyTree -> TreeNode(i, EmptyTree, EmptyTree)
  | i, TreeNode(c, l, r) ->
    if i = c then TreeNode(c, l, r)
    else if i < c then TreeNode(c, insert i l, r)
    else TreeNode(c, l, insert i r)

let rec findLeft t =
  match t with
  | EmptyTree -> 0
  | TreeNode(c, EmptyTree, _) ->
    c
  | TreeNode(c, l, _) -> findLeft l

let rec remove i t =
  match i, t with
  | _, EmptyTree -> EmptyTree
  | i, TreeNode(c, l, r) ->
    if i < c then TreeNode(c, remove i l, r)
    else if i > c then TreeNode(c, l, remove i r)
    else
      match l, r with
      | EmptyTree, EmptyTree -> EmptyTree
      | TreeNode(c1, l1, r1), EmptyTree -> TreeNode(c1, l1, r1)
      | EmptyTree, TreeNode(c2, l2, r2) -> TreeNode(c2, l2, r2)
      | l, TreeNode(c2, EmptyTree, r2) -> TreeNode(c2, l, r2)
      | l, TreeNode(c2, l2, r2) ->
          let dl = findLeft l2
          TreeNode(dl, l, remove dl (TreeNode(c2, l2, r2)))

let rec printTree t mode z =
  match t with
  | EmptyTree ->
    match z with
    | "yes" ->
      printf "X "
      ()
    | _ -> ()
  | TreeNode(c, l, r) ->
    match mode with
    | "LCR" ->
      printTree l mode z
      printf "%d " c
      printTree r mode z
    | "LRC" ->
      printTree l mode z
      printTree r mode z
      printf "%d " c
    | "CLR" ->
      printf "%d " c
      printTree l mode z
      printTree r mode z
    | _ -> ()

let rec loop tree argv =
  match argv with
  | [] -> 0
  | "i" :: n :: rest -> 
    match Int32.TryParse(n) with
    | (true, n) -> loop (insert n tree) rest
    | (false, _) -> -1
  | "r" :: n :: rest ->
    match Int32.TryParse(n) with
    | (true, n) -> loop (remove n tree) rest
    | (false, _) -> -1
  | "p" :: m :: "Z" :: rest ->
    printTree tree m "yes"
    printf "\n"
    loop tree rest
  | "p" :: m :: rest ->
    printTree tree m "no"
    printf "\n"
    loop tree rest
  | _ -> -1

[<EntryPoint>]
let main argv = 
  loop EmptyTree (argv |> Array.toList)

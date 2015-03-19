// Задачи 1-4 от 16 февраля (про числа Пеано)
// Автор: Кирилл Смиренко, группа 171

type Peano = Zero | S of Peano

let rec plus a b =
  match a with
  | Zero -> b
  | S a -> S (plus a b)

let rec minus a b =
  match a, b with
  | Zero, _ -> Zero
  | a, Zero -> a
  | S a, S b -> minus a b
  
let rec toInt a =
  match a with
  | Zero -> 0
  | S a -> 1 + toInt a

let toString a =
  a.ToString ()

let rec mult a b =
  match a, b with
  | _, Zero -> Zero
  | Zero, _ -> Zero
  | a, S b -> plus (mult a b) a

let rec pow a b =
  match a, b with
  | a, Zero -> S Zero
  | Zero, _ -> Zero
  | a, S b -> mult (pow a b) a

[<EntryPoint>]
let main argv = 
  //printfn "%A" ( pow (S (S Zero)) (S (S (S Zero))) )
  printfn "%A" (pow Zero Zero)
  0

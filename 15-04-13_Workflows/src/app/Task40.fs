// Задача 40 от 13 апреля
// Workflow, выполняющий математические вычисления в кольце вычетов по модулю n
// Автор: Кирилл Смиренко, группа 171
// Расчётное время выполнения: 2 часа
// Действительное время выполнения: 2 часа

module Task40

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

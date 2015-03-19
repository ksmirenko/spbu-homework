// Задачи 27-29 от 16 марта
// Автор: Кирилл Смиренко, группа 171
// Расчётное время выполнения: 2 часа (с 2:00)
// Действительное время выполнения: 

// 27. Интерфейс для полиморфного списка
type ICList<'A when 'A : equality> =
  interface
    abstract AddHead     : 'A            -> ICList<'A>
    abstract AddTail     : 'A            -> ICList<'A>
    abstract AddBefore   : 'A            -> int         -> ICList<'A>
    abstract RemoveHead  : unit          -> ICList<'A>
    abstract RemoveTail  : unit          -> ICList<'A>
    abstract RemoveAt    : int           -> ICList<'A>
    abstract Find        : ('A -> bool)  -> Option<'A>
    abstract ConcatWith  : ICList<'A>    -> ICList<'A>
  end

type Node<'A> =
  | Empty
  | N of 'A * Option<Node<'A>>

// 28. Реализация интерфейса списка на АТД
type CListAdt<'A when 'A : equality>(value' : 'A, next' : Option<CListAdt<'A>>) =
  class
    let value = value'
    let next = next'

    new() = CListAdt<'A>(Unchecked.defaultof<'A>, None)

    member this.GetValue =
      value
    member this.GetNext =
      next

    interface ICList<'A> with
      member this.AddHead v =
        new CListAdt<'A>(v, Some this) :> ICList<'A>
      member this.AddTail v =
        this :> ICList<'A>
      member this.AddBefore v i =
        this :> ICList<'A>
      member this.RemoveHead () =
        this :> ICList<'A>
      member this.RemoveTail () =
        this :> ICList<'A>
      member this.RemoveAt i =
        this :> ICList<'A>
      member this.Find func =
        Some value
      member this.ConcatWith l =
        this :> ICList<'A>

    static member Tail(l) =
      let rec t (l : CListAdt<'A>) =
        match l.GetNext with
        | None -> l
        | Some l' -> t l'
      t l
    static member FoldBack(f, a, l) =
      let rec foldBack f a (l : CListAdt<'B>) =
        match l.GetValue with
        | Unchecked.defaultof<'B> -> Unchecked.defaultof<'B>
        | v -> foldBack f (f a v) l.GetNext
      f a l
  end

// 29. Реализация интерфейса списка на массиве

[<EntryPoint>]
let main argv =
  0
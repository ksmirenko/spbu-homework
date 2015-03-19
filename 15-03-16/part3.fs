// Задачи 27-29 Zот 16 марта
// Автор: Кирилл Смиренко, группа 171
// Расчётное время выполнения: 2 часа (с 2:00)
// Действительное время выполнения: 

// 27. Интерфейс для полиморфного списка
type ICList<'A when 'A : equality> =
  interface
    abstract HeadValue   : unit          -> Option<'A>
    abstract Length      : unit          -> int
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
  | N of 'A * Node<'A>

// 28. Реализация интерфейса списка на АТД
type CListAdt<'A when 'A : equality>(h : Node<'A>, s : int) =
  class
    let head = h
    let length = s

    let rec tail (l : Node<'A>) =
      match l with
      | Empty -> Empty
      | N(_, Empty) -> l
      | N(_, n) -> tail n

    new() = CListAdt<'A>(Empty, 0)
    new(v : 'A) = CListAdt<'A>(N(v, Empty), 1)

    member this.HeadNode = head

    interface ICList<'A> with
      member this.HeadValue () =
        match head with
        | Empty -> None
        | N(v, _) -> Some v
      member this.Length () =
        length
      member this.AddHead v =
        new CListAdt<'A>(N(v, head), length + 1) :> ICList<'A>
      member this.AddTail v =
        let rec at n v =
          match n with
          | Empty -> N(v, Empty)
          | N(v', n') -> N(v', at n' v)
        CListAdt(at head v, length + 1) :> ICList<'A>
      member this.AddBefore v i =
        if i > length then
          (this :> ICList<'A>).AddTail v
        else
          let rec ab node v index =
            match node with
            | Empty -> N(v, Empty)
            | N(vl, nxt) ->
              match compare index i with
              | c when c < 0 -> N(vl, ab nxt v (index + 1))
              | _ -> N(v, node)
          CListAdt(ab head v 0, length + 1) :> ICList<'A>
      member this.RemoveHead () =
        match head with
        | Empty -> this :> ICList<'A>
        | N(vl, nxt) -> CListAdt<'A>(nxt, length - 1) :> ICList<'A>
      member this.RemoveTail () =
        let rec rt n =
          match n with
          | Empty -> Empty
          | N(vl, N(_, Empty)) -> N(vl, Empty)
          | N(vl, nxt) -> N(vl, rt nxt)
        CListAdt(rt head, max (length - 1) 0) :> ICList<'A>
      member this.RemoveAt i =
        if i >= length then
          this :> ICList<'A>
        elif i = 0 then
          (this :> ICList<'A>).RemoveHead()
        elif i = length - 1 then
          (this :> ICList<'A>).RemoveTail()
        else
          let rec ra n index =
            match n with
            | N(vl, (N(_, nxt) as n')) ->
              if index = i - 1 then
                N(vl, nxt)
              else
                N(vl, ra n' (index + 1))
            | _ -> Empty
          CListAdt<'A>(ra head 0, length - 1) :> ICList<'A>
      member this.Find func =
        let rec fnd n =
          match n with
          | Empty -> None
          | N(vl, nxt) -> if func vl then Some vl else fnd nxt
        fnd head
      member this.ConcatWith l =
        let rec toNode (l : ICList<'A>) =
          match l.Length() with
          | 0 -> Empty
          | _ -> N(l.HeadValue().Value, toNode (l.RemoveHead()))
        let rec cw n n' =
          match n with
          | Empty -> n'
          | N(v, n'') -> N(v, cw n'' n')
        CListAdt((cw head (toNode l)), (length + l.Length())) :> ICList<'A>

    override this.ToString() =
      let rec itos n =
        match n with
        | Empty -> "]"
        | N(vl, Empty) -> vl.ToString() + " ]"
        | N(vl, nxt) -> vl.ToString() + "; " + (itos nxt)
      "[ " + (itos head)
  end

// 29. Реализация интерфейса списка на массиве

[<EntryPoint>]
let main argv =
  printfn "28. Реализация интерфейса списка на АТД. Демонстрация:"
  let mutable l1 = CListAdt<int>(3) :> ICList<int>;
  printfn "l1 = %A" l1
  printfn "> AddHead 1 // a"
  l1 <- l1.AddHead 1
  printfn "l1 = %A\n> AddTail 4 // b" l1
  l1 <- l1.AddTail 4
  printfn "l1 = %A\n> AddBefore 2 1 // c" l1
  l1 <- l1.AddBefore 2 1
  printfn "l1 = %A\n> AddBefore 0 0" l1
  l1 <- l1.AddBefore 0 0
  printfn "l1 = %A\n> AddBefore 5 6" l1
  l1 <- l1.AddBefore 5 6
  printfn "l1 = %A\n> RemoveHead() // d" l1
  l1 <- l1.RemoveHead()
  printfn "l1 = %A\n> RemoveTail() // e" l1
  l1 <- l1.RemoveTail()
  printfn "l1 = %A\n> RemoveAt 1 // f" l1
  l1 <- l1.RemoveAt 1
  printfn "l1 = %A\n> let mutable l2 = l1.RemoveAt 3" l1
  let mutable l2 = l1.RemoveAt 3
  printfn "l2 = %A" l2
  printfn ">l2.Find ((=) 3) // g\n%A"  (l2.Find ((=) 3))
  printfn "> l1 <- l1.ConcatWith l2 // h"
  l1 <- l1.ConcatWith l2
  printfn "l1 = %A\n" l1

  0
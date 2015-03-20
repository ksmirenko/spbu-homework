// Задачи 27-29 от 16 марта
// Автор: Кирилл Смиренко, группа 171
// Расчётное время выполнения: 2 часа
// Действительное время выполнения: 3 часа

// 27. Интерфейс для полиморфного списка
type ICList<'A when 'A : equality> =
  interface
    abstract HeadValue   : Option<'A> with get
    abstract Length      : int with get
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

    new() = CListAdt(Empty, 0)
    new(v : 'A) = CListAdt(N(v, Empty), 1)

    member this.HeadNode = head

    interface ICList<'A> with
      member this.HeadValue
        with get () =
          match head with
          | Empty -> None
          | N(v, _) -> Some v
      member this.Length with get () = length
      member this.AddHead v =
        new CListAdt<'A>(N(v, head), length + 1) :> ICList<'A>
      member this.AddTail v =
        let rec at n v =
          match n with
          | Empty -> N(v, Empty)
          | N(v', n') -> N(v', at n' v)
        CListAdt(at head v, length + 1) :> ICList<'A>
      member this.AddBefore v i =
        if (i < 0) || (i >= length) then
          failwith "Index out of range!"
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
        if (i < 0) || (i >= length) then
          failwith "Index out of range!"
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
          match l.Length with
          | 0 -> Empty
          | _ -> N(l.HeadValue.Value, toNode (l.RemoveHead()))
        let rec cw n n' =
          match n with
          | Empty -> n'
          | N(v, n'') -> N(v, cw n'' n')
        CListAdt((cw head (toNode l)), (length + l.Length)) :> ICList<'A>

    override this.ToString() =
      let rec itos n =
        match n with
        | Empty -> "]"
        | N(vl, Empty) -> vl.ToString() + " ]"
        | N(vl, nxt) -> vl.ToString() + "; " + (itos nxt)
      "[ " + (itos head)
  end

// 29. Реализация интерфейса списка на массиве
type CListArray<'A when 'A : equality>(arr : 'A []) =
  class
    let array = arr
    let length = array.Length

    let wrap v =
      Array.create 1 v

    new() = CListArray<'A>(Array.empty<'A>)
    new(v : 'A) = CListArray<'A>(Array.create 1 v)

    interface ICList<'A> with
      member this.HeadValue
        with get () =
          match array with
          | [||] -> None
          | _ -> Some array.[0]
      member this.Length with get () = length
      member this.AddHead v =
        CListArray<'A>(Array.append (wrap v) array) :> ICList<'A>
      member this.AddTail v =
        CListArray<'A>(Array.append array (wrap v)) :> ICList<'A>
      member this.AddBefore v i =
        if (i < 0) || (i >= length) then
          failwith "Index out of range!"
        else
          let a =
            Array.append
              (Array.append (Array.sub array 0 i) (wrap v))
                (Array.sub array i (length - i + 1))
          CListArray<'A>(a) :> ICList<'A>
      member this.RemoveHead () =
        match length with
        | 0 -> this :> ICList<'A>
        | _ -> CListArray<'A>(Array.sub array 1 (length - 1)) :> ICList<'A>
      member this.RemoveTail () =
        match length with
        | 0 -> this :> ICList<'A>
        | _ -> CListArray<'A>(Array.sub array 0 (length - 1)) :> ICList<'A>
      member this.RemoveAt i =
        if (i < 0) || (i >= length) then
          failwith "Index out of range!"
        else
          match length with
          | 0 -> this :> ICList<'A>
          | _ ->
            let a =
              Array.append (Array.sub array 0 (i - 1)) 
                (Array.sub array i (length - i))
            CListArray<'A>(a) :> ICList<'A>
      member this.Find func =
        Array.tryFind func array
      member this.ConcatWith l =
        let rec toArr (l : ICList<'A>) =
          match l.Length with
          | 0 -> [||]
          | _ -> Array.append (wrap l.HeadValue.Value) (toArr (l.RemoveHead()))
        CListArray<'A>(Array.append array (toArr l)) :> ICList<'A>

    override this.ToString() =
      array.ToString()
  end

[<EntryPoint>]
let main argv =
  printfn "28. Реализация интерфейса списка на АТД:"
  let mutable l1 = CListAdt<int>(3) :> ICList<int>;
  printfn "l1 = %A" l1
  printfn "// a\n> AddHead 1"
  l1 <- l1.AddHead 1
  printfn "l1 = %A\n// b\n> AddTail 4" l1
  l1 <- l1.AddTail 4
  printfn "l1 = %A\n// c\n> AddBefore 2 1" l1
  l1 <- l1.AddBefore 2 1
  printfn "l1 = %A\n> AddBefore 0 0" l1
  l1 <- l1.AddBefore 0 0
  printfn "l1 = %A\n> AddTail 5" l1
  l1 <- l1.AddTail 5
  printfn "l1 = %A\n// d\n> RemoveHead()" l1
  l1 <- l1.RemoveHead()
  printfn "l1 = %A\n// e\n> RemoveTail()" l1
  l1 <- l1.RemoveTail()
  printfn "l1 = %A\n// f\n> RemoveAt 1" l1
  l1 <- l1.RemoveAt 1
  printfn "l1 = %A\n> let mutable l1' = l1.RemoveAt 2" l1
  let mutable l1' = l1.RemoveAt 2
  printfn "l1' = %A" l1'
  printfn "// g\n> l1'.Find ((=) 3)\n%A"  (l1'.Find ((=) 3))
  printfn "// h\n> l1 <- l1.ConcatWith l1'"
  l1 <- l1.ConcatWith l1'
  printfn "l1 = %A\n" l1

  printfn "29. Реализация интерфейса списка на массиве:"
  let mutable l2 = CListAdt<int>(3) :> ICList<int>;
  printfn "l2 = %A" l2
  printfn "// a\n> AddHead 1"
  l2 <- l2.AddHead 1
  printfn "l2 = %A\n// b\n> AddTail 4" l2
  l2 <- l2.AddTail 4
  printfn "l2 = %A\n// c\n> AddBefore 2 1" l2
  l2 <- l2.AddBefore 2 1
  printfn "l2 = %A\n> AddBefore 0 0" l2
  l2 <- l2.AddBefore 0 0
  printfn "l2 = %A\n> AddTail 5" l2
  l2 <- l2.AddTail 5
  printfn "l2 = %A\n// d\n> RemoveHead()" l2
  l2 <- l2.RemoveHead()
  printfn "l2 = %A\n// e\n> RemoveTail()" l2
  l2 <- l2.RemoveTail()
  printfn "l2 = %A\n// f\n> RemoveAt 1" l2
  l2 <- l2.RemoveAt 1
  printfn "l2 = %A\n> let mutable l2' = l2.RemoveAt 2" l2
  let mutable l2' = l2.RemoveAt 2
  printfn "l2' = %A" l2'
  printfn "// g\n> l2'.Find ((=) 3)\n%A"  (l2'.Find ((=) 3))
  printfn "// h\n> l2 <- l2.ConcatWith l2'"
  l2 <- l2.ConcatWith l2'
  printfn "l2 = %A\n" l2

  0
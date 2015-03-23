// Задачи 27-29 от 16 марта
// Автор: Кирилл Смиренко, группа 171
// Расчётное время выполнения: 2 часа
// Действительное время выполнения: 3 часа

// Исправленная версия: методы теперь возвращают bool или unit

// 27. Интерфейс для полиморфного списка
type ICList<'A when 'A : equality> =
  interface
    abstract HeadValue   : Option<'A> with get
    abstract Length      : int with get
    abstract Tail        : ICList<'A> with get
    abstract AddHead     : 'A            -> unit
    abstract AddTail     : 'A            -> unit
    abstract AddBefore   : 'A            -> int         -> bool
    abstract RemoveHead  : unit          -> bool
    abstract RemoveTail  : unit          -> bool
    abstract RemoveAt    : int           -> bool
    abstract Find        : ('A -> bool)  -> Option<'A>
    abstract Append  : ICList<'A>    -> unit
  end

type Node<'A> =
  | Empty
  | N of 'A * Node<'A>

// 28. Реализация интерфейса списка на АТД
type CListAdt<'A when 'A : equality>(h : Node<'A>, s : int) =
  class
    let mutable head = h
    let mutable length = s

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
      member this.Tail
        with get () =
          match head with
          | Empty -> (new CListAdt<'A>()) :> ICList<'A>
          | N(_, nxt) -> (new CListAdt<'A>(nxt, length - 1)) :> ICList<'A>
      member this.AddHead v =
        head <- N(v, head)
        length <- length + 1
      member this.AddTail v =
        let rec at n v =
          match n with
          | Empty -> N(v, Empty)
          | N(v', n') -> N(v', at n' v)
        head <- at head v
        length <- length + 1
      member this.AddBefore v i =
        if (i < 0) || (i >= length) then
          false
        else
          let rec ab node v index =
            match node with
            | Empty -> N(v, Empty)
            | N(vl, nxt) ->
              match compare index i with
              | c when c < 0 -> N(vl, ab nxt v (index + 1))
              | _ -> N(v, node)
          head <- ab head v 0
          length <- length + 1
          true
      member this.RemoveHead () =
        match head with
        | Empty -> false
        | N(vl, nxt) ->
          head <- nxt
          length <- length - 1
          true
      member this.RemoveTail () =
        if length = 0 then
          false
        else
          let rec rt n =
            match n with
            | Empty -> Empty
            | N(vl, N(_, Empty)) -> N(vl, Empty)
            | N(vl, nxt) -> N(vl, rt nxt)
          head <- rt head
          length <- length - 1
          true
      member this.RemoveAt i =
        if (i < 0) || (i >= length) then
          false
        elif i = 0 then
          ignore ((this :> ICList<'A>).RemoveHead())
          true
        elif i = length - 1 then
          ignore ((this :> ICList<'A>).RemoveTail())
          true
        else
          let rec ra n index =
            match n with
            | N(vl, (N(_, nxt) as n')) ->
              if index = i - 1 then
                N(vl, nxt)
              else
                N(vl, ra n' (index + 1))
            | _ -> Empty
          head <- ra head 0
          length <- length - 1
          true
      member this.Find func =
        let rec fnd n =
          match n with
          | Empty -> None
          | N(vl, nxt) -> if func vl then Some vl else fnd nxt
        fnd head
      member this.Append l =
        let rec toNode (l : ICList<'A>) =
          match l.Length with
          | 0 -> Empty
          | _ -> N(l.HeadValue.Value, toNode (l.Tail))
        let rec cw n n' =
          match n with
          | Empty -> n'
          | N(v, n'') -> N(v, cw n'' n')
        head <- cw head (toNode l)
        length <- length + l.Length

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
    let mutable array = arr

    let length () = array.Length
    let wrap v = Array.create 1 v

    new() = CListArray<'A>(Array.empty<'A>)
    new(v : 'A) = CListArray<'A>(Array.create 1 v)

    interface ICList<'A> with
      member this.HeadValue
        with get () =
          match array with
          | [||] -> None
          | _ -> Some array.[0]
      member this.Length with get () = length()
      member this.Tail
        with get () =
          match length() with
          | 0 -> this :> ICList<'A>
          | _ -> CListArray<'A>(Array.sub array 1 (length() - 1)) :> ICList<'A>
      member this.AddHead v =
        array <- Array.append (wrap v) array
      member this.AddTail v =
        array <- Array.append array (wrap v)
      member this.AddBefore v i =
        if (i < 0) || (i >= length()) then
          false
        else
          array <-
            Array.append
              (Array.append (Array.sub array 0 i) (wrap v))
                (Array.sub array i (length() - i + 1))
          true
      member this.RemoveHead () =
        match length() with
        | 0 -> false
        | _ ->
          array <- Array.sub array 1 (length() - 1)
          true
      member this.RemoveTail () =
        match length() with
        | 0 -> false
        | _ ->
          array <- Array.sub array 0 (length() - 1)
          true
      member this.RemoveAt i =
        if (i < 0) || (i >= length()) then
          false
        else
          match length() with
          | 0 -> false
          | _ ->
            array <-
              Array.append (Array.sub array 0 (i - 1)) 
                (Array.sub array i (length() - i))
            true
      member this.Find func =
        Array.tryFind func array
      member this.Append l =
        let rec toArr (l : ICList<'A>) =
          match l.Length with
          | 0 -> [||]
          | _ -> Array.append (wrap l.HeadValue.Value) (toArr (l.Tail))
        array <- Array.append array (toArr l)

    override this.ToString() =
      array.ToString()
  end

[<EntryPoint>]
let main argv =
  printfn "28. Реализация интерфейса списка на АТД:"
  let l1 = CListAdt<int>(3) :> ICList<int>;
  printfn "l1 = %A" l1
  printfn "// a\n> AddHead 1"
  l1.AddHead 1
  printfn "l1 = %A\n// b\n> l1.AddTail 4" l1
  l1.AddTail 4
  printfn "l1 = %A\n// c\n> l1.AddBefore 2 1" l1
  ignore (l1.AddBefore 2 1)
  printfn "l1 = %A\n> l1.AddBefore 0 0" l1
  ignore (l1.AddBefore 0 0)
  printfn "l1 = %A\n> l1.AddTail 5" l1
  l1.AddTail 5
  printfn "l1 = %A\n// d\n> l1.RemoveHead()" l1
  ignore (l1.RemoveHead())
  printfn "l1 = %A\n// e\n> l1.RemoveTail()" l1
  ignore (l1.RemoveTail())
  printfn "l1 = %A\n// f\n> l1.RemoveAt 1" l1
  ignore (l1.RemoveAt 1)
  printfn "l1 = %A\n> let l1' = l1.Tail" l1
  let l1' = l1.Tail
  printfn "l1' = %A" l1'
  printfn "// g\n> l1'.Find ((=) 3)\n%A"  (l1'.Find ((=) 3))
  printfn "// h\n> l1.Append l1'"
  l1.Append l1'
  printfn "l1 = %A\n" l1

  printfn "29. Реализация интерфейса списка на массиве:"
  let l2 = CListAdt<int>(3) :> ICList<int>;
  printfn "l2 = %A" l2
  printfn "// a\n> AddHead 1"
  l2.AddHead 1
  printfn "l2 = %A\n// b\n> l2.AddTail 4" l2
  l2.AddTail 4
  printfn "l2 = %A\n// c\n> l2.AddBefore 2 1" l2
  ignore (l2.AddBefore 2 1)
  printfn "l2 = %A\n> l2.AddBefore 0 0" l2
  ignore (l2.AddBefore 0 0)
  printfn "l2 = %A\n> l2.AddTail 5" l2
  l2.AddTail 5
  printfn "l2 = %A\n// d\n> l2.RemoveHead()" l2
  ignore (l2.RemoveHead())
  printfn "l2 = %A\n// e\n> l2.RemoveTail()" l2
  ignore (l2.RemoveTail())
  printfn "l2 = %A\n// f\n> l2.RemoveAt 1" l2
  ignore (l2.RemoveAt 1)
  printfn "l2 = %A\n> let l2' = l2.Tail" l2
  let l2' = l2.Tail
  printfn "l2' = %A" l2'
  printfn "// g\n> l2'.Find ((=) 3)\n%A"  (l2'.Find ((=) 3))
  printfn "// h\n> l2.Append l2'"
  l2.Append l2'
  printfn "l2 = %A\n" l2

  0
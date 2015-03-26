// Задачи 20-25 от 16 марта
// Автор: Кирилл Смиренко, группа 171
// Расчётное время выполнения: 3 часа
// Действительное время выполнения: 5 часов

// Задача 31 от 23 марта
// Расчётное время выполнения: 1 час
// Действительное время выполнения: 1,25 часа

module Spbu

open NUnit.Framework
open FsUnit

// 20. Интерфейс ориентированного графа, пригодный для решения дальнейших задач
type IGraph<'V> =
  interface
    // возвращает количество вершин
    abstract GetSize      : unit -> int
    // возвращает значение, "написанное" на вершине с данным индексом (с нуля)
    abstract GetValue     : int -> 'V
    // отвечает, есть ли прямое ребро из одной вершины в другую
    abstract HasEdge      : int -> int -> bool
    // возвращает список вершин, в которые есть прямое ребро из данной
    abstract GetOutEdges  : int -> int list
    // печатает граф на экран
    abstract Print        : unit -> unit
  end

// 21. Реализация интерфейса графа с матрицей смежности
type AdjMatrixGraph<'V> (vert : 'V [], edges : list<int * int>) =
  class
    let v = vert
    let size = v.Length
    let e = Array2D.create size size false

    do
      try
        for (v1, v2) in edges do
          Array2D.set e v1 v2 true
      with
        | _ -> printfn "Invalid input data!"

    interface IGraph<'V> with
      member this.GetSize () = size
      member this.GetValue i =
        if i < size then
          Array.get v i
        else
          failwith "Index out of range!"
      member this.HasEdge v1 v2 =
        if (v1 < size) && (v2 < size) then
          Array2D.get e v1 v2
        else
          false
      member this.GetOutEdges i =
        if i < size then
          let rec oe j l =
            if j < 0 then
              l
            elif Array2D.get e i j then
              oe (j - 1) (j::l)
            else
              oe (j - 1) l
          oe (size - 1) []
        else
          failwith "Index out of range!"
      member this.Print () =
        printfn "Вершины: %A" v
        printfn "Рёбра:\n%A" e
  end

// 22. Реализация интерфейса графа со списками смежности
type AdjListGraph<'V> (vert : 'V [], edges : list<int * int>) =
  class
    let v = vert
    let size = v.Length
    let e : int list [] = Array.create size List.empty

    do
      for (v1, v2) in edges do
        Array.set e v1 (v2::(Array.get e v1))

    interface IGraph<'V> with
      member this.GetSize () = size
      member this.GetValue i =
        if i < size then
          Array.get v i
        else
          failwith "Index out of range!"
      member this.HasEdge v1 v2 =
        if (v1 < size) && (v2 < size) then
          List.exists ((=) v2) (Array.get e v1)
        else
          false
      member this.GetOutEdges i =
        if i < size then
          Array.get e i
        else
          failwith "Index out of range!"
      member this.Print () =
        printfn "Вершины: %A" v
        printfn "Рёбра: %A" e
  end

// 23. По вершине вернуть список вершин, доступных из нее
// принимается индекс вершины, возвращается список "значений" вершин
let spreadOut (graph : IGraph<'V>) v =
  let s = graph.GetSize()
  if v >= s then
    failwith "Index out of range!"
  let visited = Array.create s false
  let rec dfs v =
    if not (Array.get visited v) then
      Array.set visited v true
      for i in graph.GetOutEdges v do
        dfs i
    else ()
  dfs v
  List.map graph.GetValue (List.filter (Array.get visited) [0 .. (s-1)]) |> List.sort

// 24. По вершине вернуть список вершин, из которых она доступна
let spreadIn (graph : IGraph<'V>) v =
  let s = graph.GetSize()
  if v >= s then
    failwith "Index out of range!"
  let search u =
    let visited = Array.create s false
    let rec dfs u =
      if u = v then
        true
      elif not (Array.get visited u) then
        Array.set visited u true
        List.fold (fun acc u -> acc || (dfs u)) false (graph.GetOutEdges u)
      else false
    dfs u
  List.map graph.GetValue (List.filter search [0 .. (s-1)])

// 25. Интерфейс полиморфного помеченного графа, использующий интерфейс из 20 задачи
type IMarkedGraph<'V, 'M> =
  interface
    inherit IGraph<'V>

    abstract HasEdgeByVal  : 'V -> 'V -> bool

    abstract GetMark  : 'V -> 'V -> 'M
  end

// 31. Unit-тесты для задач 23, 24
// Граф, используемый для тестов:
let graphMap =
    "1 --> 2 --> 3     7\n" +
    "      |     |     |\n" +
    "      |     |     |\n" +
    "      v     |     v\n" +
    "5 --> 4     |     8\n" +
    "|     ^     |     |\n" +
    "|     |     |     |\n" +
    "|     |     |     v\n" +
    "+---> 6 <---+     9"

[<TestFixture>]
type ``Тест графа с матрицей смежности`` () =  
  let g =
    new AdjMatrixGraph<int>([|1; 2; 3; 4; 5; 6; 7; 8; 9|],
      [(0, 1); (1, 2); (1, 3); (4, 3); (4, 5); (5, 3); (2, 5); (6, 7); (7, 8)])
        :> IGraph<int>
  [<Test>] member this.
    ``Из вершины 1 можно попасть в 1, 2, 3, 4, 6`` () =
      (sprintf "%A" (spreadOut g 0))
        |> should equal "[1; 2; 3; 4; 6]"
  [<Test>] member this.
    ``Из вершины 4 можно попасть в 4`` () =
      (sprintf "%A" (spreadOut g 3))
        |> should equal "[4]"
  [<Test>] member this.
    ``Из вершины 8 можно попасть в 8, 9`` () =
      (sprintf "%A" (spreadOut g 7))
        |> should equal "[8; 9]"  
  [<Test>] member this.
    ``В вершину 4 можно попасть из 1, 2, 3, 4, 5, 6`` () =
      (sprintf "%A" (spreadIn g 3))
        |> should equal "[1; 2; 3; 4; 5; 6]"  
  [<Test>] member this.
    ``В вершину 7 можно попасть из 7`` () =
      (sprintf "%A" (spreadIn g 6))
        |> should equal "[7]"
  [<Test>] member this.
    ``В вершину 8 можно попасть из 7, 8`` () =
      (sprintf "%A" (spreadIn g 7))
        |> should equal "[7; 8]"

[<TestFixture>]
type ``Тест графа со списком смежности`` () =  
  let g =
    new AdjListGraph<int>([|1; 2; 3; 4; 5; 6; 7; 8; 9|],
      [(0, 1); (1, 2); (1, 3); (4, 3); (4, 5); (5, 3); (2, 5); (6, 7); (7, 8)])
        :> IGraph<int>
  [<Test>] member this.
    ``Из вершины 1 можно попасть в 1, 2, 3, 4, 6`` () =
      (sprintf "%A" (spreadOut g 0))
        |> should equal "[1; 2; 3; 4; 6]"
  [<Test>] member this.
    ``Из вершины 4 можно попасть в 4`` () =
      (sprintf "%A" (spreadOut g 3))
        |> should equal "[4]"
  [<Test>] member this.
    ``Из вершины 8 можно попасть в 8, 9`` () =
      (sprintf "%A" (spreadOut g 7))
        |> should equal "[8; 9]"  
  [<Test>] member this.
    ``В вершину 4 можно попасть из 1, 2, 3, 4, 5, 6`` () =
      (sprintf "%A" (spreadIn g 3))
        |> should equal "[1; 2; 3; 4; 5; 6]"  
  [<Test>] member this.
    ``В вершину 7 можно попасть из 7`` () =
      (sprintf "%A" (spreadIn g 6))
        |> should equal "[7]"
  [<Test>] member this.
    ``В вершину 8 можно попасть из 7, 8`` () =
      (sprintf "%A" (spreadIn g 7))
        |> should equal "[7; 8]"

[<EntryPoint>]
let main argv =
  let g1 =
    new AdjMatrixGraph<int>([|1; 2; 3; 4|],
      [(0, 1); (0, 3); (1, 0); (3, 0)])
  let g2 =
    new AdjListGraph<char>([|'A'; 'B'; 'C'; 'D'; 'E'; 'F' |],
      [(0, 1); (1, 2); (2, 3); (4, 0); (1, 5); (4, 5); (5, 0)])
  printfn "21. Граф с матрицей смежности:"
  (g1 :> IGraph<int>).Print()
  printfn "\n22. Граф со списками смежности:"
  (g2 :> IGraph<char>).Print()
  printfn "\n23. Cписок вершин, доступных из вершины А второго графа:\n%A"
    ((spreadOut g2 0).ToString())
  printfn "\n24. Cписок вершин, из которых доступна из вершина А второго графа:\n%A"
    (spreadIn g2 0)
  0
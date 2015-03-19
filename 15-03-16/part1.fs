// Задачи 20-25 от 16 марта
// Автор: Кирилл Смиренко, группа 171
// Расчётное время выполнения: 3 часа (c 18:30)
// Действительное время выполнения: 3 часа +...

// 20. Интерфейс ориентированного графа, пригодный для решения дальнейших задач
type IGraph =
  interface
    abstract HasEdge      : int -> int -> bool
    // для задания 23
    abstract GetOutEdges  : int -> int list // TODO: remove
    // для задания 24
    abstract GetInEdges   : int -> int list
    // решил, что лучше сделать различные реализации заданий 23-24,
    // чтобы учитывать конкретную реализацию графа
  end

// 21. Реализация интерфейса графа с матрицей смежности
type AdjMatrixGraph (s : int, e : list<int * int>) =
  class
    let size = s
    let matrix = Array2D.create size size false

    do
      try
        for (u, v) in e do
          Array2D.set matrix u v true
      with
        | _ -> printfn "Invalid input data!"

    interface IGraph with
      member this.HasEdge v1 v2 =
        if ((v1 < size) && (v2 < size)) then
          Array2D.get matrix v1 v2
        else
          false
      member this.GetOutEdges v = // TODO
        if (v < size) then
          let rec oe i l =
            if i < 0 then
              l
            elif Array2D.get matrix v i then
              oe (i - 1) (i::l)
            else
              oe (i - 1) l
          oe (size - 1) []
        else
          []
      member this.GetInEdges v = // TODO
        if (v < size) then
          let rec oe i l =
            if i < 0 then
              l
            elif Array2D.get matrix i v then
              oe (i - 1) (i::l)
            else
              oe (i - 1) l
          oe (size - 1) []
        else
          []
  end

// 22. Реализация интерфейса графа со списками смежности
type AdjListGraph (s, e : list<int * int>) =
  class
    let size = s
    let edges : int list [] = Array.create size List.empty

    do
      for (u, v) in e do
        Array.set edges u (v::(Array.get edges u))

    interface IGraph with
      member this.HasEdge u v =
        if (u < size) then
          List.exists (fun v' -> v = v') (Array.get edges u)
        else
          false
      member this.GetOutEdges v =
        Array.get edges v // TODO
      member this.GetInEdges v =
        [] // TODO
       //Array.foldBack (List.foldBack ) edges []
  end

// 23. По вершине вернуть список вершин, доступных из нее

// 24. По вершине вернуть список вершин, из которых она доступна

// 25. Интерфейс полиморфного помеченного графа, использующий интерфейс из 20 задачи
type IMarkedGraph<'K, 'M> =
  interface
    inherit IGraph
    // метод HasEdge из IGraph здесь будет принимать в качестве аргументов
    // индексы проверяемых вершин, а HasEdgeK - "обозначения вершин" полиморфного типа

    abstract HasEdgeK  : 'K -> 'K -> bool

    abstract GetMark  : 'K -> 'K -> 'M
  end

[<EntryPoint>]
let main argv =
  let graph1 = new AdjMatrixGraph(4, [(0, 1); (0, 3)])
  let graph2 = new AdjListGraph(4, [(0, 1); (0, 3)])
  printfn "21. Граф с матрицей смежности:\n"
  printfn "22. Граф со списками смежности:\n"
  0
// Задача 26 от 16 марта
// Автор: Кирилл Смиренко, группа 171
// Расчётное время выполнения: 1 час
// Действительное время выполнения: 1,5 часа

// Задача 32 от 23 марта (плюс мелкие исправления, в частности, по части рандома)
// Расчётное время выполнения: 1,5 часа
// Действительное время выполнения: 2,5 часа

open System
open NUnit.Framework
open FsUnit

let GetInfectProb os =
  match os with
  | "Windows" -> 0.4
  | "Linux" -> 0.15
  | "OS X" -> 0.05
  | "Android" -> 0.3
  | _ -> 0.0

type Computer(osName : string) =
  class
    let os = osName
    let mutable infected = false

    member this.Infect() =
      infected <- true

    member this.IsInfected() =
      infected

    member this.TryInfect(rnd : Random) =
      if (not infected) && (rnd.NextDouble() < GetInfectProb os) then
        infected <- true
      else ()

    override this.ToString() =
      "{ " + (if infected then "Infected!" else "Not infected") + "\t" +
      (GetInfectProb os).ToString() + "\t" + "\t" + os + " }"
  end

type Network
  (comps : string [], edges : list<int * int>, infected : int list, r : Random) =
  class
    let c = Array.map (fun n -> new Computer(n)) comps
    let s = c.Length
    let e : int list [] = Array.create s List.empty
    let rnd = r
    let mutable time = 0

    do
      for (u, v) in edges do
        Array.set e u (v::(Array.get e u))
        Array.set e v (u::(Array.get e v))
      for i in infected do
        c.[i].Infect()

    member this.PrintStats() =
      printfn "\nTime: %A" time
      for m in c do
        printfn "%A" m
    
    member this.Live() =
      time <- time + 1
      let q = Array.filter (fun i -> c.[i].IsInfected()) [| 0 .. s-1 |]
      for i in q do
        for v in e.[i] do
          c.[v].TryInfect(rnd)
    
    member this.GetInfectMarkers() =
      Array.map  (fun i -> (if (c.[i].IsInfected()) then "!" else " ")) [| 0 .. s-1 |]
  end

let net =
  new Network([| "Windows"; "Windows"; "Windows"; "Linux"; "Linux"; "Linux";
    "Android"; "Android"; "Android"; "OS X"; "OS X"; "OS X";
      "Windows"; "Linux"; "Windows"; "Linux" |],
        [(0, 1); (0, 6); (0, 7); (1, 2); (2, 6); (3, 4);
          (3, 5); (3, 6); (6, 9); (7, 8); (8, 9); (10, 11);
            (12, 13); (12, 15); (13, 14); (14, 15)],
              [0; 10; 13], new System.Random())

type CustomRandom (nd : unit -> float) =
  class
    inherit System.Random()
    override this.NextDouble() =
      nd()
  end

[<TestFixture>]
type ``Тест линейной сети где зараза не распространяется`` () =
  let net =
    new Network([| "Windows"; "Android"; "OS X"; "Linux" |],
      [(0, 1); (1, 2); (2, 3)], [1], new CustomRandom(fun () -> 1.0))
  let drawNet (ar : string []) =
    "W" + ar.[0] + "--- A" + ar.[1] + "--- M" + ar.[2] + "--- L" + ar.[3]
  [<Test>] member this.
    ``Через 100 минут никто не заражен, кроме одного`` () =
      (for i in 1 .. 100 do net.Live());
      net.GetInfectMarkers() |> drawNet
        |> should equal "W --- A!--- M --- L "
 
[<TestFixture>]
type ``Тест линейной сети где зараза всегда распространяется`` () =
    let drawNet (ar : string []) =
      "W" + ar.[0] + "--- A" + ar.[1] + "--- M" + ar.[2] + "--- L" + ar.[3]
         + "--- W" + ar.[4]
    let v = [| "Windows"; "Android"; "OS X"; "Linux"; "Windows" |]
    let e = [(0, 1); (1, 2); (2, 3); (3, 4)]
    let inf = [1]
    let rnd = new CustomRandom(fun () -> 0.0)
    [<Test>]
    member this.``Через 1 минуту заражаются соседи`` () =
      let net =
        new Network(v, e, inf, rnd)
      net.Live()
      net.GetInfectMarkers() |> drawNet
        |> should equal "W!--- A!--- M!--- L --- W "
    [<Test>]
    member this.``Через 2 минуты заражены все кроме последнего`` () =
      let net =
        new Network(v, e, inf, rnd)
      for i in 1 .. 2 do net.Live()
      net.GetInfectMarkers() |> drawNet
        |> should equal "W!--- A!--- M!--- L!--- W "
    [<Test>]
    member this.``Через 3 минуты заражены все`` () =
      let net =
        new Network(v, e, inf, rnd)
      for i in 1 .. 3 do net.Live()
      net.GetInfectMarkers() |> drawNet
        |> should equal "W!--- A!--- M!--- L!--- W!"

[<TestFixture>]
type ``Тест разветвленной сети где зараза всегда распространяется`` () =
    let drawNet (ar : string []) =
      "W" + ar.[0] + "--- L" + ar.[1] + "    W" + ar.[2] + "\n" +
      "|     |     |\n|     |     |\n" +
      "A" + ar.[3] + "--- M" + ar.[4] + "--- W" + ar.[5] + "\n" +
      "      |\n      |\n" +
      "      W" + ar.[6]
    let v = [| "Windows"; "Linux"; "Windows"; "Android"; "OS X"; "Windows"; "Windows" |]
    let e = [(0, 1); (0, 3); (1, 4); (3, 4); (4, 5); (2, 5); (4, 6)]
    let inf = [3]
    let rnd = new CustomRandom(fun () -> 0.0)
    [<Test>]
    member this.``Через 1 минуту заражено 3 компа`` () =
      let net =
        new Network(v, e, inf, rnd)
      net.Live()
      net.GetInfectMarkers() |> drawNet
        |> should equal
          ("W!--- L     W \n" +
          "|     |     |\n" +
          "|     |     |\n" +
          "A!--- M!--- W \n" +
          "      |\n" +
          "      |\n" +
          "      W ")
    [<Test>]
    member this.``Через 2 минуты заражено 6 компов`` () =
      let net =
        new Network(v, e, inf, rnd)
      for i in 1 .. 2 do net.Live()
      net.GetInfectMarkers() |> drawNet
        |> should equal
          ("W!--- L!    W \n" +
          "|     |     |\n" +
          "|     |     |\n" +
          "A!--- M!--- W!\n" +
          "      |\n" +
          "      |\n" +
          "      W!")
    [<Test>]
    member this.``Через 3 минуты заражены все`` () =
      let net =
        new Network(v, e, inf, rnd)
      for i in 1 .. 3 do net.Live()
      net.GetInfectMarkers() |> drawNet
        |> should equal
          ("W!--- L!    W!\n" +
          "|     |     |\n" +
          "|     |     |\n" +
          "A!--- M!--- W!\n" +
          "      |\n" +
          "      |\n" +
          "      W!")

//   W ------- W     L         L!-- W
//   |         |     | \       |    |
//   |         |     |  \      |    |
//   W!------- A --- L -- L    W -- L
//   |         |
//   |         |
//   A -- A -- M     M!-- M
// я старался

let rec loop сnt =
  for i in 1 .. сnt do
    net.Live()
  net.PrintStats()
  match Console.ReadKey().Key with
  | ConsoleKey.F ->
    loop 1
  | ConsoleKey.G ->
    loop 5
  | ConsoleKey.H ->
    loop 10
  | _ -> ()

[<EntryPoint>]
let main argv =
  printfn "F = 1 шаг\nG = 5 шагов\nH = 10 шагов\n_ = выход"
  printfn "Начальный статус:"
  loop 0
  0
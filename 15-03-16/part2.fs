// Задача 26 от 16 марта
// Автор: Кирилл Смиренко, группа 171
// Расчётное время выполнения: 1 час
// Действительное время выполнения: 1,5 часа

open System

let rnd = System.Random()

let GetInfectProb os =
  match os with
  | "Windows" -> 0.4
  | "Linux" -> 0.15
  | "OS X" -> 0.05
  | "Android" -> 0.3
  | _ -> failwith "What kind of OS is that?"

type Computer(osName : string) =
  class
    let os = osName
    let mutable infected = false

    member this.Infect() =
      infected <- true

    member this.IsInfected() =
      infected

    member this.TryInfect() =
      if (not infected) && (rnd.NextDouble() < GetInfectProb os) then
        infected <- true
      else ()

    override this.ToString() =
      "{ " + (if infected then "Infected!" else "Not infected") + "\t" +
      (GetInfectProb os).ToString() + "\t" + "\t" + os + " }"
  end

type Network (comps : string [], edges : list<int * int>, infected : int list) =
  class
    let c = Array.map (fun n -> new Computer(n)) comps
    let s = c.Length
    let e : int list [] = Array.create s List.empty
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
          c.[v].TryInfect()
  end

let net =
  new Network([| "Windows"; "Windows"; "Windows"; "Linux"; "Linux"; "Linux";
    "Android"; "Android"; "Android"; "OS X"; "OS X"; "OS X";
      "Windows"; "Linux"; "Windows"; "Linux" |],
        [(0, 1); (0, 6); (0, 7); (1, 2); (2, 6); (3, 4);
          (3, 5); (3, 6); (6, 9); (7, 8); (8, 9); (10, 11);
            (12, 13); (12, 15); (13, 14); (14, 15)],
              [0; 10; 13])

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
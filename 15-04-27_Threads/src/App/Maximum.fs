// 46. Параллельный max на массиве

module Maximum

open System
open System.Threading
open Util

let localMax (arr : int []) l r =
  let mutable res = Int32.MinValue
  for i in l .. r do
    res <- Math.Max(res, arr.[i])
  res

let maxMT threadNumber (arr : int []) =
  let rec reduce (arr : int []) = 
    match arr.Length with
    | l when l <= threadNumber * 2 ->
      Array.max arr
    | l ->
      let res = ref []
      let step = l / threadNumber
      let threadArray = Array.init threadNumber (fun i ->
          new Thread(ThreadStart(fun _ ->
              let threadRes = localMax arr (i * step) ((i+1) * step - 1)
              lock res (fun _ -> res := threadRes :: !res)
            ))
        )
      for t in threadArray do
        t.Start()
      for t in threadArray do
        t.Join()
      reduce (List.toArray !res)
  reduce arr

let tryMax (arr : int []) =
  printfn "\tNo threading:\t%A" (measureTime ( fun() -> Array.max arr ))
  let testcase n =
    printfn "\t%A threads:\t%A" n (measureTime ( fun() -> maxMT n arr ))
  for i in [1; 2; 4; 8; 10] do
    testcase i
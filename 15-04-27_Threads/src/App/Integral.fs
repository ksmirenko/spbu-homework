// 47. Параллельный определенный интеграл
// Предполагается, что функция интегрируема на данном промежутке

module Integral

open System
open System.Threading
open Util

let localArea (f : float -> float) l r eps =
  let mutable res = 0.0
  for i in l .. eps .. (r - eps) do
    res <- res + 0.5 * (f(i) + f(i + eps)) * eps
  res

let defIntegralMT (threadNumber : int) f (l : float) (r : float) eps =
  let res = ref 0.0
  let step = (r - l) / (float threadNumber)
  let threadArray = Array.init threadNumber (fun i ->
      new Thread(ThreadStart(fun _ ->
          let threadRes =
            localArea f (l + step * (float i)) (l + step * (float (i + 1))) eps
          lock res (fun _ -> res := !res + threadRes)
        ))
    )
  for t in threadArray do
    t.Start()
  for t in threadArray do
    t.Join()
  !res

let tryIntegral f l r eps =
  let testcase n =
    printfn "\t%A threads:\t%A"
      n (measureTime ( fun() -> defIntegralMT n f l r eps))
  for n in [1; 2; 4; 8; 10] do
    testcase n
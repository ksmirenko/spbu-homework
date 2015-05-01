module Util

let measureTime f =
  let timer = new System.Diagnostics.Stopwatch()
  timer.Start()
  let returnValue = f()
  timer.Stop()
  printf "Time: %A" timer.Elapsed.TotalMilliseconds
  returnValue
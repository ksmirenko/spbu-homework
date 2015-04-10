module Program

open System.IO
open IOProviders
open Interpreter

[<EntryPoint>]
let main argv = 
  if argv.Length < 1 then printfn "Not enough arguments.\n"; exit 1
  let inStream = new StreamReader(argv.[0])
  let code = inStream.ReadToEnd ()
  inStream.Dispose()
  let ip = new ConsoleInputProvider()
  let op =
    if argv.Length < 2 then
      new ConsoleOutputProvider() :> IOutputProvider
    else
      new StringOutputProvider() :> IOutputProvider
  match interpret code ip op with
  | ErrNo ->
    match op with
    | :? StringOutputProvider as op' ->
      let outStream = new StreamWriter(argv.[1])
      outStream.Write(op'.GetString())
      outStream.Dispose()
      0
    | _ -> 0
  | err ->
    printfn "Error! %A" err
    1

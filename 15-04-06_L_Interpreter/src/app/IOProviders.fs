module IOProviders

open System

exception IOException'

type IInputProvider =
  interface
    abstract Read : unit -> int
  end

type IOutputProvider =
  interface
    abstract Write : int -> unit
  end

type ConsoleInputProvider() =
  class
    interface IInputProvider with
      member this.Read () =
        try
          Console.ReadLine() |> Int32.Parse
        with
        | _ -> raise IOException'
  end

type ListInputProvider(sl : int list) =
  class
    let mutable l = sl

    interface IInputProvider with
      member this.Read () =
        match l with
        | [] -> raise IOException'
        | s::t ->
          l <- t
          s
  end

type ConsoleOutputProvider() =
  class
    interface IOutputProvider with
      member this.Write s =
        try
          Console.WriteLine(s.ToString())
        with
        | _ -> raise IOException'
  end

type StringOutputProvider() =
  class
    let mutable str = ""

    interface IOutputProvider with
      member this.Write s =
        str <- str + s.ToString() + "\n"

    member this.GetString() = str
  end
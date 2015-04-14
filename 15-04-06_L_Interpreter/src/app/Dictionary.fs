module Dictionary

open System

type Dict<'K, 'V when 'K : equality>()   = 
  class
    let mutable l = []

    member this.Store (k : 'K, v : 'V) =
      l <- (k, v)::(List.filter (fun (k', _) -> k <> k') l)
    
    member this.TryLookup k =
      match List.tryFind (fun (k', _) -> k = k') l with
      | None -> None
      | Some (_, v) -> Some v
  end
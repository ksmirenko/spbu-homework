// Задачи 37-38 от 23 марта
// Автор: Кирилл Смиренко, группа 171
// Расчётное время выполнения: 2 часа
// Действительное время выполнения: (с 12:30)

// корректным представлением отрицательного числа во входе считается "(-V)"
// названия переменных могут содержать только латинские буквы нижнего регистра

open System
open System.IO
open Checked
open NUnit.Framework
open FsUnit

type Token =
  | TNum of int 
  | TOp of char * int * bool // tag * precedence * left-assoc
  | TParO
  | TParC

exception LexicalError of string
exception SyntaxError of string

// пытается строку преобразовать в int
// или подставить значение переменной с таким названием
let parseValue (context : (string * int) list) str =
  try
    TNum(Int32.Parse str)
  with
  | _ ->
    match context with
    | [] -> raise (LexicalError("Error at stage 1: Incorrect number format!"))
    | _ ->
      try
        TNum(snd (List.find (fun (k, v) -> str = k) context))
      with
      | _ -> raise (LexicalError("Error at stage 1: Unknown expression!"))

// разбирает строку на токены
let parse str =
  let rec tillClose (s : string) n =
    if s.Length > 0 then
      match s.[0] with
      | c when c >= '0' && c <= '9' ->
        tillClose (s.Substring 1) (n + c.ToString())
      | ')' ->
        try
          true, (s.Substring 1), (Int32.Parse n)
        with
        | _ -> false, "", 0
      | _ ->
        false, "", 0
    else
      false, "", 0
  let rec f (str : string) (l : Token list) irn num =
    if str = "" then
      if irn then
        List.rev (TNum(Int32.Parse num)::l)
      else
        List.rev l
    else
      let c = str.[0]
      let str' = str.Substring 1
      match c with
      | c when (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') ->
        f str' l true (num + c.ToString())
      | _ ->
        let l' =
          if irn then
            TNum(Int32.Parse num)::l
          else l
        match c with
        | ' ' ->
           f str' l' false ""
        | '+' | '-' ->
          f str' (TOp(c, 2, true)::l') false ""
        | '*' | '/' | '%' ->
          f str' (TOp(c, 3, true)::l') false ""
        | '^' ->
          f str' (TOp(c, 4, false)::l') false ""
        | ')' ->
          f str' (TParC::l') false ""
        | '(' ->
          if str'.Length > 0 && str'.[0] = '-' then
            match tillClose (str'.Substring 1) "" with
            | (false, _, _) ->  
              raise (LexicalError("Error at stage 1: Incorrect negative number format!"))
            | (true, str'', n) ->
              f str'' (TNum(-n)::l') false ""
          else
            f str' (TParO::l') false ""
        | _ ->
          raise (
            LexicalError(
              "Error at stage 1: Unknown symbol '" + c.ToString() + "' in input!"))
  f str [] false ""

// переводит поток токенов в инфиксной записи в постфиксную запись
let toPostfix stream =
  let rec op stack output s p la =
    match stack with
    | (TOp(s', p', la') as op') :: st when
      (la && p <= p') || ((not la) && p < p') ->
        op st (op' :: output) s p la
    | _ ->
      (TOp(s, p, la) :: stack), output
  let rec close stack output =
    match stack with
    | [] ->
      raise (SyntaxError("Error at stage 2: Mismatched parentheses!"))
    | TParO :: tail ->
      tail, output
    | t :: tail ->
      close tail (t::output)
  let rec finalize stack output =
    match stack with
    | [] -> output
    | TParO::t | TParC::t ->
      raise (SyntaxError("Error at stage 2: Mismatched parentheses!"))
    | h::t -> finalize t (h::output)
  let rec f stream (stack, output) =
    match stream with
    | TNum(n) :: tail ->
      f tail (stack, ((TNum(n))::output))
    | TOp(s, p, la) :: tail ->
      f tail (op stack output s p la)
    | TParO :: tail ->
      f tail ((TParO :: stack), output)
    | TParC :: tail ->
      f tail (close stack output)
    | [] ->
      finalize stack output
  List.rev (f stream ([], []))

// вычислительная часть задания 37
[<TestCase ("1 + 2", Result = "1\n2\n+\n")>]
[<TestCase ("7 * (6 + 5)", Result = "7\n6\n5\n+\n*\n")>]
[<TestCase ("3+ 7 *(1 -5) ^2 ^ 3 / 1024",
  Result = "3\n7\n1\n5\n-\n2\n3\n^\n^\n*\n1024\n/\n+\n")>]
[<TestCase ("(4^(5 % 3) + (-68)) / ((-6)* (-2))",
  Result = "4\n5\n3\n%\n^\n-68\n+\n-6\n-2\n*\n/\n")>]
let toVerticalPostfix (expr : string) =
  try
    let pt t =
      match t with
      | TNum(n) -> n.ToString() + "\n"
      | TOp(c, _, _) -> c.ToString() + "\n"
      | _ -> "" // забиваем на скобки, предполагая, что в постфиксной записи их нет
    expr |> parse |> toPostfix |> (List.fold (fun s t -> s + (pt t)) "")
  with
  | _ -> "Error: Could not process input expression!"

// 37. Перевод выражения без переменных в вертикальную постфиксную запись
let task37 (fin : string) (fout : string) =
  try
    use streamIn = new StreamReader(fin)
    use streamOut = new StreamWriter(fout)
    let expr = streamIn.ReadToEnd ()
    streamOut.Write(expr |> toVerticalPostfix)
  with
  | ex -> printf "I/O error!\n%A" ex

// вычислительная часть задания 38
let processCommands (comms : string list) =
  let rec powInt n k =
    if k = 0 then
      1
    else
      let t = (powInt n (k / 2))
      if k % 2 = 0 then t * t else t * t * n
  let rec pr comms st =
    match comms with
    | [] ->
      match st with
      | top::[] -> top.ToString()
      | _ -> "Error: Incorrect token sequence!"
    | s::t ->
      match s with
      | "" | " " -> pr t st
      | _ ->
        match Int32.TryParse(s) with
        | (true, n) -> pr t (n::st)
        | (false, _) ->
          match st with
          | tr::tl::st' ->
            match s with
            | "+" -> pr t ((tl + tr)::st')
            | "-" -> pr t ((tl - tr)::st')
            | "*" -> pr t ((tl * tr)::st')
            | "/" ->
              match tr with
              | 0 -> "Error: Division by zero!"
              | _ -> pr t ((tl / tr)::st')
            | "%" -> pr t ((tl % tr)::st')
            | "^" -> pr t ((powInt tl tr)::st')
            | _ -> "Error: Unknown operator!"
          | _ -> "Error: Incorrect token sequence!"
  try
    pr comms []
  with
  | :? OverflowException -> "Error: Arithmetic overflow!"
  | exc -> "Error: " + exc.Message

// 38. Стековая машина.
[<TestCase ("test38.in.txt", Result = "77")>] // проверка работы с файлами
let task38 (fin : string) =
  try
    use streamIn = new StreamReader(fin)
    let input = streamIn.ReadToEnd ()
    input.Split([|'\n'|]) |> Array.toList |> processCommands
  with
  | ex -> sprintf "I/O error!\n%A" ex

// ------------------------------------------------------------------------------------
// проверка работы с файлами
[<TestCase ("test37.in.txt", "test.out", Result = "7\n6\n5\n+\n*\n")>]
let testFiles37 (fin : string) (fout : string) =
  task37 fin fout
  use stream = new StreamReader(fout)
  stream.ReadToEnd()

[<TestCase ("-42", Result = "-42")>]
[<TestCase ("1\n2\n+\n", Result = "3")>]
[<TestCase ("1\n2\n3\n/\n2\n^\n+", Result = "1")>]
[<TestCase ("7\n6\n5\n+\n*\n", Result = "77")>]
[<TestCase ("3\n7\n1\n5\n-\n2\n3\n^\n^\n*\n1024\n/\n+\n", Result = "451")>]
[<TestCase ("4\n5\n3\n%\n^\n-68\n+\n-6\n-2\n*\n/\n",
  Result = "-4")>]
let test38 (str : string) =
  str.Split([|'\n'|]) |> Array.toList |> processCommands

[<TestCase ("")>]
[<TestCase ("-44\n0\n/\n")>]
[<TestCase ("1\ntrash\n2\n+\n")>]
[<TestCase ("7\n6\n5\n+\n*\n^")>]
[<TestCase ("2147483646\n2\n+\n")>]
let testErrors38 (str : string) =
  str.Split([|'\n'|]) |> Array.toList |> processCommands |>
    should startWith "Error"

[<EntryPoint>]
let main argv =
  0
// Задачи 35-36 от 23 марта
// Автор: Кирилл Смиренко, группа 171
// Расчётное время выполнения: 5 часов
// Действительное время выполнения: 7 часов

// корректным представлением отрицательного числа во входе считается "(-V)"
// названия переменных могут содержать только латинские буквы нижнего регистра

open System
open Checked
open NUnit.Framework
open FsUnit

type Token =
  | TNum of int 
  | TOp of char * int * bool // tag * precedence * left-assoc
  | TParO
  | TParC

type Expr =
  | V of int
  | Plus of Expr * Expr
  | Minus of Expr * Expr
  | Mult of Expr * Expr
  | Div of Expr * Expr
  | Mod of Expr * Expr
  | Pow of Expr * Expr

exception LexicalError of string
exception SyntaxError of string
exception RuntimeError of string

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

// разбирает строку на токены (без формирования дерева)
let parse (context : (string * int) list) str =
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
        List.rev ((parseValue context num)::l)
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
            (parseValue context num)::l
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

// по токенам в постфиксной записи строит синтаксическое дерево
let buildTree (queue : Token list) =
  let rec f q st =
    match q with
    | TNum(n) :: q' -> f q' (V(n)::st)
    | TOp(s, _, _) :: q' ->
      match st with
      | tr::tl::st' ->
        match s with
        | '+' -> f q' (Plus(tl, tr)::st')
        | '-' -> f q' (Minus(tl, tr)::st')
        | '*' -> f q' (Mult(tl, tr)::st')
        | '/' -> f q' (Div(tl, tr)::st')
        | '%' -> f q' (Mod(tl, tr)::st')
        | '^' -> f q' (Pow(tl, tr)::st')
        | _ -> raise (LexicalError("Error at stage 3: Unknown operator!"))
      | _ -> raise (SyntaxError("Error at stage 3: Incorrect token sequence!"))
    | TParO::_ | TParC::_ ->
      raise (SyntaxError("Error at stage 3: What are parentheses doing in RPN?"))
    | [] ->
      match st with
      | exp::[] -> exp
      | _ -> raise (SyntaxError("Error at stage 3: Incorrect token sequence!"))
  f queue []

// вычисляет выражение в дереве
let rec calculate ex =
  try 
    match ex with
    | V(v) -> v
    | Plus(a, b) -> (calculate a) + (calculate b)
    | Minus(a, b) -> (calculate a) - (calculate b)
    | Mult(a, b) -> (calculate a) * (calculate b)
    | Div(a, b) -> (calculate a) / (calculate b)
    | Mod(a, b) -> (calculate a) % (calculate b)
    | Pow(a, b) ->
      let rec powInt n k =
        if k = 0 then
          1
        else
          let t = (powInt n (k / 2))
          if k % 2 = 0 then t * t else t * t * n
      powInt (calculate a) (calculate b)
  with
  | :? OverflowException ->
    raise (RuntimeError("Error at stage 4: Arithmetic overflow!"))
  | :? DivideByZeroException ->
    raise (RuntimeError("Error at stage 4: Division by zero!"))
  | exc ->
    raise (RuntimeError("Error at stage 4: " + exc.Message))

// 36. Калькулятор выражений, в которых есть переменные.
let calc2 (context : (string * int) list) str =
  try
    (
      str |> (parse context) |> toPostfix |> buildTree |> calculate
    ).ToString()
  with
  | exc -> (sprintf "%A" exc)

// 35. Калькулятор выражений (+, -, *, /, %, ^, скобки )
let calc str = (calc2 [] str)
// ------------------------------------------------------------------------------------
// Тесты

[<Test>]
let test01 () =
  calc "3+ 7 *(1 -5) ^2 ^ 3 / 1024"
  |> should equal "451"

[<Test>]
let test02 () =
  calc "1 + (2 / 3) ^2" // из условия
  |> should equal "1"

[<Test>]
let test03 () =
  calc "(4^(5 % 3) + (-68)) / ((-6)* (-2))"
  |> should equal "-4"

[<Test>]
let testFail01 () =
  calc "(4^(5 % 3) + (-68)) / (-6 * (-2))"
  |> should startWith "LexicalError"

[<Test>]
let testFail02 () =
  calc "(6 + 7"
  |> should startWith "SyntaxError"

[<Test>]
let testFail03 () =
  calc "(-42) / 0"
  |> should startWith "RuntimeError"

[<Test>]
let testDict01 () =
  (calc2 [ ("a", 3) ] "1 + (2 / a) ^2")
  |> should equal "1"

[<Test>]
let testDict02 () =
  (calc2 [ ("lol", -68); ("three", 3) ] "(4^(5 % three) + lol) / ((-6)* (-2))")
  |> should equal "-4"

// --------------------------------------------------------------------------------------
// далее идут не слишком содержательные тесты,
// но я писал "этапы" обработки отдельно и по технике TDD

[<Test>]
let testParse01 () =
  (
    try
      sprintf "%A" (parse [] "1 + 2")
    with
    | ex -> sprintf "%A" ex)
    |> should equal (sprintf "%A" [TNum(1); TOp('+', 2, true); TNum(2)])

[<Test>]
let testParse02 () =
  (
    try
      sprintf "%A" (parse [] "1 + (-2) * 3")
    with
    | ex -> sprintf "%A" ex)
    |> should equal (
      sprintf "%A" [TNum(1); TOp('+', 2, true); TNum(-2); TOp('*', 3, true); TNum(3)])

[<Test>]
let testParse03 () =
  (
    try
      sprintf "%A" (parse [] "3+ 7 *(1 -5) ^2 ^ 3 / 1024")
    with
    | ex -> sprintf "%A" ex)
    |> should equal (
      sprintf "%A" [
        TNum(3); TOp('+', 2, true); TNum(7); TOp('*', 3, true);
        TParO; TNum(1); TOp('-', 2, true); TNum(5); TParC;
        TOp('^', 4, false); TNum(2); TOp('^', 4, false);
        TNum(3); TOp('/', 3, true); TNum(1024);])

[<Test>]
let testToPostfix01 () =
  sprintf "%A" (toPostfix [TNum(1); TOp('+', 2, true); TNum(2)])
    |> should equal (sprintf "%A" [TNum(1); TNum(2); TOp('+', 2, true)])

[<Test>]
let testToPostfix02 () =
  sprintf "%A" (toPostfix
    [TNum(1); TOp('+', 2, true); TNum(-2); TOp('*', 3, true); TNum(3)])
    |> should equal (sprintf "%A"
      [TNum(1); TNum(-2); TNum(3); TOp('*', 3, true); TOp('+', 2, true)])

[<Test>]
let testToPostfix03 () =
  sprintf "%A" (toPostfix
    [
      TNum(3); TOp('+', 2, true); TNum(7); TOp('*', 3, true);
      TParO; TNum(1); TOp('-', 2, true); TNum(5); TParC;
      TOp('^', 4, false); TNum(2); TOp('^', 4, false); TNum(3);
      TOp('/', 3, true); TNum(1024);])
    |> should equal (sprintf "%A"
      [
        TNum(3); TNum(7); TNum(1); TNum(5); TOp('-', 2, true);
        TNum(2); TNum(3); TOp('^', 4, false); TOp('^', 4, false);
        TOp('*', 3, true); TNum(1024); TOp('/', 3, true); TOp('+', 2, true)])

[<Test>]
let testBuildTree01 () =
  sprintf "%A" (buildTree [TNum(1); TNum(2); TOp('+', 2, true)])
    |> should equal ( sprintf "%A" (Plus(V(1), V(2))) )

[<Test>]
let testBuildTree02 () =
  sprintf "%A" (
    buildTree [TNum(1); TNum(-2); TNum(3); TOp('*', 3, true); TOp('+', 2, true)])
    |> should equal ( sprintf "%A" (Plus(V(1), Mult(V(-2), V(3)))) )

[<Test>]
let testBuildTree03 () =
  sprintf "%A" (
    buildTree [
      TNum(3); TNum(7); TNum(1); TNum(5); TOp('-', 2, true);
      TNum(2); TNum(3); TOp('^', 4, false); TOp('^', 4, false);
      TOp('*', 3, true); TNum(1024); TOp('/', 3, true); TOp('+', 2, true)])
    |> should equal (
      sprintf "%A" (
        Plus(V(3), Div(Mult(V(7), Pow(Minus(V(1), V(5)), Pow(V(2), V(3)))), V(1024)))) )

[<Test>]
let testCalculate01() =
  calculate (Plus(V(1), V(2)))
    |> should equal 3

[<Test>]
let testCalculate02() =
  calculate (Plus(V(1), Mult(V(-2), V(3))))
    |> should equal -5

[<Test>]
let testCalculate03() =
  calculate (
    Plus(V(3), Div(Mult(V(7), Pow(Minus(V(1), V(5)), Pow(V(2), V(3)))), V(1024))))
    |> should equal 451

[<EntryPoint>]
let main argv =
  0
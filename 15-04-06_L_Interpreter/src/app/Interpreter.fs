module Interpreter

open System
open Dictionary
open IOProviders

type Expr =
  | Num of int
  | Var of string
  | BinOp of char * Expr * Expr

type Stmt = 
  | Read   of string
  | Write  of Expr
  | Assign of string * Expr
  | Seq    of Stmt * Stmt
  | If     of Expr * Stmt * Stmt
  | While  of Expr * Stmt

type Error = ErrNo | ErrDivByZero | ErrOverflow | ErrIO | ErrUnknown of string

exception NotExpression of string
exception NotStatement of string
exception SyntaxError of string

let rec parseExpr toks =
  let isLetter c =
    (('a' <= c) && (c <= 'z')) || (('A' <= c) && (c <= 'Z'))
  let isDigit c =
    (('0' <= c) && (c <= '9'))
  let isValidVarName (s : string) =
    (isLetter (s.[0])) &&
    (String.forall (fun c -> (isLetter c) || (isDigit c)) s) &&
    not (s = "read" || s = "write" || s = "if" || s = "while")
  match toks with
  | [] -> invalidArg "toks" "The input stream was empty!"
  | str::rest ->
    match str with
    | str when isValidVarName(str) ->
      Var(str), rest
    | "+" | "-" | "*" | "/" | "%" | "^" ->
      let (exprL, rest') = parseExpr rest
      let (exprR, rest'') = parseExpr rest'
      BinOp(str.[0], exprL, exprR), rest''
    | _ ->
      match Int32.TryParse(str) with
      | true, n -> Num(n), rest
      | _ -> raise (NotExpression(str))

let rec parseStmt toks =
  match toks with
  | [] -> invalidArg "toks" "The input stream was empty!"
  | str::rest ->
    match str with
    | "read" ->
      let (expr, rest') = parseExpr rest
      match expr with
      | Var(v) -> Read(v), rest'
      | _ -> raise (SyntaxError("Variable name exprected."))
    | "write" ->
      let (expr, rest') = parseExpr rest
      Write(expr), rest'
    | ":=" ->
      let (exprL, rest') = parseExpr rest
      let (exprR, rest'') = parseExpr rest'
      match exprL with
      | Var(v) -> Assign(v, exprR), rest''
      | _ -> raise (SyntaxError("Variable name exprected."))
    | "if" ->
      let (expr, rest') = parseExpr rest
      let (stmtT, rest'') = parseStmt rest'
      let (stmtF, rest''') = parseStmt rest''
      If(expr, stmtT, stmtF), rest'''
    | "while" ->
      let (expr, rest') = parseExpr rest
      let (stmt, rest'') = parseStmt rest'
      While(expr, stmt), rest''
    | ";" ->
      let (stmt1, rest') = parseStmt rest
      let (stmt2, rest'') = parseStmt rest'
      Seq(stmt1, stmt2), rest''
    | _ ->
      raise (NotStatement(str))

let rec calcExpr expr (scope : Dict<string, int>) =
  match expr with
  | Num(n) -> n
  | BinOp(c, exprL, exprR) ->
    let op =
      match c with
      | '+' -> ( + )
      | '-' -> ( - )
      | '*' -> ( * )
      | '/' -> ( / )
      | '%' -> ( % )
      | '^' -> pown
      | _ -> raise (NotExpression("Unknown operator in tree!"))
    op (calcExpr exprL scope) (calcExpr exprR scope)
  | Var(v) ->
    match scope.TryLookup v with
    | None -> raise (SyntaxError("Variable not defined!"))
    | Some n -> n

let execute (inP : IInputProvider) (outP : IOutputProvider) (stmt : Stmt) =
  let scope = new Dict<string, int>()
  let rec exec stmt =
    match stmt with
    | Read(v) ->
      scope.Store(v, inP.Read())
    | Write(expr) ->
      outP.Write(calcExpr expr scope)
    | Assign(v, expr) ->
      scope.Store (v, calcExpr expr scope)
    | Seq(st1, st2) ->
      exec st1
      exec st2
    | If(expr, stT, stF) ->
      if (calcExpr expr scope) <> 0 then exec stT else exec stF
    | While(expr, st) ->
      while (calcExpr expr scope) <> 0 do exec st
  try
    exec stmt
    ErrNo
  with
  | :? OverflowException -> ErrOverflow
  | :? DivideByZeroException -> ErrDivByZero
  | :? IOException' -> ErrIO
  | exc -> ErrUnknown(exc.Message)

let interpret (code : string) (inP : IInputProvider) (outP : IOutputProvider) =
  code.Split(' ', '\n') |> Array.toList |> parseStmt |> fst |> (execute inP outP)
// LanguagePlus.fs

module Language



open System
open System.IO
open System.Text.RegularExpressions


// ************************  Scanner Functions  **************************

// Tokens returned by the scanner have this type
type Token =
    | AddOp
    | SubOp
    | MulOp
    | DivOp
    | IntConst of int
    | Identifier of string
    | LeftParen
    | RightParen
    | LeftBrace
    | RightBrace
    | VarKeyword
    | IfKeyword
    | ThenKeyword
    | ElseKeyword
    | WhileKeyword
    | DoKeyword
    | ReturnKeyword
    | InputKeyword
    | OutputKeyword
    | FunctionKeyword
    | Assign
    | Semicolon
    | Comma
    | EOF

exception ScannerError

let idPattern = @"^[a-zA-Z][a-zA-Z0-9_]*"
let numPattern = @"^\d+"
let idRgx = Regex idPattern
let numRgx = Regex numPattern


// scans a string containing a statement and returns a list of tokens
let rec scanner str =
    if str = "" then
        [EOF]
    else
        let c = str.[0]
        let r = str.Substring 1
        if Char.IsWhiteSpace c then
            scanner r
        elif Char.IsLetter c then
            scanIdentifier str
        elif Char.IsDigit c then
            scanNumber str
        else
            match c with
            | '+' -> [AddOp] @ scanner r
            | '-' -> [SubOp] @ scanner r
            | '*' -> [MulOp] @ scanner r
            | '/' -> [DivOp] @ scanner r
            | '(' -> [LeftParen] @ scanner r
            | ')' -> [RightParen] @ scanner r
            | '{' -> [LeftBrace] @ scanner r
            | '}' -> [RightBrace] @ scanner r
            | '=' -> [Assign] @ scanner r
            | ';' -> [Semicolon] @ scanner r
            | ',' -> [Comma] @ scanner r
            | _   -> 
                     printfn "\nLexical error: %A unexpected\n" c
                     raise ScannerError
and
    scanIdentifier str =
         let m = idRgx.Match str
         if m.Success then
             let id = m.Value
             let tail = str.Substring m.Length
             if id = "var" then
                 [VarKeyword] @ scanner tail
             elif id = "return" then
                 [ReturnKeyword] @ scanner tail
             elif id = "if" then
                 [IfKeyword] @ scanner tail
             elif id = "then" then
                 [ThenKeyword] @ scanner tail
             elif id = "else" then
                 [ElseKeyword] @ scanner tail
             elif id = "while" then
                 [WhileKeyword] @ scanner tail
             elif id = "do" then
                 [DoKeyword] @ scanner tail
             elif id = "input" then
                 [InputKeyword] @ scanner tail
             elif id = "output" then
                 [OutputKeyword] @ scanner tail
             elif id = "function" then
                 [FunctionKeyword] @ scanner tail
             else
                 [ Identifier ( id ) ] @ scanner tail
         else
             printfn "\nLexical error: identifier regex failed\n"
             raise ScannerError
and
    scanNumber str =
         let m = numRgx.Match str
         if m.Success then
             let num = m.Value
             let len = m.Length
             [ IntConst ( Int32.Parse num ) ] @ scanner ( str.Substring len )
         else
             printfn "\nLexical error: number regex failed\n"
             raise ScannerError


// ******************** parser functions ******************************

exception ParseError

// The Expression type represents an expression as a tree
type Expression =
    | BinaryOpExp of Token * Expression * Expression
    | UnaryOpExp of Token * Expression
    | IntConstExp of int
    | IdentifierExp of string
    | InputExp
    | FuncCall of string * Expression list

// The Statement type represents one statement
type Statement =
    | Declaration of string list
    | Function of string * string list * Statement
    | Assignment of string * Expression
    | Output of Expression
    | If of Expression * Statement * Statement
    | While of Expression * Statement
    | Block of Statement list
    | Return of Expression


// Prints a fully-parenthesized version of an Expression value
let PrintExp exp =
    let rec PrintExp0 exp =
        let PrintToken t =
            match t with
            | AddOp -> printf "+"
            | SubOp -> printf "-"
            | MulOp -> printf "*"
            | DivOp -> printf "/"
            | IntConst(n) -> printf "%d" n
            | Identifier(name) -> printf "%s" name
            | _ -> printf " ?? "
        match exp with
        | BinaryOpExp(t,left,right) ->
                    printf "("
                    PrintExp0 left
                    PrintToken t
                    PrintExp0 right
                    printf ")"
        | UnaryOpExp(t,opnd) ->
                    printf "("
                    PrintToken t
                    PrintExp0 opnd
                    printf ")"
        | IntConstExp(n) -> printf "%d" n
        | IdentifierExp(name) -> printf "%s" name
        | InputExp -> printf "input"
        | FuncCall(name,args) ->
                    printf "%s(" name
                    PrintArgs args
                    printf ")"
    and    PrintArgs args =
        match args with
        | [] -> ()
        | [arg] -> PrintExp0 arg
        |  arg :: more ->
                   PrintExp0 arg
                   printf ", "
                   PrintArgs more
    PrintExp0 exp

let indentation = ref 0

// Prints a statement ... a statement is indented
// by a number of spaces proportional to indentation

let PrintIndent () =
    List.map (fun _  -> printf " ") [0 .. 3 * !indentation] |> ignore
 
let rec PrintStmt stmt =
    let rec PrintIDList vl =
        match vl with
        | [] -> ()
        | [v] -> printf "%s" v
        | v :: tail ->
                 printf "%s, " v
                 PrintIDList tail
    PrintIndent ()
    match stmt with
    | Assignment(lhs,rhs) ->
                   printf "%s = " lhs
                   PrintExp rhs
                   printfn ";"
    | Declaration(varlist) ->
                   printf "var "
                   PrintIDList varlist
                   printfn ";"
    | Function(name,parms,body) ->
                   printf "function %s(" name
                   PrintIDList parms
                   printfn ")"
                   incr indentation
                   PrintStmt body
                   decr indentation
    | Output(rhs) ->
                   printf "output = "
                   PrintExp rhs
                   printfn ";"
    | If(exp,thenpart,elsepart) ->
                   printf "if "
                   PrintExp exp
                   printfn " then"
                   incr indentation
                   PrintStmt thenpart
                   decr indentation
                   PrintIndent ()
                   printfn "else"
                   incr indentation
                   PrintStmt elsepart
                   decr indentation
    | While(exp,body) ->
                   printf "while "
                   PrintExp exp
                   printfn " do"
                   incr indentation
                   PrintStmt body
                   decr indentation
    | Return(exp) ->
                   printf "return "
                   PrintExp exp
                   printfn ";"
    | Block(stlst) ->
                   printfn "{"
                   incr indentation
                   PrintStmtList stlst
                   decr indentation
                   PrintIndent ()
                   printfn "}"


and
    // Prints a statement list
    PrintStmtList sl =
        List.map PrintStmt sl  |>  ignore


(*
   The nine functions defined here parse a list of tokens using
   this grammar:

   StmtList ::= { Stmt }

   Stmt     ::=  var IdList ';'
              |  Identifier '=' Exp ';'
              |  output = Exp ';'
              |  if Exp then Stmt else Stmt
              |  while Exp do Stmt
              |  return Exp ';'
              |  '{' StmtList '}'
              |  function Identifier '(' Formals ')' Stmt

   Formals  ::=  [ IdList ]

   IdList   ::=  Identifier {',' Identifier}

   Actuals  ::=  [ ExpList ]

   ExpList  ::=  Exp  {',' Exp}

   Exp      ::=  Term  { ('+'|'-') Term }

   Term     ::=  Factor  { ('*'|'/') Factor }

   Factor   ::=  ('+'|'-') Factor
              |  IntConst
              |  Identifier
              |  '(' Exp ')'
              |  Identifier '(' Actuals ')'
              |  input

   The argument for function ParseStmt is a list where elements have type
   Token;

   The argument for functions ParseExp, ParseTerm and ParseFactor is
   a list where elements have type Token.
   The result for each function is a tuple (tree,rest) where
   * tree is a value of type Expression which has been constructed by
     parsing one or more tokens from the front of the argument list, and
   * rest is a list of the remaining tokens in the argument which are
     unparsed.
*)


// Matches one token against the head of the tokens list
// and returns the remainder.
let MatchOne tok tokens =
    match tokens with
    | [] ->
         printfn "\nParse error: expected %A but hit end of input" tok
         raise ParseError
    | first :: rest ->
        if first <> tok then
           printfn "\nParse error: expected %A but found %A" tok first
           raise ParseError
        else
           rest

let rec ParseExp s =
        // ParseMoreTerms matches 0 or more occurrences of a plus or minus
        // operator followed by a term.  Its arguments are:
        // * leftExp -- the Expression which appears on the left, and
        // * s1      -- the list of tokens to parse.
        // The result is a tuple (tree,rest) where tree is an Expression
        // which represents everything parsed by this function, combined with
        // the leftExp argument, and rest is the remaining unparsed tokens.
        let rec ParseMoreTerms leftExp s1 =
            let t1 = List.head s1
            if (t1 = AddOp) || (t1 = SubOp) then
                let (next,s2) = ParseTerm (List.tail s1)
                let newExp = BinaryOpExp(t1,leftExp,next)
                ParseMoreTerms newExp s2
            else
                (leftExp,s1)
        let (left,rest) = ParseTerm s
        ParseMoreTerms left rest
and
      ParseTerm s =
        // ParseMoreFactors is similar to ParseMoreTerms above.
        let rec ParseMoreFactors leftExp s1 =
            let t1 = List.head s1
            if (t1 = MulOp) || (t1 = DivOp) then
                let (next,s2) = ParseTerm (List.tail s1)
                let newExp = BinaryOpExp(t1,leftExp,next)
                ParseMoreFactors newExp s2
            else
                (leftExp,s1)
        let (left,rest) = ParseFactor s
        ParseMoreFactors left rest
and
     ParseFactor s =
        let rec ParseArgs s =
            if List.head s = RightParen then
                ([],s)
            else
                let (exp1,rest1) = ParseExp s
                let rec ParseMoreArgs ss =
                    match ss with
                    | RightParen :: rest  ->  ([], rest)
                    | Comma :: rest ->
                                 let (exp2,rest2) = ParseExp rest
                                 let (lst3,rest3) = ParseMoreArgs rest2
                                 (exp2 :: lst3, rest3)
                    | x :: xx ->
                                 printfn "Parse error: %A found in argument list" x
                                 raise ParseError
                    | [] ->
                                 printfn "Parse error: unexpected end of input in argument list"
                                 raise ParseError
                let (exps,rr) = ParseMoreArgs rest1
                (exp1::exps, rr)
        match s with
        | IntConst(n) :: r -> ( IntConstExp(n), r )
        | Identifier(name) :: LeftParen :: r ->
                        let (args,rest1) = ParseArgs r
                        (FuncCall(name,args), rest1)
        | Identifier(name) :: r -> ( IdentifierExp(name), r )
        | InputKeyword :: r -> ( InputExp, r )
        | LeftParen :: r ->
                        let (tree,rest) = ParseExp r
                        let first = List.head rest
                        if first <> RightParen then
                            printfn "Parse error: ) expected but %A found" first
                            raise ParseError
                        (tree, List.tail rest)
        | AddOp :: r ->
                        let (tree,rest) = ParseFactor r
                        ( UnaryOpExp(AddOp,tree), rest )
        | SubOp :: r ->
                        let (tree,rest) = ParseFactor r
                        ( UnaryOpExp(SubOp,tree), rest )
        | x :: xx ->
                        printfn "Parse error: unexpected %A in expression" x
                        raise ParseError
        | [] ->
                        printfn "Parse error: unexpected end of input"
                        raise ParseError


let rec ParseStmt s =
    let rec getnames x =
        match x with
        | Identifier(name) :: Comma :: r ->
              let (nn,rest2) = getnames r
              ((name :: nn), rest2)
        | Identifier(name) :: r -> ([name],r)
        | _ -> raise ParseError
    match s with
    | VarKeyword :: rest ->
        let (tree1,rest1) = getnames rest
        (Declaration(tree1),(MatchOne Semicolon rest1))
    | Identifier(name) :: Assign :: rhs ->
        let (tree,rest) = ParseExp rhs
        (Assignment(name,tree), (MatchOne Semicolon rest))
    | OutputKeyword :: Assign :: rhs ->
        let (tree,rest) = ParseExp rhs
        (Output(tree), (MatchOne Semicolon rest))
    | ReturnKeyword :: rhs ->
        let (tree,rest) = ParseExp rhs
        (Return(tree), (MatchOne Semicolon rest))
    | IfKeyword :: after ->
        let (tree1,rest1) = ParseExp after
        let thenpart = MatchOne ThenKeyword rest1
        let (tree2,rest2) = ParseStmt thenpart
        let elsepart = MatchOne ElseKeyword rest2
        let (tree3,rest3) = ParseStmt elsepart
        (If(tree1,tree2,tree3), rest3)
    | WhileKeyword :: after ->
        let (tree1,rest1) = ParseExp after
        let body = MatchOne DoKeyword rest1
        let (tree2,rest2) = ParseStmt body
        (While(tree1,tree2), rest2)
    | LeftBrace :: after ->
        let (sl,rest) = ParseStmtList after
        (Block(sl), (MatchOne RightBrace rest))
    | FunctionKeyword :: Identifier(name) :: LeftParen :: after ->
        let (formals,rest1) = getnames after
        let (body,rest2) = ParseStmt (MatchOne RightParen rest1)
        (Function(name,formals,body), rest2)
    | x :: xx ->
           printfn "\nParse error: unexpected %A in input when new statement expected" x
           raise ParseError
    | [] ->
           printfn "\nParse error: unexpected end of input"
           raise ParseError

and
        ParseStmtList s =
    match s with
    | [] -> ([],[])
    | EOF :: _ -> ([], [])
    | RightBrace :: rest -> ([],s)
    | _ ->
          let (stmt1,rest1) = ParseStmt s
          let (list2,rest2) = ParseStmtList rest1
          ((stmt1::list2), rest2)


// Reads a program from a file and returns a list of statements
let ReadProgram filename =
    let text = File.ReadAllText(filename)
    let tokens = scanner text
    let (list,rest) = ParseStmtList tokens
    if rest <> [] then
        printfn "\nWarning: input contains unread tokens"
    list

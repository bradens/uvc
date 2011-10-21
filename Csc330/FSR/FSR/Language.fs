// Language.fs

module Language

// The functions and types defined in Language.fs provide the start of a package
// suitable for parsing a simple programming language.
//
// The datatype Statement represents a statement in a programming language.
// At the moment, there are only two statement types supported:
// 1. a declaration statement, and
// 2. an assignment statement.
//
// A declaration is written in the input stream as
//     var idlist ';'
// where an idlist is a sequence of identifiers separate by commas.
// An assignment is written as
//     id '=' expression ';'
// where the LHS is an identifier which must have been previously declared,
// and an expression is as defined below.
//
// Each statement must be written on a (single) line by itself.
//
// The datatype Expression represents an expression in the programming languages.
// Expressions may contain:
//    + - * /           i.e. infix operators
//    + -               i.e. prefix operators
//    identifiers
//    integer constants
// and may use parentheses to enclose subexpressions.
//
// System.Char and System.String provide some functions used in the scanner functions.
// See
// http://msdn.microsoft.com/en-us/library/system.text.regularexpressions.regex%28VS.100%29.aspx
// for documentation on regular expressions in .NET

open System
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
    | VarKeyword
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
            | '=' -> [Assign] @ scanner r
            | ';' -> [Semicolon] @ scanner r
            | ',' -> [Comma] @ scanner r
            | _   -> raise ScannerError
and
    scanIdentifier str =
         let m = idRgx.Match str
         if m.Success then
             let id = m.Value
             let tail = str.Substring m.Length
             if id = "var" then
                 [VarKeyword] @ scanner tail
             else
                 [ Identifier ( id ) ] @ scanner tail
         else
             raise ScannerError
and
    scanNumber str =
         let m = numRgx.Match str
         if m.Success then
             let num = m.Value
             let len = m.Length
             [ IntConst ( Int32.Parse num ) ] @ scanner ( str.Substring len )
         else
             raise ScannerError


// ******************** parser functions ******************************

exception ParseError

// The Expression type represents an expression as a tree
type Expression =
    | BinaryOpExp of Token * Expression * Expression
    | UnaryOpExp of Token * Expression
    | IntConstExp of int
    | IdentifierExp of string

// The Statement type represents one statement
type Statement =
    | Declaration of string list
    | Assignment of string * Expression

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
    PrintExp0 exp

// Prints a statement
let PrintStmt stmt =
    let rec PrintVarList vl =
        match vl with
        | [] -> printfn ""
        | [v] -> printfn "%s ;" v
        | v :: tail ->
                 printf "%s, " v
                 PrintVarList tail
    match stmt with
    | Assignment(lhs,rhs) ->
                   printf "%s = " lhs
                   PrintExp rhs
                   printfn ";"
    | Declaration(varlist) ->
                   printf "var "
                   PrintVarList varlist

// Prints a statement list
let PrintStmtList sl =
    printfn ""
    let _ = List.map PrintStmt sl
    printfn ""


(*
   The six functions defined here parse a list of tokens using
   this grammar:

   StmtList ::= { Stmt }

   Stmt     ::=  var IdList ';'
              |  Identifier '=' Exp ';'

   IdList   ::=  Identifier {',' Identifier}

   Exp      ::=  Term  { ('+'|'-') Term }

   Term     ::=  Factor  { ('*'|'/') Factor }

   Factor   ::=  ('+'|'-') Factor
              |  IntConst
              |  Identifier
              |  '(' Exp ')'

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
        match s with
        | IntConst(n) :: r -> ( IntConstExp(n), r )
        | Identifier(name) :: r -> ( IdentifierExp(name), r )
        | LeftParen :: r ->
                        let (tree,rest) = ParseExp r
                        if List.head rest <> RightParen then
                            raise ParseError
                        (tree, List.tail rest)
        | AddOp :: r ->
                        let (tree,rest) = ParseFactor r
                        ( UnaryOpExp(AddOp,tree), rest )
        | SubOp :: r ->
                        let (tree,rest) = ParseFactor r
                        ( UnaryOpExp(SubOp,tree), rest )
        | _ -> raise ParseError

let ParseStmt s =
    match s with
    | VarKeyword :: rest ->
        let rec getnames x =
            match x with
            | Identifier(name) :: Comma :: r -> name :: (getnames r)
            | Identifier(name) :: Semicolon :: _ -> [name]
            | _ -> raise ParseError
        Declaration(getnames rest)
    | Identifier(name) :: Assign :: rhs ->
        let (tree,rest) = ParseExp rhs
        if (List.head rest) = Semicolon then
            Assignment(name,tree)
        else
            raise ParseError
    | _ -> raise ParseError


// converts a string containing an arithmetic expression into a
// value of type Statement
let Convert str =
        ParseStmt (scanner str)

// Reads a sequence of statements from the console, one statement
// per line.
// Input ends when a blank line is read.
let rec ReadProgram () =
    let line = Console.ReadLine()
    let line = line.Trim()
    if line = "" then
        []
    else
        let stmt = Convert line
        stmt :: ReadProgram ()


// Some sequences of tokens to use for testing purposes
let testExp = [ Identifier("a"); MulOp; IntConst(99); AddOp; Identifier("b");
                SubOp; IntConst(37); DivOp; SubOp; Identifier("c"); EOF ]
let testStmt1 = [ VarKeyword; Identifier("a"); Comma, Identifier("b"); Semicolon; EOF ]
let testStmt2 = [ Identifier("a"); Assign; IntConst(37); Semicolon; EOF ]
let testStmt3 = [ Identifier("b"); Assign; Identifier("a"); MulOp; IntConst(37); Semicolon; EOF ]




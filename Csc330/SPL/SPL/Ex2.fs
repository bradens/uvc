//
// Braden Simpson V00685500
// SPL, CSC330 Assignment 3.
// Ex2
//
module Ex2
open System
open Language
// 
// Part 2
// LookUp
//
let rec LookUp vals id = 
    match vals with
    | [] -> None
    | hd::td -> if (fst (List.head vals) = id) then
                    Some (List.head vals)
                else
                    LookUp (List.tail vals) id
                     

//
// Part 2 
// Update
// 
let rec Update vals id value = 
    match vals with
    | [] -> vals
    | hd::tl -> if (fst (List.head vals) = id) then
                    [(id, value)] @ (List.tail vals)
                else
                    [(List.head vals)] @ Update vals.Tail id value


// 
// Part 2
// Eval
// Evaluates a expression tree by parsing the two sides recursively and 
// then calculating the integer value.
// @returns Option type.
//
let Eval vals exp = 
    let rec EvalTree expTree =
        let EvalUT tok left =
            match tok with
            | SubOp -> Option.get left * -1
            | _ -> 0

        let EvalBT tok left right =
            match tok with
            | AddOp -> Option.get left + Option.get right
            | SubOp -> Option.get left - Option.get right
            | MulOp -> Option.get left * Option.get right
            | DivOp -> Option.get left / Option.get right
            | _ -> 0

        match expTree with 
        | BinaryOpExp(t,left,right) ->
            let leftEval = EvalTree(left)
            let rightEval = EvalTree(right)
            if (leftEval <> None && rightEval <> None) then
                Some(EvalBT t leftEval rightEval)
            else
                None
        | UnaryOpExp(t,opnd) -> 
            let evalExp = EvalTree(opnd)
            if (evalExp <> None) then
                Some(EvalUT t evalExp)
            else
                None
        | IntConstExp(n) -> Some(n) 
        | InputExp -> 
            printfn "Enter a number: "
            let x = Console.ReadLine()
            Some(int32 x) 
        | IdentifierExp(name) -> 
            if (LookUp vals name).IsSome then 
                Some(snd (LookUp vals name).Value)
            else 
                printfn "Error: identifier %s is unbound" name
                None
    EvalTree exp

let Add values a integer = 
    (a, integer)::values 

//
// Part 2 
// SingleStep
//
let rec SingleStep values s = 
    let rec addVars vals vars = 
        match vars with 
        | [] -> values
        | [v] -> Add vals v 0
        | hd::tl -> addVars (Add vals hd 0) tl

    match s with 
    | Assignment(lhs, rhs) -> 
                    let id = LookUp values lhs
                    if (Option.isSome id) then
                        Update values (lhs) (Eval values rhs).Value
                    else 
                        printfn "Error: name %s is undeclared" lhs
                        values  
    | Declaration(varlist) -> 
                    match varlist with
                    | [] -> values
                    | hd::tl -> addVars values varlist
    | Output(exp) -> 
        let x = (Eval values exp)
        if (x.IsSome) then
            printfn "%A" x.Value
        else
            printfn "Err: Output is None."
        values
    | If(exp, stmt1, stmt2) -> 
        let cond = (Eval values exp).Value
        if (cond <> 0) then
            SingleStep values stmt1
        else
            SingleStep values stmt2
// 
// Part 2
// Interpret
//
let Interpret stmtList =
    let rec interp list stmts = 
        match stmts with
        | [] -> list 
        | hd::tl -> interp (SingleStep list hd) tl

    let rec printstmts list =
        match list with
        | [] -> []
        | hd::tl -> 
            printfn "%s = %d" (fst hd) (snd hd)
            printstmts tl

    
    printfn "Interpretation ended normally with"
    printstmts (interp [] stmtList)

let Run file = 
    let code = ReadProgram file
    Interpret code
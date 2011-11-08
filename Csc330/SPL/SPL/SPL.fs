//
// Braden Simpson V00685500
// SPL, CSC330 Assignment 3.
//
module SPL
open System
open Language

//
// Finds a variable in the bottommost scope, optional findDuplicates param
// to return if the scope has more than 1 var of same name.
//
let rec findInScope vals id findDuplicates = 
    match vals with
    | [] -> 
        None
    | hd::tl ->
        if (fst hd = id) then
            if (findDuplicates) then
                findInScope(List.tail vals) id false
            else
                Some (List.head vals)
        else if (fst hd = "***") then
            None
        else
            findInScope(List.tail vals) id findDuplicates
and
    LookUp vals id =
    match vals with
    | [] -> 
        None
    | ("***", 0)::tl -> 
        LookUp tl id
    | hd::tl ->
        if (fst (List.head vals) = id) then
                Some (List.head vals)
            else
                LookUp(List.tail vals) id

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
        | FuncCall(name, args) ->
            None
    EvalTree exp

let Add values a integer = 
    (a, integer)::values 

let rec removeScopeMarker = function
        | [] -> []
        | hd::tl -> 
            if ((fst hd) = "***") then
                tl
            else
                removeScopeMarker tl
and
    removeScopeMarkerFun fns = 
        match fns with 
        | [] -> []
        | (_, Function(name, parms, body))::tl ->
            if (name = "***") then
                tl
            else
                removeScopeMarkerFun tl

let addScopeMarkers list fns = 
    (("***", 0)::list, ("***", Function("***", ["***"], Declaration(["***"])))::fns)

//
// Part 2 
// SingleStep
//
let rec doLoop fns vals expr stmt1 =
    let cond = (Eval vals expr)
    if (cond.IsNone) then
        printfn "Error: Failed to compute while condition."
        (vals, fns)
    else
        if (cond.Value <> 0) then
            let vals = SingleStep vals fns stmt1
            doLoop fns (fst vals) expr stmt1
        else
            (vals, fns)
and
    SingleStep values fns s = 
    let rec addVars vals vars = 
        match vars with 
        | [] -> values
        | [v] -> Add vals v 0
        | hd::tl -> addVars (Add vals hd 0) tl

    let rec interp l fns stmts = 
            match stmts with
                | [] -> l
                | hd::tl ->
                    let vals = (SingleStep l fns hd)
                    interp (fst vals) (snd vals) tl

    match s with 
    | Assignment(lhs, rhs) -> 
                    let rec findUse vals id = 
                        match vals with
                        | [] -> Some(id)
                        | hd::tl -> 
                            if ((findInScope vals lhs true) <> None) then
                                printfn "Error: duplicate declaration for %s" (fst hd)
                                None
                            else
                                findUse tl id
                    if ((findUse values lhs) = None) then
                        (values, fns)
                    else
                        let id = LookUp values lhs
                        if (Option.isSome id) then
                            (Update values (lhs) (Eval values rhs).Value, fns)
                        else 
                            printfn "Error: name %s is undeclared" lhs
                            (values, fns)  
    | Declaration(varlist) -> 
                    match varlist with
                    | [] -> (values, fns)
                    | hd::tl -> (addVars values varlist, fns)
    | Output(exp) -> 
        let x = (Eval values exp)
        if (x.IsSome) then
            printfn "%A" x.Value
        else
            printfn "Err: Output is None."
        (values, fns)
    | If(exp, stmt1, stmt2) -> 
        let cond = (Eval values exp)
        if (cond.IsNone) then
            printfn "Error: Failed to compute if statement."
            (values, fns)
        else
            if (cond.Value <> 0) then
                SingleStep values fns stmt1
            else
                SingleStep values fns stmt2
    | While(exp, stmt) ->
        let cond = (Eval values exp)
        if (cond.IsNone) then
            printfn "Error: Failed to compute while condition."
            (values, fns)
        else
            doLoop fns values exp stmt // run the loop.
    | Block(stmtlist) -> 
        let lists = addScopeMarkers values fns
        let vList = fst lists
        let fList = snd lists
        let vList = interp vList fList stmtlist
        let vList = removeScopeMarker vList
        let fList = removeScopeMarkerFun fList
        (vList, fList)
    | Function(name, parms, body) ->
        let fns = (name, s)::fns
        (values, fns)
        
// 
// Part 2
// Interpret
//
let Interpret stmtList =
    let rec interp l fns stmts = 
        match stmts with
        | [] -> l
        | hd::tl -> 
            let vals = (SingleStep l fns hd)
            interp (fst vals) (snd vals) tl

    let rec printstmts list =
        match list with
        | [] -> []
        | hd::tl -> 
            printfn "%s = %d" (fst hd) (snd hd)
            printstmts tl

    
    printfn "Interpretation ended normally with"
    printstmts (interp [] [] stmtList)

let Run file = 
    let code = ReadProgram file
    Interpret code
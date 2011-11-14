//
// Braden Simpson V00685500
// SPL, CSC330 Assignment 3.
// Exercise 1 - 5
//
module Ex5
open System
open Language

exception InterpreterException

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
// Looks up a function by its string id in the function
// association list.
//
let rec LookUpFun fns id =
    match fns with 
    | [] -> None
    | hd::tl -> 
            if ((fst hd) = id) then
                Some(hd)
            else
                LookUpFun tl id 

let rec Update vals id value = 
    match vals with
    | [] -> vals
    | hd::tl -> 
            if (fst (List.head vals) = id) then
                [(id, value)] @ (List.tail vals)
            else
                [(List.head vals)] @ Update vals.Tail id value

let Add values a integer = 
    (a, integer)::values 

//
// Next two functions remove the scope markers "***" from both association lists.
// 
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
        | _ -> 
                []
//
// Adds a scope marker to both association lists
// returns (vals, fns)
// 
let addScopeMarkers list fns = 
    (("***", 0)::list, ("***", Function("***", ["***"], Declaration(["***"])))::fns)


//
// Constructs a list of arguments and their corresponding values given a Function Statement and a list of string args.
//
let rec constructArgumentsList stmt argsExprs vals fns =
    let args = []
    match stmt with 
    | Function(name, parms, body) -> 
            let argsNames = parms
            if ((List.length argsNames) <> (List.length argsExprs)) then
                printfn "Duplicate arguments."
                raise InterpreterException
                []
            else
                let args = List.collect (fun x ->
                                [(x, Option.get (Eval vals (argsExprs.Item (List.findIndex (fun p -> p = x) argsNames)) fns))]) argsNames
                args
    | _ -> 
            args
and
//
// Evaluates an expression given an assoc list for values and one for functions.
// returns Option (int)
//
    Eval vals expTree fns =
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
            let leftEval = Eval vals left fns
            let rightEval = Eval vals right fns
            if (leftEval <> None && rightEval <> None) then
                Some(EvalBT t leftEval rightEval)
            else
                None
    | UnaryOpExp(t,opnd) -> 
            let evalExp = Eval vals opnd fns
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
                raise InterpreterException
                None
    | FuncCall(name, args) ->
            let newLists = addScopeMarkers vals fns
            let vals = fst newLists
            let funStmt = (snd (LookUpFun fns name).Value)
            match funStmt with 
            | Function(name, parms, body) -> 
                    let argsList = constructArgumentsList funStmt args vals fns
                    let rec isDupe vals1 toAdd =
                        match vals1 with
                            | [] -> false
                            | hd::tl ->
                                    if ((fst hd) = "***") then
                                        false
                                    else if ((fst hd) = (fst toAdd)) then
                                        true
                                    else
                                        isDupe tl toAdd 
                    let rec addArguments argslist vals2 =
                        match argslist with 
                            | [] -> vals2
                            | hd::tl -> 
                                    if ((isDupe vals2 hd)) then
                                        printfn "Error, two args with same name"
                                        raise InterpreterException
                                        []
                                    else
                                        let vals2 = Add vals2 (fst hd) (snd hd)
                                        addArguments tl vals2
                    let vals = addArguments argsList vals
                    let scopeMarked = addScopeMarkers vals fns
                    let vals = (fst scopeMarked)
                    let fns = (snd scopeMarked)
                    let vals = Add vals "rv" 0
                    let l = SingleStep vals fns body
                    let vals = (fst l)
                    let fns = (snd l)
                    let returnValue = LookUp vals "rv"
                    let vals = removeScopeMarker vals
                    let fns = removeScopeMarkerFun fns 
                    Some(snd (returnValue).Value)
            | _ -> 
                    None
        
and
//
// Function to run a loop over a statement body
//
    doLoop fns vals expr stmt1 =
    let cond = (Eval vals expr fns)
    if (cond.IsNone) then
        printfn "Error: Failed to compute while condition."
        raise InterpreterException
        (vals, fns)
    else
        if (cond.Value <> 0) then
            let vals = SingleStep vals fns stmt1
            doLoop fns (fst vals) expr stmt1
        else
            (vals, fns)
and
// 
// Function that invokes the interpreter.
//
    interp l fns stmts = 
        match stmts with
            | [] -> l
            | hd::tl ->
                    let vals = (SingleStep l fns hd)
                    interp (fst vals) (snd vals) tl
and
//
// Does a Single Step into a statement, given a list of values and functions.
// returns (values:(string, int), fns:(string, statement))
//
    SingleStep values fns s = 
    let rec addVars vals vars = 
        match vars with 
        | [] -> values
        | [v] -> Add vals v 0
        | hd::tl -> addVars (Add vals hd 0) tl

    match s with 
    | Assignment(lhs, rhs) -> 
            let rec findUse vals id = 
                match vals with
                | [] -> Some(id)
                | hd::tl -> 
                    if ((findInScope vals lhs true) <> None) then
                        printfn "Error: duplicate declaration for %s" (fst hd)
                        raise InterpreterException
                        None
                    else
                        findUse tl id
            if ((findUse values lhs) = None) then
                (values, fns)
            else
                let id = LookUp values lhs
                if (Option.isSome id) then
                    (Update values (lhs) (Eval values rhs fns).Value, fns)
                else 
                    printfn "Error: name %s is undeclared" lhs
                    raise InterpreterException
                    (values, fns)  
    | Declaration(varlist) -> 
            match varlist with
            | [] -> (values, fns)
            | hd::tl -> (addVars values varlist, fns)
    | Output(exp) -> 
            let x = (Eval values exp fns)
            if (x.IsSome) then
                printfn "%A" x.Value
            else
                printfn "Err: Output is None."
                raise InterpreterException
            (values, fns)
    | If(exp, stmt1, stmt2) -> 
            let cond = (Eval values exp fns)
            if (cond.IsNone) then
                printfn "Error: Failed to compute if statement."
                raise InterpreterException
                (values, fns)
            else
                if (cond.Value <> 0) then
                    SingleStep values fns stmt1
                else
                    SingleStep values fns stmt2
    | While(exp, stmt) ->
            let cond = (Eval values exp fns)
            if (cond.IsNone) then
                printfn "Error: Failed to compute while condition."
                raise InterpreterException
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
    | Return(exp) ->
            let ev = (Eval values exp fns)
            let values = Update values "rv" ev.Value
            (values, fns) 
        
// 
// Invoke the interpreter on a statement list
// 
let Interpret stmtList =
    try
        let rec printstmts list =
            match list with
            | [] -> []
            | hd::tl -> 
                    printfn "%s = %d" (fst hd) (snd hd)
                    printstmts tl

    
        printfn "Interpretation ended normally with"
        printstmts (interp [] [] stmtList)
    with
        interpreterexception ->
                []

//
// Runs the interpreter on a given file
//
let Run file = 
    try
        let code = ReadProgram file
        Interpret code
    with
        filenotfoundexception -> 
                printfn "Error: File %s Not Found." file
                []

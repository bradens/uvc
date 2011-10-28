﻿module Part2
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
        | IdentifierExp(name) -> 
            if (LookUp vals name).IsSome then 
                Some(snd (LookUp vals name).Value)
            else 
                printfn "Error: identifier %s is unbound" name
                None
    EvalTree exp
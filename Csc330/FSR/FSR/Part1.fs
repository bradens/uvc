module Part1
open System

////////////////////////// Exercise 1 ////////////////////////////////////
//
// Does the recursive work for part 1. exercise 1.
// recurses all rotations of the inList and appends
// them to the appendedList.
//
let rec findRotations inList appendedList length = 
    if length = 0 then
        appendedList
    else
        let newList = List.append (List.tail inList) ([List.head inList])
        findRotations newList (newList :: appendedList) (length-1)

//
// Part 1. Exercise 1.
// inList : the input list to test rotations.
// returns : a list of rotations of the inList.
// 
let AllRotations inList =
   let len = List.length inList
   findRotations inList [] len

////////////////////////// Exercise 2 ////////////////////////////////////
//
// dist will place an element (e) in every possible spot in 
// the given sequence (Lst)
//
let dist e lst =
    let rec aux pre post = 
        seq {
            match post with
            | [] -> yield (lst @ [e])
            | h::t -> yield (List.rev pre @ [e] @ post)
                      yield! aux (h::pre) t 
        }
    aux [] lst

// 
// permute gives all the permutations of a given list
//
let rec permute = function 
    | [] -> Seq.singleton []
    | h::t -> Seq.collect (dist h) (permute t)
 
let AllPerms inList = 
    Seq.toList (permute inList)

////////////////////////// Exercise 3 ////////////////////////////////////

let rec findOddEvens inList evenList oddList = 
    match inList with 
        | [] -> (evenList, oddList)
        | hd::tail -> if ((List.head inList) % 2 = 0) then 
                            findOddEvens (List.tail inList) ((List.head inList)::evenList) oddList
                      else
                            findOddEvens (List.tail inList) evenList ((List.head inList)::oddList)

let OddEven inList = 
    findOddEvens inList [] []

////////////////////////// Exercise 3 ////////////////////////////////////
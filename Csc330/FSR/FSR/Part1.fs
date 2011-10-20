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
// ex. [1; 2; 3;]
// dist will place an element (e) in every possible spot in 
// the given sequence
//
let dist e L =
    let rec aux pre post = 
        seq {
            match post with
            | [] -> yield (L @ [e])
            | h::t -> yield (List.rev pre @ [e] @ post)
                      yield! aux (h::pre) t 
        }
    aux [] L

// 
// permute gives all the permutations of a given list
//
let rec permute = function 
    | [] -> Seq.singleton []
    | h::t -> Seq.collect (dist h) (permute t)
 
let AllPerms inList = 
    Seq.toList (permute inList)

////////////////////////// Exercise 3 ////////////////////////////////////


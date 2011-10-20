module Part1
open System

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
module Main

open System

let testList = [1..4]
let testList2 = [2..5]

let main =
    printfn "Running FSR..."
    let a = Part1.AllRotations testList
    printfn "Part1, exercise %i\n%A\nExercise %i done.\n" 1 a 1
    let b = Part1.AllPerms testList
    printfn "Part1, exercise %i\n%A\nExercise %i done.\n" 2 b 2
    let c = Part1.OddEven testList
    printfn "Part1, exercise %i\n%A\nExercise %i done.\n" 3 c 3
    let d = Part1.Merge testList testList2
    printfn "Part1, exercise %i\n%A\nExercise %i done.\n" 4 d 4
    Console.ReadKey()

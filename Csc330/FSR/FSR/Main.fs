module Main

open System

let testList = [1..4]
let testList2 = [2..5]
let testList3 = [2;3;1;5;7;12;44;13;77;24;9;55]

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
    let e = Part1.MergeSort testList3
    printfn "Part1, exercise %i\n%A\nExercise %i done.\n" 5 e 5
    Console.ReadKey()

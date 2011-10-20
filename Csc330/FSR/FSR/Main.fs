module Main

open System

let testList = [1..4]

let main =
    printfn "Running FSR..."
    let a = Part1.AllRotations testList
    printfn "Part1, exercise %i\n%A\nExercise %i done.\n" 1 a 1
    let b = Part1.AllPerms testList
    printfn "Part1, exercise %i\n%A\nExercise %i done.\n" 2 b 2
    let c = Part1.OddEven testList
    printfn "Part1, exercise %i\n%A\nExercise %i done.\n" 3 c 3
    Console.ReadKey()

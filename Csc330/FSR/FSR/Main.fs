module Main

open System

let testList = [1..5]

let main =
    Console.WriteLine("Running FSR...")
    Console.WriteLine("Part1, excercise 1")
    let a = Part1.AllRotations testList
    printfn "%A" a
    Console.WriteLine("Excercise 1 done.\n")
    Console.WriteLine("Part1, excercise 2")
    let b = Part1.AllPerms testList
    printfn "%A" b
    Console.WriteLine("Excercise 2 done.\n")
    Console.WriteLine("Part1, excercise 3")
    let c = Part1.OddEven testList
    printfn "%A" c
    Console.WriteLine("Excercise 3 done.\n")
    Console.ReadKey()

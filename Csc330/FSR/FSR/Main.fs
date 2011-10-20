module Main

open System

let testList = [1;2;3;4;5;6;7;8;9;10;]
//let testList = [1;2;3;]

let main =
    Console.WriteLine("Running FSR...");
    Console.WriteLine("Part1, excercise 1")
    let a = Part1.AllRotations testList
    printfn "%A" a
    Console.WriteLine("Excercise 1 done.");
    Console.ReadKey()

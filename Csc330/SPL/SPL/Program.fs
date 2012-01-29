// Learn more about F# at http://fsharp.net

open System
open Ex5
open Language


let main = 
    printfn "Enter a text file: "
    let x = Console.ReadLine()
    Run x |> ignore
    printfn "-----------------------------------------------------------------------------------\nDone %s\n" x

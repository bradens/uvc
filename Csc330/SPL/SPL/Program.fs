// Learn more about F# at http://fsharp.net

open System
open SPL
open Language


let main = 
    Run "t3.txt" |> ignore
    Console.ReadKey()
    printfn "Running Main"
    
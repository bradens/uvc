// Learn more about F# at http://fsharp.net

open System
open Ex4
open Language


let main = 
    Run "t1.txt" |> ignore
    Console.ReadKey()
    printfn "Running Main"
    
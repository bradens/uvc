module Main

open System
open Language

let testList = [1..4]
let testList2 = [2..5]
let testList3 = [2;3;1;5;7;12;44;13;77;24;9;55]
let vals = [ ("Num", 37); ("x", 42); ("y", -1); ("asdf", 11); ]
let vals2 = [ ("1", 1); ("2", 2); ("3", 3); ("4", 4); ("5", 5);]
let exp1 = [ Identifier("x"); MulOp; IntConst(10); AddOp;
        Identifier("y"); EOF ]
let exp2 = [ Identifier("x"); MulOp; Identifier("foo"); EOF ] 
let (expTree1,_) = ParseExp exp1
let (expTree2,_) = ParseExp exp2

let part1 = 
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

let part2 = 
    
    printfn "Testing LookUp.\n%A\n%A\n%A\n%A" (Part2.LookUp vals "Num") (Part2.LookUp vals "x") (Part2.LookUp vals "y") (Part2.LookUp vals "aasdf")
    printfn "Testing Update.\n%A\n%A\n" (Part2.Update vals "Num" 1) (Part2.Update vals "x" 50)
    printfn "%A" (Part2.Eval vals expTree1)
    printfn "%A" (Part2.Eval vals expTree2)

    let s11 = Convert "x = Num * 10 ;"
    let s12 = Convert "var a, x;"
    let s13 = Convert "foo = x*666;"
    printfn "%A\n%A\n%A\n" (Part2.SingleStep vals s11) (Part2.SingleStep vals s12) (Part2.SingleStep vals s13)


    printfn "Reading from input.."
    let code = ReadProgram()
    printfn "Interpreting..."
    Part2.Interpret code |> ignore

    
let main =
    part2 |> ignore
    Console.ReadKey()
module Part2

let rec LookUp vals id = 
    match vals with
    | [] -> []
    | hd::td -> 
        if ((fst (List.head vals)) = id) then
                    vals
                     



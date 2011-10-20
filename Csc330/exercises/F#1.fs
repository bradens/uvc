let rec mylength x = 
    match x with 
    | [] -> 0
    | hd :: tail -> 1 + mylength tail

module Q = struct
  type t = int * int
  (* (a, b) encode a/b*)
  (* invariant : a ∧ b = 1 et b > 0 et si a = 0 alors b = 1 *)
  (* NB: vu l'invariant, l'égalité syntaxique de OCaml (a, b)=(c, d)*)
  (*     encode en fait l'égalité sémantique de Q : a/b = c/d *)
  let print ((a, b): t) = Printf.printf "%d/%d" a b
  let dirty_print ((a, b): t) = Printf.printf "%.2f" ((float_of_int a) /. (float_of_int b))

end

type sad =
  {
    n  : int;                (* le nombre d’objets                  *)
    wi : int array;          (* le tableau des poids, de taille n   *)
    vi : int array;          (* le tableau des valeurs, de taille n *)
    p  : int                 (* le poids maximal du sac à dos, >=0  *)
}

let ex =
  {
    n  = 6;
    wi = [|6  ; 8 ; 10; 14; 2; 5|] ;
    vi = [|13 ; 16; 19; 24; 3; 5|] ;
    p  = 20;
  }


type masque = bool option array

type solution = bool array

type qsolution = Q.t array

(* Fonction d'affichage d'un tableau *)
let affiche_array_param (pp: 'a -> unit) (m: 'a array) =
  let n = Array.length m in
  Printf.printf "[|";
  for i = 0 to n-2 do
    pp m.(i) ; Printf.printf "; "
  done;
  if n > 0 then pp m.(n-1);
  Printf.printf "|]"

(* Affiche un masque *)
let affiche_masque (m: masque) =
  let print_masque_atom (bo: bool option) =
    match bo with
    | None        -> Printf.printf " "
    | Some(true)  -> Printf.printf "Y"
    | Some(false) -> Printf.printf "N"
  in
  affiche_array_param print_masque_atom m

(* Affichage d'une solution entière *)
let affiche_solution (m: solution) =
  let print_solution_atom (bo: bool) =
    match bo with
    | true  -> Printf.printf "1"
    | false -> Printf.printf "0"
  in
  affiche_array_param print_solution_atom m

(* Affichage d'une solution fractionnaire *)
let affiche_qsolution (m: qsolution) =
  affiche_array_param Q.print m

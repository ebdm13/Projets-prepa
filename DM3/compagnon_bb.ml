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

let ex_sol: solution = [|true; true; false; false; false; true|]
let ex_sol2: solution = [|true; true; true; true; false; true|]
let ex_masque: masque = [|Some(true); Some(true); None; None; Some(false); Some(true)|]
let ex_masque2: masque = [|Some(true); Some(true); Some(true); Some(true); Some(false); Some(true)|]

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

(* Test si l'instance e est valide *)
let est_sad_valide (e: sad) : bool =
  let res = ref true in
  res := (e.vi.(0) > 0 && e.wi.(0) > 0 && Array.length e.wi = Array.length e.vi) && (Array.length e.vi = e.n) && (e.wi.(0) <= e.p);
  let i = ref 1 in
  while !i < e.n && !res do
    res := e.vi.(!i) > 0 && e.wi.(!i) > 0 && (e.wi.(!i) <= e.p) && (e.vi.(!i-1) * e.wi.(!i) >= e.vi.(!i) * e.wi.(!i-1));
    incr i
  done;
  !res

(* Retourne la valeur associé à s pour e *)
let valeur_sol (e: sad) (s: solution) : int =
  let valeur = ref 0 in
  for i = 0 to e.n - 1 do
    if s.(i) then
      valeur := !valeur + e.vi.(i)
    else ()
  done;
  !valeur

(* Retourne le poids associé à s pour e *)
let poids_sol (e: sad) (s: solution) : int =
  let poids = ref 0 in
  for i = 0 to e.n - 1 do
    if s.(i) then
      poids := !poids + e.wi.(i)
    else ()
  done;
  !poids

(* Test si m est valide pour e *)
let est_masque_valide (e: sad) (m: masque) : bool =
  let res = ref true in
  let poids = ref 0 in
  let i = ref 0 in
  while !i < e.n && !res do
    if m.(!i) = Some(true) then begin
      poids := !poids + e.wi.(!i);
      res := !poids <= e.p
      end
    else ();
    incr i
  done;
  !res

(* Modifie s afin qu'il contienne le prochain tableau dans l'ordre d'énumération.
   Ici, l'ordre est l'énumration binaire classique (+1 à chaque fois).
   Les valeurs controlées par le masque sont inchangées.
*)
let next (m: masque) (s: solution) : unit =
  let n = Array.length s in
  let i = ref 0 in
  let continue = ref true in
  while !i < n && !continue do
    if m.(!i) = None then begin
      match s.(!i) with
      | true  -> s.(!i) <- false
      | false -> s.(!i) <- true; continue := false
    end
    else ();
    incr i
  done

(* Renvoie a puissance n *)
let rec pow (a: int) (n: int) : int =
  assert (n >= 0);
  if n = 0 then 1
  else begin
    if n mod 2 = 0 then pow (a * a) (n/2)
    else pow (a * a) (n/2) * a
  end

(* Renvoie une solution optimale pour l'instance e avec la contrainte m.
   None si le masque n'est pas valide
*)
let brute_force (e: sad) (m: masque) : (solution * int) option =
  if not (est_masque_valide e m) then None
  else (
    let valeur_opt = ref 0 in
    let sol_opt = Array.make e.n false in
    let free_val = ref e.n in

    for i = 0 to e.n - 1 do
      match m.(i) with
      | Some(x) -> sol_opt.(i) <- x; decr free_val
      | _ -> ()
    done;

    let current_sol = Array.copy sol_opt in
    valeur_opt := valeur_sol e sol_opt;
    for _ = 2 to pow 2 !free_val do
      next m current_sol;
      let v = valeur_sol e current_sol in
      if v > !valeur_opt && poids_sol e current_sol <= e.p then begin
        valeur_opt := v;
        Array.blit current_sol 0 sol_opt 0 e.n
        end
      else ()
    done;
    Some (sol_opt, !valeur_opt))

let prog_dyn_tab (e: sad) : int array array =
  let s = Array.init (e.n + 1) (fun _ -> Array.make (e.p + 1) 0) in
  for i = 1 to e.n do
    for j = 1 to e.p do
      if e.wi.(i-1) <= j then s.(i).(j) <- max s.(i-1).(j) (e.vi.(i-1) + s.(i-1).(j-e.wi.(i-1)))
      else s.(i).(j) <- s.(i-1).(j)
    done
  done;
  s

let prog_dyn (e: sad) : solution * int =
  let s = prog_dyn_tab e in
  let sol_opt = Array.make e.n false in
  let j = ref e.p in
  for i = e.n downto 1 do
    if s.(i).(!j) <> s.(i-1).(!j) then begin
      sol_opt.(i-1) <- true;
      j := !j - e.wi.(i-1)
      end
    else ()
  done;
  sol_opt, s.(e.n).(e.p)

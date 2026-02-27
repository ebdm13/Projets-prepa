let rec pgcd (a: int) (b: int) : int =
  let a,b = abs(a), abs(b) in
  let a, b = if a >= b then a,b else b, a in
  if b = 0 then a
  else pgcd b (a mod b)

module Q = struct
  type t = int * int
  (* (a, b) encode a/b*)
  (* invariant : a ∧ b = 1 et b > 0 et si a = 0 alors b = 1 *)
  (* NB: vu l'invariant, l'égalité syntaxique de OCaml (a, b)=(c, d)*)
  (*     encode en fait l'égalité sémantique de Q : a/b = c/d *)
  let print ((a, b): t) = Printf.printf "%d/%d" a b
  let dirty_print ((a, b): t) = Printf.printf "%.2f" ((float_of_int a) /. (float_of_int b))
  let (+/) (p, q: t) (p', q': t) : t =
    let a = p * q' + p' * q in
    let b = q*q' in
    let d = pgcd a b in
     a / d, b / d
  let (~/) (p, q: t): t = -p, q
  let (-/) (a: t) (b: t) : t = a +/ (~/ b)
  let ( */ ) (p, q: t) (p', q': t) : t =
    if p = 0 || p' = 0 then (0, 1)
    else begin
    let a = p * p' in
    let b = q * q' in
    let d = pgcd a b in
    a / d, b / d
    end
  let (//) (a: t) (p, q: t) : t = if p = 0 then raise Division_by_zero else a */ (q, p)
  let (</) (p, q: t) (p', q': t) : bool = p * q' < p' * q
  let (>/) (p, q: t) (p', q': t) : bool = p * q' > p' * q
  let (<=/) (p, q: t) (p', q': t) : bool = p * q' <= p' * q
  let (>=/) (p, q: t) (p', q': t) : bool = p * q' >= p' * q
  let is_int (p, q: t) : bool = q = 1
  let int_to_q (n: int) : t = (n, 1)
  let sgn (p, _: t) : int =
    if p = 0 then 0
    else if p > 0 then 1
    else -1
  let format (p, q: t) : t =
    let s = sgn (p,q) * (sgn (q, p)) in
    if s = 0 then (if q = 0 then raise Division_by_zero else 0, 1)
    else (let d = pgcd p q in
        s * abs(p) / d, abs(q) / d)
end

open Q

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

let masque_to_sol (e: sad) (m: masque) : solution =
  let sol = Array.make e.n false in
  for i = 0 to e.n - 1 do
    match m.(i) with
    | Some(x) -> sol.(i) <- x
    | _ -> ()
  done;
  sol

let glouton_n (e: sad) (m: masque) : (solution * int) option =
  let sol = masque_to_sol e m in
  let w = ref (poids_sol e sol) in
  let v = ref (valeur_sol e sol) in
  if !w > e.p then None
  else begin
    for i = 0 to e.n -1 do
      if !w + e.wi.(i) <= e.p && m.(i) = None then begin
        sol.(i) <- true;
        w := !w + e.wi.(i);
        v := !v + e.vi.(i)
        end
      else ()
    done;
    Some(sol, !v)
    end


let glouton_r (e: sad) (m: masque) : (qsolution * Q.t) option =
  let sol = Array.make e.n (0, 1) in
  let w = ref (0, 1) in
  let v = ref (0, 1) in
  for i = 0 to e.n - 1 do
    match m.(i) with
    | Some(true) -> sol.(i) <- (1, 1); w := !w +/ (e.wi.(i), 1); v := !v +/ (e.vi.(i), 1)
    | _ -> ()
  done;
  if !w >/ (e.p, 1) then None
  else begin
    let i = ref 0 in
    while !i < e.n && !w </ (e.p, 1) do
      if m.(!i) = None then begin
        if !w +/ (e.wi.(!i), 1) <=/ (e.p, 1) then (
          sol.(!i) <- (1, 1); w := !w +/ (e.wi.(!i), 1); v := !v +/ (e.vi.(!i), 1) )
        else (
          sol.(!i) <- ((e.p,1) -/ !w) // (e.wi.(!i), 1); w := (e.p, 1); v := !v +/ (sol.(!i) */ (e.vi.(!i), 1)))
      end
      else ();
      incr i
    done;
    Some(sol, !v)
  end

let impose_i (m: masque) (i: int) (b: bool) : masque =
  let m' = Array.copy m in m'.(i) <- Some(b); m'

let find_i_frac (s: qsolution) : int =
  let i = ref 0 in
  let n = Array.length s in
  while !i < n && snd s.(!i) = 1 do incr i done;
  !i

let branch_and_bound (e: sad) : solution * int =
  let best_value = ref 0 in
  let best_sol = Array.make e.n false in
  let todo = Queue.create () in
  Queue.push (Array.make e.n None) todo;
  while not (Queue.is_empty todo) do
    let m = Queue.pop todo in
    match glouton_r e m with
    | Some(s_q, v_q) when v_q >/ (!best_value, 1) -> begin
      match glouton_n e m with
      | Some(s_n, v_n) ->
        if v_n > !best_value then begin
          best_value := v_n;
          Array.blit s_n 0 best_sol 0 e.n
        end;
        if v_q >=/ (v_n + 1, 1) then begin
          let i_sep = find_i_frac s_q in
          Queue.push (impose_i m i_sep false) todo;
          Queue.push (impose_i m i_sep true) todo
        end
      | _ -> ()
      end
    | _ -> ()
  done;
  best_sol, !best_value

let compare_algo (n: int) (p: int) : unit =
  let e: sad = {
    n = n;
    p = p;
    wi = Array.init n (fun _ -> 1 + Random.int p);
    vi = Array.init n (fun _ -> Random.int (n*5));
  } in
  let i_t = Array.init n (fun i -> i) in
  Array.sort (fun i j -> e.vi.(j) * e.wi.(i) - e.vi.(i) * e.wi.(j)) i_t;
  let wi' = Array.init n (fun k -> e.wi.(i_t.(k))) in
  let vi' = Array.init n (fun k -> e.vi.(i_t.(k))) in
  Array.blit wi' 0 e.wi 0 n;
  Array.blit vi' 0 e.vi 0 n;

  let t_1 = Sys.time () in
  let s_1, v_1 = prog_dyn e in
  let t_2 = Sys.time () in
  let s_2, v_2 = branch_and_bound e in
  let t_3 = Sys.time () in

  Printf.printf "Temps (s): prog_dyn: %f, B&B: %f\n" (t_2 -. t_1) (t_3 -. t_2);
  Printf.printf "Valeur: prog_dyn: %d, B&B: %d\n" v_1 v_2;
  Printf.printf "Poids: prog_dyn: %d, B&B: %d\n" (poids_sol e s_1) (poids_sol e s_2);
  if s_1 = s_2 then
  print_string "Mêmes solutions\n"
  else print_string "Solutions différentes\n"

open Parser
open ARN

type valuation = (string * bool) list
type sat_result = valuation option

type litteral = Var of string | NotVar of string
type clause = litteral arn
type fnc = clause list

let is_litteral (f: formule) : bool =
	match f with
	| Var _ | Not(Var _) -> true
	| _ -> false

let rec is_clause (f: formule) : bool =
	match f with
	| Or(f1, f2) -> is_clause f1 && is_clause f2
	| _ -> is_litteral f

let rec is_fnc (f: formule) : bool =
	match f with
	| And(f1, f2) -> is_fnc f1 && is_fnc f2
	| _ -> is_clause f

(* convertie f en clause. Précondition: f est une clause*)
let clause_of_formule (f: formule) : clause =
	let rec clause_of_formule_insert (f: formule) (c: clause) : clause =
		match f with
		| Var x -> insertionARN (Var x) c
		| Not (Var x) -> insertionARN (NotVar x) c
		| Or(f1, f2) -> clause_of_formule_insert f1 (clause_of_formule_insert f2 c)
		| _ -> None
	in clause_of_formule_insert f None

(* convertie f en fnc. Précondition: f est sous fnc *)
let fnc_of_formule (f: formule) : fnc = 
	let rec fnc_of_formule_concat (f: formule) (f': fnc) : fnc = 
		match f with
		| And(c1, c2) -> fnc_of_formule_concat c1 (fnc_of_formule_concat c2 f')
		| c -> (clause_of_formule c)::f'
	in fnc_of_formule_concat f []

(* Retourne l'ensemble des variable de f sous forme d'arn *)
let var_arn (f: formule) : string arn =
	let rec insert_var_arn (f: formule) (t: string arn) : string arn =
		match f with
		| Top | Bot -> t
		| Var(q) -> insertionARN q t
		| Not(f') -> insert_var_arn f' t
		| And(f1, f2) | Or(f1, f2) -> insert_var_arn f1 (insert_var_arn f2 t)
	in insert_var_arn f None

(* transforme l'arn des variables en liste *)
let list_var_from_arn (t: 'a arn) : 'a list =
	let rec list_arn_aux (t: 'a arn) (l: 'a list) : 'a list = 
		match t with
		| None -> l
		| Some Feuille(a) -> a::l
		| Some Noeud(_, _, g, d) -> list_arn_aux (Some g) (list_arn_aux (Some d) l)
	in list_arn_aux t []

let rec print_list (l: valuation) : unit =
	match l with
	| [] -> ()
	| (x, b)::q -> print_string (x ^ "; ") ; print_list q

(* substitue x par v dans f et simplifie f.*)
let rec subst_fnc (x: string) (v: bool) (f: fnc) : fnc =
	match f, v with
	| [], _ -> []
	| c::f', true when recherche (Var x) c -> subst_fnc x v f'
	| c::f', true when recherche (NotVar x) c-> (deleteARN (NotVar x) c)::(subst_fnc x v f')
	| c::f', false when recherche (NotVar x) c -> subst_fnc x v f'
	| c::f', false when recherche (Var x) c-> (deleteARN (Var x) c)::(subst_fnc x v f')
	| c::f', _ -> c::(subst_fnc x v f')

(* Renvoie un couple (x, b) option avec x une variable unitaire, càd qui est l'unique variable d'une
clause et b sa valuation pour satisfaire la clause. None sinon *)
let rec unit_propagation (f: fnc) (v: string list) : ((string * bool) * (string list)) option =
	match f with
	| [] -> None
	| (Some (Feuille (Var x)))::_ -> Some ((x, true), List.filter (fun y -> y <> x) v)
	| (Some (Feuille (NotVar x)))::_ -> Some ((x, false), List.filter (fun y -> y <> x) v)
	| _::f' -> unit_propagation f' v

(* Renvoie si une clause de f est vide *)
let rec empty_clause (f: fnc) : bool = 
	match f with
  | [] -> false
  | None::f' -> true
  | _::f' -> empty_clause f'

let rec quine_fnc_aux (f: fnc) (v: string list) : sat_result =
	print_int (List.length v); print_newline ();
	match f with
	| [] -> Some []
	| _ when empty_clause f -> None
	| _ ->
		match unit_propagation f v with
		| Some ((x, b), v') ->
			(
			match quine_fnc_aux (subst_fnc x b f) v' with
			| None -> None
			| Some r -> Some ((x, b)::r)
			)
		| None ->
			match v with
			| [] -> None
			| x::q -> 
				match quine_fnc_aux (subst_fnc x false f) q with
				| Some r1 -> Some ((x, false)::r1)
				| None -> 
					print_string ("BT: " ^ x ^ "------------------------------------------\n"); 
					match quine_fnc_aux (subst_fnc x true f) q with
					| None -> None
					| Some r2 -> Some ((x, true)::r2)

let quine_fnc (f: formule) : sat_result =
	let f' = fnc_of_formule f in
	let v = list_var_from_arn (var_arn f) in
	quine_fnc_aux f' v

(* affiche seuleument les variable dont la valuation est true *)
let rec print_true (v: valuation) : unit = 
	match v with
	| [] -> ()
	| (x, true)::q -> print_string x; print_newline (); print_true q
	| (x, false)::q -> print_true q	

exception Not_enough_arguments of string

let main () =
	if Array.length Sys.argv <> 2 then
		raise (Not_enough_arguments "il faut un argument")
	else
		match Sys.argv.(1) with
		(* | "test" -> test () *)
		| file ->
			let f = from_file file in
			let res = quine_fnc f in
			print_string "Temps d'execution: "; print_float (Sys.time ()); print_string "s\n";
			match res with
			| None -> print_string "La formule n'est pas satisfiable\n"
			| Some v -> print_string "La formule est satisfiable en assignant 1 aux variables suivantes et 0 aux autres:\n";
				print_true v


let _ = main ()

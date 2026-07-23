open Parser
open ARN
open Dico

(* Supprime v dans l'arn de dico à la clée k *)
let del_arn (dico: ('k, 'v arn) dico) (k: 'k) (v: 'v) : ('k, 'v arn) dico =
	match get dico k with
	| None -> dico
	| Some (Some (Feuille v')) when v' = v -> del dico k
	| Some t -> set dico k (deleteARN v t)

(* Insère v dans l'arn de dico à la clée k *)
let set_arn (dico: ('k, 'v arn) dico) (k: 'k) (v: 'v) : ('k, 'v arn) dico =
	match get dico k with
	| None -> set dico k (Some (Feuille v))
	| Some t -> set dico k (insertionARN v t)

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

(* On stocke les variables avec un triplet de dictionaire.
le premier contentant toutes les variables est trié par nom de variable
le second contient les variables dont la valuation n'est pas certaine; trié par nombre d'occurence
(les valeurs sont des arn qui contiennent les variables de même occurence)
le dernier contient les variables dont la valuation est certaine: càd qui apparait sous
une seule forme (positive ou négatige) dans toute la formule.
(les valeurs sont des arn qui contiennent les variables de même occurence)
 *)
type all_var = (string, int*int) dico
type uncertain_var = (int, (string*bool) arn) dico
type certain_var = (int, (string*bool) arn) dico
type variables = all_var * uncertain_var * certain_var

(* Ajoute l dans d et met à jours le nombre d'occurence*)
let add_var (d: all_var) (l: litteral) : all_var =
	match l with
	| Var x -> (match get d x with
					   | None -> set d x (1, 0)
					   | Some (pos, neg) -> set d x (pos + 1, neg))
	| NotVar x -> match get d x with
					   		| None -> set d x (0, 1)
					   		| Some (pos, neg) -> set d x (pos, neg + 1)

(* Ajoute les variables de c dans d *)
let rec add_clause (c: clause) (d: all_var): all_var =
	match c with
	| None -> None
	| Some (Feuille l) -> add_var d l
	| Some (Noeud(_, _, c1, c2)) -> add_clause (Some c1) (add_clause (Some c2) d)

(* Retourne le dictionaire contenant toutes les variables en clée
et le nombre d'occurence positive / négative en clée *)
let find_var (f: fnc) : all_var = 
	let rec find_var_set (f: fnc) (d: all_var) : all_var =
		match f with
		| [] -> d
		| c::f' -> find_var_set f' (add_clause c d)
	in find_var_set f None

(* Génère les des autres dictionaires: certain / uncertain _var à partir de all_var et renvoie 
let triplet.
*)
let gen_var (d: all_var) : variables =
	let rec gen_var_aux (v: variables) : uncertain_var * certain_var = 
		let d1, d2, d3 = v in
		match d1 with
		| None -> None, None
		| Some (Feuille (x, (pos, 0))) -> d2, set_arn d3 pos (x, true)
		| Some (Feuille (x, (0, neg))) -> d2, set_arn d3 neg (x, false)
		| Some (Feuille (x, (pos, neg))) -> set_arn d2 (pos + neg) (x, pos >= neg), d3
		| Some (Noeud(_, _, v1, v2)) -> let d2', d3' = gen_var_aux ((Some v1), d2, d3) in
																		gen_var_aux ((Some v2), d2', d3')
	in let d2, d3 = gen_var_aux (d, None, None)
	in d, d2, d3

(* supprime une occurence de l dans vars*)
let update_var_from_litteral (l: litteral) (vars: variables) : variables =
	let d1, d2, d3 = vars in
	match l with
	| Var x -> begin
			 match get d1 x with
			 | None -> vars
			 | Some (1, 0) -> del d1 x, d2, del_arn d3 1 (x, true) 
			 | Some (1, neg) -> 
			 		set d1 x (0, neg), del_arn d2 (1+neg) (x, 1 = neg), set_arn d3 neg (x, false)
			 | Some (pos, 0) -> set d1 x (pos-1, 0), d2, set_arn (del_arn d3 pos (x, true)) (pos-1) (x, true)
			 | Some (pos, neg) -> 
			 		set d1 x (pos-1, neg), set_arn (del_arn d2 (pos+neg) (x, pos >= neg)) (pos+neg-1) (x, pos-1>= neg), d3
			 end
	| NotVar x -> match get d1 x with
				 | None -> vars
				 | Some (0, 1) -> del d1 x, d2, del_arn d3 1 (x, false)
				 | Some (pos, 1) -> set d1 x (pos, 0), del_arn d2 (1+pos) (x, true), set_arn d3 pos (x, true)
				 | Some (0, neg) -> set d1 x (0, neg-1), d2, set_arn (del_arn d3 neg (x, false)) (neg-1) (x, false)
				 | Some (pos, neg) ->
				 		set d1 x (pos, neg-1), set_arn (del_arn d2 (pos+neg) (x, pos >= neg)) (pos+neg-1) (x, pos >= neg-1), d3

(* supprime une occurence de tous les littéraux de c dans vars*)
let rec update_var_from_clause (c: clause) (vars: variables) : variables =
	match c with
	| None -> vars
	| Some (Feuille l) -> update_var_from_litteral l vars
	| Some (Noeud(_, _, c1, c2)) -> 
			update_var_from_clause (Some c1) (update_var_from_clause (Some c2) vars)

(* substitue x par v dans f, simplifie f et met à jour vars*)
let rec subst_fnc (x: string) (v: bool) (f: fnc) (vars: variables) : fnc * variables =
	match f, v with
	| [], _ -> [], vars
	| c::f', true when recherche (Var x) c -> subst_fnc x v f' (update_var_from_clause c vars)
	| c::f', true when recherche (NotVar x) c-> let f'', vars' = subst_fnc x v f' (update_var_from_litteral (NotVar x) vars) in
		(deleteARN (NotVar x) c)::f'', vars'
	| c::f', false when recherche (NotVar x) c -> subst_fnc x v f' (update_var_from_clause c vars)
	| c::f', false when recherche (Var x) c-> let f'', vars' = subst_fnc x v f' (update_var_from_litteral (Var x) vars) in
		(deleteARN (Var x) c)::f'', vars'
	| c::f', _ -> let f'', vars' = subst_fnc x v f' vars in c::f'', vars'

(* Renvoie la plus grande valeur de d *)
let rec max_dict_arn (dico: ('k, 'v arn) dico) : 'v option = 
	match dico with
	| None -> None
	| Some (Feuille (_, t)) -> max_arn t
	| Some (Noeud(_, _, _, d)) -> max_dict_arn (Some d)

(* Renvoie un couple (x, b) option avec x une variable unitaire, càd qui est l'unique variable d'une
clause et b sa valuation pour satisfaire la clause. None sinon *)
let rec unit_propagation (f: fnc) : (string * bool) option =
	match f with
	| [] -> None
	| (Some (Feuille (Var x)))::_ -> Some ((x, true))
	| (Some (Feuille (NotVar x)))::_ -> Some ((x, false))
	| _::f' -> unit_propagation f'

(* Renvoie si une clause de f est vide *)
let rec empty_clause (f: fnc) : bool = 
	match f with
  | [] -> false
  | None::f' -> true
  | _::f' -> empty_clause f'

let rec quine_fnc_aux (f: fnc) (vars: variables) : sat_result =
	let d1, d2, d3 = vars in
	match f with
	| [] -> Some []
	| _ when empty_clause f -> None
	| _ ->
	match unit_propagation f with
	| Some (x, b) ->
		(
		let f', vars' = subst_fnc x b f vars in
		match quine_fnc_aux f' vars' with
		| None -> None
		| Some r -> Some ((x, b)::r)
		)
	| None ->
			match max_dict_arn d3 with
			| Some (x, b) -> 
			(
			let f', vars' = subst_fnc x b f vars in
			match quine_fnc_aux f' vars' with
			| None -> None
			| Some r -> Some ((x, b)::r)
			)
			| None ->
				match max_dict_arn d2 with
				| None -> None
				| Some (x, v) -> 
					let f1, vars1 = subst_fnc x v f vars in
					match quine_fnc_aux f1 vars1 with
					| Some r1 -> Some ((x, v)::r1)
					| None -> 
						let f2, vars2 = subst_fnc x (not v) f vars in
						match quine_fnc_aux f2 vars2 with
						| None -> None
						| Some r2 -> Some ((x, not v)::r2)

let quine_fnc (f: formule) : sat_result = 
	if is_fnc f then
		let f' = fnc_of_formule f in
		let vars = gen_var (find_var f') in
		quine_fnc_aux f' vars
	else failwith "la formule n'est pas sous fnc"

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
			print_string "solvation begin\n";
			let t0 = Sys.time () in
			let res = quine_fnc f in
			let time = Sys.time () -. t0 in
			print_string "Temps d'execution: "; print_float time; print_string "s\n";
			match res with
			| None -> print_string "La formule n'est pas satisfiable\n"
			| Some v -> print_string "La formule est satisfiable en assignant 1 aux variables suivantes et 0 aux autres:\n";
				print_true v


let _ = main ()	
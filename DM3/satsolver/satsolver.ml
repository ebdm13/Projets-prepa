type formule =
	| Var of string
	| Top
	| Bot
	| And of formule * formule
	| Or of formule * formule
	| Not of formule

type couleur = Rouge | Noir
type 'a noeud_arn = Feuille of 'a | Noeud of couleur * 'a * 'a noeud_arn * 'a noeud_arn
type 'a arn = 'a noeud_arn option

let implique (f1, f2) = Or(Not f1, f2)
let equivalence (f1, f2) = And(implique (f1, f2), implique (f2, f1))

(*** PARSER ***)

exception Erreur_syntaxe
exception Fichier_invalide

(* Symboles:
	'T' -> true
	'F' -> false
	'&' -> And
	'|' -> Or
	'~' -> Not
	'>' -> implication
	'=' -> equivalence
 *)

(* Détermine si c correspond à un opérateur binaire logique *)
let is_binop (c: char) : bool = match c with 
	| '&' |  '|' |  '>' |  '='  -> true
	| _ -> false 

(* Priorité de l'opérateur c. Permet de déterminer
	comment interpréter une formule sans parenthèses.
	Par exemple, "x&y|z" sera interprété comme "(x&y)|z"
	car & est plus prioritaire que | *)
let priority (c: char) : int = match c with
	| '&' -> 4
	| '|' -> 3
	| '=' -> 2
	| '>' -> 1
	| _ -> raise Erreur_syntaxe (* c n'est pas un opérateur *)

(* indice de l'opérateur le moins prioritaire parmis ceux
   qui ne sont pas entre parenthèses entre s.[i] et s.[j] 
   inclus *)
 let find_op_surface (s: string) (i: int) (j: int) : int =
 	(* 
 	   Renvoie l'indice de l'opérateur le moins prioritaire entre
 	   i et j, sachant que res est l'indice du meilleur opérateur
 	   entre i et k-1.
 	   paren_lvl: niveau d'imbrication actuel des parenthèses *)
 	let rec find_op_paren (k:int) (res:int) (paren_lvl: int) : int  =
 		if k=j+1 then res else
 		if s.[k] = '(' then find_op_paren (k+1) res (paren_lvl+1)
 		else if s.[k] = ')' then find_op_paren (k+1) res (paren_lvl-1) 

 		(* Le caractère lu est pris si l'on est hors des parenthèses,
 		   que le caractère est bien un opérateur, et qu'il est moins
 		   prioritaire que le meilleur résultat jusqu'ici *)
 		else if paren_lvl = 0 
 			 && is_binop s.[k] 
 			 && (res = -1 || priority s.[k] < priority s.[res]) 
 			 then find_op_paren (k+1) k (paren_lvl)
 		else find_op_paren (k+1) res (paren_lvl)
 	in find_op_paren i (-1) 0;;

(* Renvoie une formule construite à partir de la chaîne s.
   Lève une exception Erreur_syntaxe si la chaîne ne représente pas une formule valide. *)
let parse (s: string) : formule =
	let n = String.length s in
	(* construit une formule à partir de s[i..j] *)
	let rec parse_aux (i: int) (j:int) =
		assert (0 <= i && i < n && 0 <= j && j < n && i <= j );
		if s.[i] = ' ' then parse_aux (i+1) j
		else if s.[j] = ' ' then parse_aux i (j-1)
		else let k = find_op_surface s i j in 
		if k = -1 then
			if s.[i] = '~' then 
				Not (parse_aux (i+1) j)
			else if s.[i] = '(' then
				begin 
					if (s.[j] != ')') then (print_int j; failwith "mauvais parenthésage") else
					parse_aux (i+1) (j-1)
				end
			else if (i = j && s.[i] = 'T') then Top
			else if (i = j && s.[i] = 'F') then Bot
			else Var(String.sub s i (j-i+1))

		else match s.[k] with
			| '&' -> And(parse_aux i (k-1), parse_aux (k+1) j)
			| '|' -> Or(parse_aux i (k-1), parse_aux (k+1) j)
			| '=' -> equivalence(parse_aux i (k-1), parse_aux (k+1) j)
			| '>' -> implique(parse_aux i (k-1), parse_aux (k+1) j)
			| _ -> raise Erreur_syntaxe
	in parse_aux 0 (String.length s -1)

(* Renvoie une formule construire à partir du contenu du fichier fn.
   Lève une exception Erreur_syntaxe si le contenu du fichier n'est pas une formule valide.
   Lève une exception Sys_error(message_erreur) si le nom du fichier n'est pas valide. *)
let from_file (filename: string) : formule = 
	(* concatène toutes les lignes de f en une seule chaîne *)
	let rec read_lines f = 
		try 
			let next_line = input_line f in
			let s = read_lines f in
			next_line ^ s
		with 
			| End_of_file -> ""
	in
	let f = open_in filename in 
	let s = read_lines f in
	parse s

let test_parse () =
	assert (parse "a | (b & ~c)" = Or(Var "a", And(Var "b", Not (Var "c"))));
	assert (parse "a | (b & (c > ~b)) | (a = F)" = Or(Var "a", Or(And(Var "b", implique (Var "c", Not(Var "b"))), equivalence (Var "a", Bot))));
	assert (parse "((a | b) & c) > d" = implique (And(Or(Var "a", Var "b"), Var "c"), Var "d"));
	try
    let _ = parse "a & (b | c" in
    assert false;
  	with
  	| Failure("mauvais parenthésage") -> assert true;
	
	(* try
    let _ = parse "a $ b" in
    assert false;
  	with
  	| Erreur_syntaxe -> assert true; *)
	print_string "Tests OK\n"

let test_from_file () =
	assert (from_file "tests/test1.txt" =
	Or (Not (Or (Var "a", And (Var "b", Not (Var "c")))),
 And
  (Or (And (Or (Not (Var "d"), Var "e"), Or (Not (Var "e"), Var "d")),
    And (Var "f", Not (Var "g"))),
  Or (Not (Var "h"), Or (Var "i", Var "j")))))

exception Not_enough_arguments of string

(* Renvoie le contenu de la première ligne du fichier file *)
let read_file (file: string) : string = 
	let ic = open_in file in
	let res = input_line ic in
	close_in ic; res

(* Compte le nombres de OR / AND / NOT dans une formule f *)
let rec compte_ops (f: formule) : int =
	match f with
	| Top | Bot | Var(_) -> 0
	| Not(f') -> 1 + compte_ops f'
	| And(f1, f2) | Or(f1, f2) -> 1 + compte_ops f1 + compte_ops f2

let test_compte_ops () = 
	assert (compte_ops (parse "x | (y & ~z)") = 3);
	assert (compte_ops (parse "~(x | (x & ~z) | y)") = 5)

(* Vérifie qu'une liste est strictement croissante *)
let rec is_sort (l: 'a list) : bool =
	match l with
	| [] | _::[] -> true
	| x::y::q -> x < y && is_sort (y::q)

(* Fusionne l1 et l2, deux listes strictement croissante en une nouvelle liste strictement croissante *)
let rec union (l1: 'a list) (l2: 'a list) : 'a list =
	match l1, l2 with
	| l, [] | [], l -> l 
	| x::q, y::p -> if x < y then x::(union q (y::p)) 
						else if x > y then y::(union (x::q) p) 
						else x::(union q p)

(* Retourne la liste des variables de f *)
let rec var_list (f: formule) : string list =
	match f with
	| Top | Bot -> []
	| Var(q) -> [q]
	| Not(f') -> var_list f'
	| And(f1, f2) | Or(f1, f2) -> union (var_list f1) (var_list f2)

let correctionARN (t: 'a noeud_arn) : 'a noeud_arn = 
	match t with
	| Noeud(Noir, z, Noeud(Rouge, y, Noeud(Rouge, x, a, b), c), d)
	| Noeud(Noir, z, Noeud(Rouge, x, a, Noeud(Rouge, y, b, c)), d) 
	| Noeud(Noir, x, a, Noeud(Rouge, z, Noeud(Rouge, y, b, c), d))
	| Noeud(Noir, x, a, Noeud(Rouge, y, b, Noeud(Rouge, z, c, d)))
	-> Noeud(Rouge, y, Noeud(Noir, x, a, b), Noeud(Noir, z, c, d))
	| _ -> t

let rec insertionARNrelax (x: 'a) (t: 'a noeud_arn ) : 'a noeud_arn =
	match t with
	| Feuille e when e < x -> Noeud(Rouge, e, Feuille e, Feuille x)
	| Feuille e | Noeud(_, e, _, _) when e = x -> t
	| Feuille e -> Noeud(Rouge, x, Feuille x, Feuille e)
 	| Noeud(c, e, g, d) when e < x -> correctionARN (Noeud(c, e, g, insertionARNrelax x d))
 	| Noeud(c, e, g, d) -> correctionARN (Noeud(c, e, insertionARNrelax x g, d))

let insertionARN (x: 'a) (t: 'a arn) : 'a arn =
	match t with
	| None -> Some (Feuille x)
	| Some t' -> let Noeud(c, e, g, d) = insertionARNrelax x t' in
	Some (Noeud(Noir, e, g, d))

let var_arn (f: formule) : string arn =
	let rec insert_var_arn (f: formule) (t: string arn) : string arn =
		match f with
		| Top | Bot -> t
		| Var(q) -> insertionARN q t
		| Not(f') -> insert_var_arn f' t
		| And(f1, f2) | Or(f1, f2) -> insert_var_arn f1 (insert_var_arn f2 t)
	in insert_var_arn f None

let list_var_from_arn (t: string arn) : string list =
	let rec list_arn_aux (t: string arn) (l: string list) : string list = 
		match t with
		| None -> l
		| Some Feuille(a) -> a::l
		| Some Noeud(_, _, t1, t2) -> list_arn_aux (Some t1) (list_arn_aux (Some t2) l)
	in list_arn_aux t []


let test () = 
	test_parse ();
	test_from_file ();
	test_compte_ops ();
	print_string "Tous les tests ont réussi\n"

let main () =
	if Array.length Sys.argv <> 2 then
		raise (Not_enough_arguments "il faut un argument")
	else
		match Sys.argv.(1) with
		| "test" -> test ()
		| file -> print_string (read_file file) ; print_newline ()

let _ = main ()
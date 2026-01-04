type vector = float array

type kd_tree =
  | Vide
  | Node  of int * vector * kd_tree * kd_tree


(* GÉNÉRATION JEU TEST *)
let genere_jeu_donnes (n: int) (d: int) : vector array =
  Array.init n (fun _ -> Array.init d (fun _ -> Random.float 1.))

(* OUTILS GRAPHIQUES *)

let yellow = Graphics.rgb 210 160 4
let red = Graphics.rgb 168 27 3
let green = Graphics.rgb 105 105 0
let cx = 1000
let cy = 1000
let to_x pt = (pt *. (float_of_int cx)) |> int_of_float
let to_y pt = (pt *. (float_of_int cy)) |> int_of_float

let rec draw_kd_tree_aux (t: kd_tree) swx swy nex ney =
  match t with
  | Vide -> ()
  | Node(dim, v_sep, g, d) ->
    begin
      let dx, dy = to_x v_sep.(0), to_y v_sep.(1) in
      if dim = 0 then
        begin
          Graphics.set_color Graphics.black;
          Graphics.moveto dx swy; Graphics.lineto dx ney;
          Graphics.set_color yellow;
          Graphics.fill_circle dx dy 5;
          draw_kd_tree_aux g swx swy dx ney;
          draw_kd_tree_aux d dx swy nex ney;
        end
      else
        begin
          Graphics.set_color Graphics.black;
          Graphics.moveto swx dy; Graphics.lineto nex dy;
          Graphics.set_color yellow;
          Graphics.fill_circle dx dy 5;
          draw_kd_tree_aux g swx swy nex dy;
          draw_kd_tree_aux d swx dy nex ney;
        end;
    end

let draw_kd_tree (t: kd_tree) =
  (* Hyp : fenêtre graphique de taille cx * cy ouverte
     dessine l'arbre t
  *)
  draw_kd_tree_aux t 0 0 cx cy

let f_cmp (i: int) (x: vector) (y: vector) : bool =
  x.(i) <= y.(i)

(* cmp est un préordre sur t. 
   La fonction renvoie q et modifie t de façon à ce que pour tous éléments t.(i) de t plus petit
   que t.(p) selon cmp, i <= q et pour tous élémets t.(j) de t plus grand que t.(p)
   selon cmp, j > q. De plus t.(q) = t.(p).
 *)
let partition (t: 'a array) (d: int) (f: int) (p: int) (cmp: 'a -> 'a -> bool) : int =
  let pivot = t.(p) in
  t.(p) <- t.(d);
  t.(d) <- pivot;
  let a = ref (d+1) in
  let b = ref f in
  while !a <= !b do
    if cmp t.(!a) pivot then
      incr a
    else
      begin
        let temp = t.(!a) in
        t.(!a) <- t.(!b);
        t.(!b) <- temp;
        decr b
      end
  done;
  let temp = t.(d) in
  t.(d) <- t.(!a-1);
  t.(!a-1) <- temp;
  !a-1

(* cmp est un préordre sur t. 
La fonction renvoie un élément v de t entre d et f tel qu'il existe un tris de t entre d et f
dans lequel v est de rang r
 *)
let rec selection_rapide (t: 'a array) (d: int) (f: int) (r: int) (cmp: 'a -> 'a -> bool) : 'a =
  assert(d <= f);
  let p = Random.int_in_range ~min: d ~max: f in
  let q = partition t d f p cmp in
  if r = q then
    t.(r)
  else if r < q then selection_rapide t d (q-1) r cmp
  else selection_rapide t (q+1) f r cmp

(* Construit un l'arbre k-dimensionel à partir des vecteurs de data entre l'indice d et f
en partant de la direction i *)
let rec cree_arbre_kd_aux (data: vector array) (k: int) (i: int) (d: int) (f: int) : kd_tree =
  if d > f then Vide
  else
    begin
      let v = selection_rapide data d f ((f + d)/2) (f_cmp i) in
      let gauche = cree_arbre_kd_aux data k ((i+1) mod k) d ((f + d)/2 - 1) in
      let droite = cree_arbre_kd_aux data k ((i+1) mod k) ((f + d)/2 + 1) f in
      Node(i, v, gauche, droite)
    end

(* Renvoie l'arbre k-dimensionel construit à partir de data *)
let rec cree_arbre_kd (data: vector array) (k: int) =
  cree_arbre_kd_aux data k 0 0 (Array.length data - 1)

(* Renvoie la distance au carré entre y et x *)
let sq_distance (x: vector) (y: vector) : float =
  let n = Array.length x in
  let d = ref 0. in
  for i = 0 to n - 1 do
    d := !d +. (x.(i) -. y.(i)) *. (x.(i) -. y.(i))
  done;
  !d

(* Renvoie le plus proche voisins de x dans l si il y en a un, None sinon. *)
let nearest_among (l: vector option list) (x: vector) : vector option =
  let rec nearest_among_aux (l: vector option list) (x: vector) (m: vector option) (d_min: float): vector option =
    match l with
    | [] -> m
    | None::q -> nearest_among_aux q x m d_min
    | Some v::q -> if m = None || sq_distance v x < d_min then nearest_among_aux q x (Some v) (sq_distance v x)
                   else nearest_among_aux q x m d_min
  in nearest_among_aux l x None 0.

(* Renvoie le plus proche voisin de x dans t si il y en a un, None sinon.*)
let rec pp_voisin (t: kd_tree) (x: vector) : vector option =
  match t with
  | Vide -> None
  | Node(i, v, g, d) ->
    let not_sure (t': kd_tree) (c: vector option) : vector option=
      let c' = pp_voisin t' x in
      nearest_among (c::c'::(Some v)::[]) x
    in
    let check_from (t': kd_tree) : vector option =
      let c = pp_voisin t' x in
      let other_direction = if t' = g then d else g in
      match c with
      | None -> not_sure other_direction c
      | Some c_vect when sq_distance c_vect x <= (x.(i) -. v.(i)) *. (x.(i) -. v.(i)) -> c
      | _ -> not_sure other_direction c
    in
    if x.(i) <= v.(i) then check_from g
    else check_from d

(* Insert (c, d_c) dans l où l est une de (v, d_v) où d_v est la distance (au carré)
de v.
Présuposé: l est triée par ordre décroissant de distances (dv) et de taille r <= n.
La liste renvoyée est aussi de taille <= n.
*)
let insert (c: vector) (d_c: float) (l: (vector*float) list) (r: int) (n: int) : (vector*float) list * int =
  assert (r <= n);
  let rec insert_aux (c: vector) (d_c: float) (succ: (vector*float) list) (pred: (vector*float) list) : (vector*float) list =
    match succ with
    | (v, d_v)::q when d_c < d_v -> insert_aux c d_c q ((v, d_v)::pred)
    | _ -> List.rev_append pred ((c, d_c)::succ)
  in
  match insert_aux c d_c l [] with
  | [] -> [], 0
  | (v, d_v)::q when r = n -> q, n
  | l' -> l', r+1

(* Renvoie les (au plus) n vecteurs les plus proches de x dans l (triés par ordre décroissant de distance) *)
let k_nearest_among (l: vector list) (x: vector) (n: int) : vector list =
  let rec k_nearest_among_aux (l: vector list) (x: vector) (kn: (vector*float) list) (r: int) : (vector*float) list =
    assert (r <= n);
    match l with
    | [] -> kn
    | c::q -> let kn', r' = insert c (sq_distance c x) kn r n in k_nearest_among_aux q x kn' r'
  in
  List.map fst (k_nearest_among_aux l x [] 0)

(* Renvoie les n plus proches voisins de x dans t *)
let rec pp_voisins (t: kd_tree) (x: vector) (n: int) : vector list =
  match t with
  | Vide -> []
  | Node(i, v, g, d) ->
    let not_sure (t': kd_tree) (lc: vector list) (p: int) : vector list=
      let lc' = pp_voisins t' x p in
       k_nearest_among (v::(lc@lc')) x p
    in
    let check_from (t': kd_tree) : vector list =
      let lc = pp_voisins t' x n in
      let other_direction = if t' = g then d else g in
      let dist_from_sep = (x.(i) -. v.(i)) *. (x.(i) -. v.(i)) in
      let s, ns = List.partition (fun c -> sq_distance x c <= dist_from_sep) lc in
      let p = n - (List.length s) in
      s@(not_sure other_direction ns p)
    in
    if x.(i) <= v.(i) then check_from g
    else check_from d

(* 
  Soit x un vecteur quelconque.
  Renvoie une liste de n couples (vecteur, distance à x) dont la distance à x minimise celles des couples de s, ns, l et l'.
  On suppose que s, ns, l et l' sont triées par ordre croissant de distance à x.
*)
let concat_4_until (s: (vector*float) list) (ns: (vector*float) list) (l: (vector*float) list) (l': (vector*float) list) (n: int) : (vector*float) list =
  let rec concat_4_until_aux (s: (vector*float) list) (ns: (vector*float) list) (l: (vector*float) list) (l': (vector*float) list) (acc: (vector*float) list) (r: int) : (vector*float) list =
  if r = n then List.rev acc
  else
    match s, ns, l, l' with
    | [], [], [], [] -> List.rev acc
    | c::q, [], [], [] | [], c::q, [], [] | [], [], c::q, [] | [], [], [], c::q -> concat_4_until_aux q [] [] [] (c::acc) (r+1)
    | c::q, c'::q', [], [] | [], c::q, c'::q', [] | [], [], c::q, c'::q'
    | c::q, [], [], c'::q' | [], c::q, [], c'::q' | c::q, [], c'::q', []
    -> if snd c < snd c' then
        concat_4_until_aux q (c'::q') [] [] (c::acc) (r+1)
       else
        concat_4_until_aux q' (c::q) [] [] (c'::acc) (r+1)
    | (v1, d1)::q1, (v2, d2)::q2, (v3, d3)::q3, [] | (v1, d1)::q1, (v2, d2)::q2, [], (v3, d3)::q3 | (v1, d1)::q1, [], (v2, d2)::q2, (v3, d3)::q3 | [], (v1, d1)::q1, (v2, d2)::q2, (v3, d3)::q3
      -> if d1 < d2 && d1 < d3 then
            concat_4_until_aux q1 ((v2, d2)::q2) ((v3, d3)::q3) [] ((v1, d1)::acc) (r+1)
         else if d2 < d1 && d2 < d3 then
            concat_4_until_aux ((v1, d1)::q1) q2 ((v3, d3)::q3) [] ((v2, d2)::acc) (r+1)
         else
            concat_4_until_aux ((v1, d1)::q1) ((v2, d2)::q2) q3 [] ((v3, d3)::acc) (r+1)
    | (v1, d1)::q1, (v2, d2)::q2, (v3, d3)::q3, (v4, d4)::q4 ->
        if d1 < d2 && d1 < d3 && d1 < d4 then
          concat_4_until_aux q1 ns l l' ((v1, d1)::acc) (r+1)
        else if d2 < d1 && d2 < d3 && d2 < d4 then
          concat_4_until_aux s q2 l l' ((v2, d2)::acc) (r+1)
        else if d3 < d1 && d3 < d2 && d3 < d4 then
          concat_4_until_aux s ns q3 l' ((v3, d3)::acc) (r+1)
        else
          concat_4_until_aux s ns l q4 ((v4, d4)::acc) (r+1)
  in
  concat_4_until_aux s ns l l' [] 0


(* Renvoie la liste (vecteur, distance à x) des n plus proches voisins de x dans t *)
let rec pp_voisins_2_aux (t: kd_tree) (x: vector) (n: int) : (vector*float) list =
  if n = 0 then []
  else
  match t with
  | Vide -> []
  | Node(i, v, g, d) ->
    let check_from (t': kd_tree) : (vector*float) list =
      let lc = pp_voisins_2_aux t' x n in
      let other_direction = if t' = g then d else g in
      let dist_from_sep = (x.(i) -. v.(i)) *. (x.(i) -. v.(i)) in
      let s, ns = List.partition (fun (c, d_c) -> d_c <= dist_from_sep) lc in
      let p = n - (List.length s) in
      let lc' = pp_voisins_2_aux other_direction x p in
      concat_4_until s ns lc' [v, (sq_distance v x)] n
    in
    if x.(i) <= v.(i) then check_from g
    else check_from d

(* Renvoie la liste des n plus proches voisins de x dans t *)
let pp_voisins_2 (t: kd_tree) (x: vector) (n: int) : vector list =
  List.map fst (pp_voisins_2_aux t x n)

(* Affiche v *)
let draw_one_nn (v: vector) : unit =
  let dx', dy' = to_x v.(0), to_y v.(1) in
  Graphics.fill_circle dx' dy' 5

(* Affiche le plus proche voisin de x en utilisant pp_voisin *)
let draw_nn (t: kd_tree) (x: vector) : unit =
  let dx, dy = to_x x.(0), to_y x.(1) in
  Graphics.set_color red;
  draw_one_nn x;
  match pp_voisin t x with
  | None -> ()
  | Some v ->
    begin
      let dx', dy' = to_x v.(0), to_y v.(1) in
      Graphics.set_color green;
      draw_one_nn v;
      Graphics.set_color Graphics.black;
      let dist: int = (dx - dx') * (dx - dx') + (dy - dy') * (dy - dy') |> float_of_int |> Float.sqrt |> int_of_float in
      Graphics.draw_circle dx dy dist
    end

(* Affiche les n plus proches voisins de x en utilisant pp_voisins_2 *)
let draw_knn (t: kd_tree) (x: vector) (n: int) : unit =
  Graphics.set_color red;
  draw_one_nn x;
  Graphics.set_color green;
  let l = pp_voisins_2 t x n in
  let v_max = List.hd (List.rev l) in
  List.iter (fun v -> draw_one_nn v) l;

  let dx, dy = to_x x.(0), to_y x.(1) in
  let dx', dy' = to_x v_max.(0), to_y v_max.(1) in
  Graphics.set_color Graphics.black;
  let dist: int = (dx - dx') * (dx - dx') + (dy - dy') * (dy - dy') |> float_of_int |> Float.sqrt |> int_of_float in
  Graphics.draw_circle dx dy dist

(* Compare deux vecteurs v1 et v2 par rapport à leur distance à x *)
let compare_vect_from (x: vector) (v1: vector) (v2: vector): int =
  let d1 = sq_distance x v1 in
  let d2 = sq_distance x v2 in
  if d1 = d2 then 0
  else if d1 > d2 then 1
  else -1

(* Compare les performances des fonctions v1: pp_voisins et v2: pp_voisins_2 
deux implémentation de l'algorithme des n plus proches voisins
pour nb_vector vecteurs de dimension k
*)
let compare_ppv (nb_vector: int) (n: int) (k: int) : unit =
  let t = genere_jeu_donnes nb_vector k in
  let kd_tree = cree_arbre_kd t k in
  let x = Array.init k (fun _ -> Random.float 1.) in
  let t1 = Sys.time () in
  let res1 = pp_voisins kd_tree x n in
  let t2 = Sys.time () in
  let res2 = pp_voisins_2 kd_tree x n in
  let t3 = Sys.time () in

  let res1 = List.sort (compare_vect_from x) res1 in
  assert(res1 = res2);

   Printf.printf "Temps écoulé v1: %fs\n" (t2 -. t1);
   Printf.printf "Temps écoulé v2: %fs\n" (t3 -. t2)


let main_exemple () =
  let nb_points = 1000 in
  let _t = genere_jeu_donnes nb_points 2 in
  let n = 100 in
  let kd_tree = cree_arbre_kd _t 2 in
  Graphics.open_graph " 1000x1000";
  draw_kd_tree kd_tree;
  (* draw_nn kd_tree (Array.init 2 (fun _ -> Random.float 1.)); *)
  draw_knn kd_tree (Array.init 2 (fun _ -> Random.float 1.)) n;
  let _ = Graphics.wait_next_event [Key_pressed] in
  Graphics.close_graph ()

let () = main_exemple ()
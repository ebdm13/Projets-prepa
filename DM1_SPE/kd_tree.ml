type vector = float array

type kd_tree =
  | Vide
  | Node  of int * vector * kd_tree * kd_tree


(* GÉNÉRATION JEU TEST *)
let genere_jeu_donnes (n: int) : vector array =
  Array.init n (fun _ -> Array.init 2 (fun _ -> Random.float 1.))

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

let rec selection_rapide (t: 'a array) (d: int) (f: int) (r: int) (cmp: 'a -> 'a -> bool) : 'a =
  assert(d <= f);
  let p = Random.int_in_range ~min: d ~max: f in
  let q = partition t d f p cmp in
  if r = q then
    t.(r)
  else if r < q then selection_rapide t d (q-1) r cmp
  else selection_rapide t (q+1) f r cmp

let rec cree_arbre_kd_aux (data: vector array) (k: int) (i: int) (d: int) (f: int) : kd_tree =
  if d > f then Vide
  else
    begin
      let v = selection_rapide data d f ((f + d)/2) (f_cmp i) in
      let gauche = cree_arbre_kd_aux data k ((i+1) mod k) d ((f + d)/2 - 1) in
      let droite = cree_arbre_kd_aux data k ((i+1) mod k) ((f + d)/2 + 1) f in
      Node(i, v, gauche, droite)
    end

let rec cree_arbre_kd (data: vector array) (k: int) =
  cree_arbre_kd_aux data k 0 0 (Array.length data - 1)

let sq_distance (x: vector) (y: vector) : float =
  let n = Array.length x in
  let d = ref 0. in
  for i = 0 to n - 1 do
    d := !d +. (x.(i) -. y.(i)) *. (x.(i) -. y.(i))
  done;
  !d

let nearest_among (l: vector option list) (x: vector) : vector option =
  let rec nearest_among_aux (l: vector option list) (x: vector) (m: vector option) (d_min: float): vector option =
    match l with
    | [] -> m
    | None::q -> nearest_among_aux q x m d_min
    | Some v::q -> if m = None || sq_distance v x < d_min then nearest_among_aux q x (Some v) (sq_distance v x)
                   else nearest_among_aux q x m d_min
  in nearest_among_aux l x None 0.

(* Renvoie le plus proche voisin de x dans t*)
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

let test_insert () : unit = 
  let x = [| 0.; 0.; 0. |] in

  let v0 = [| 0.; 1.; 2. |] in
  let v1 = [| 2.; 5.; 4. |] in
  let v2 = [| 1.; 2.; 3. |] in

  let d0 = sq_distance x v0 in
  let d1 = sq_distance x v1 in
  let d2 = sq_distance x v2 in

 (* --- insertion 1 --- *)
  let l, r = insert v0 d0 [] 0 3 in
  assert (r = 1);
  assert (List.length l = 1);
  assert (snd (List.hd l) = d0);

  (* --- insertion 2 --- *)
  let l, r = insert v1 d1 l r 3 in
  assert (r = 2);
  assert (List.length l = 2);
  (* ordre décroissant : v1 (plus loin) doit être avant v0 *)
  let [(v_a, d_a); (v_b, d_b)] = l in
  assert (d_a >= d_b);
  assert (d_a = d1);
  assert (d_b = d0);

  (* --- insertion 3 --- *)
  let l, r = insert v2 d2 l r 3 in
  assert (r = 3);
  assert (List.length l = 3);
  (* tri décroissant attendu : v1 (plus loin), v2, v0 (plus proche) *)
  let [(v_a, d_a); (v_b, d_b); (v_c, d_c)] = l in
  assert (d_a >= d_b && d_b >= d_c);
  assert (abs_float (d_a -. d1) < 1e-9);
  assert (abs_float (d_b -. d2) < 1e-9);
  assert (abs_float (d_c -. d0) < 1e-9);

  (* --- insertion 4 (inutile) --- *)
  let l', r' = insert v1 d1 l r 3 in
  assert (r' = 3);
  assert (l' = l);
  (* toujours triée et identique *)
  let [(v_a, d_a); (v_b, d_b); (v_c, d_c)] = l' in
  assert (d_a >= d_b && d_b >= d_c);
  assert (abs_float (d_a -. d1) < 1e-9);
  assert (abs_float (d_b -. d2) < 1e-9);
  assert (abs_float (d_c -. d0) < 1e-9)

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

let draw_one_nn (v: vector) : unit =
  let dx', dy' = to_x v.(0), to_y v.(1) in
  Graphics.fill_circle dx' dy' 5

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

let draw_knn (t: kd_tree) (x: vector) (n: int) : unit = 
  Graphics.set_color red;
  draw_one_nn x;
  Graphics.set_color green;
  let l = pp_voisins t x n in
  let v_max = List.fold_left (fun acc v -> if sq_distance v x  > sq_distance acc x then v else acc) (List.hd l) l in
  List.iter (fun v -> draw_one_nn v) l;

  let dx, dy = to_x x.(0), to_y x.(1) in
  let dx', dy' = to_x v_max.(0), to_y v_max.(1) in
  Graphics.set_color Graphics.black;
  let dist: int = (dx - dx') * (dx - dx') + (dy - dy') * (dy - dy') |> float_of_int |> Float.sqrt |> int_of_float in
  Graphics.draw_circle dx dy dist

let main_exemple () =
  let nb_points = 1000 in
  let _t = genere_jeu_donnes nb_points in
  let n = 10 in
  let kd_tree = cree_arbre_kd _t 2 in      
  Graphics.open_graph " 1000x1000";
  draw_kd_tree kd_tree;
  (* draw_nn kd_tree (Array.init 2 (fun _ -> Random.float 1.)); *)
  draw_knn kd_tree (Array.init 2 (fun _ -> Random.float 1.)) n;
  let _ = Graphics.wait_next_event [Key_pressed] in
  Graphics.close_graph ()

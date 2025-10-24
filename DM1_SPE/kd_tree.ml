type vector = float array

type kd_tree =
  | Vide
  | Node  of int * vector * kd_tree * kd_tree


(* GÉNÉRATION JEU TEST *)
let genere_jeu_donnes (n: int) : vector array =
  Array.init n (fun _ -> Array.init 2 (fun _ -> Random.float 1.))

(* OUTILS GRAPHIQUES *)

let yellow = Graphics.rgb 210 160 4
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

let rec pp_voisins (n: int) (t: kd_tree) (v: vector) : vector array =


let main_exemple () =
  let nb_points = 50 in
  let _t = genere_jeu_donnes nb_points in
  let kd_tree = cree_arbre_kd _t 2 in         (* TODO : remplacer ici par votre fonction de génération d'un arbre k dimensionel *)
  Graphics.open_graph " 1000x1000";
  draw_kd_tree kd_tree;
  Graphics.loop_at_exit [] (fun _ -> ())

let () = main_exemple ()

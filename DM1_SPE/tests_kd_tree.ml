let test_partition () =
  let t = [|3; 1; 4; 2; 5|] in
  let q = partition t 0 4 2 (<=) in
  assert (t.(q) = 4);
  for i = 0 to q - 1 do assert (t.(i) <= t.(q)) done;
  for i = q + 1 to 4 do assert (t.(i) >= t.(q)) done

let test_selection_rapide () =
  let t = [|5; 3; 8; 1; 4|] in
  let median = selection_rapide t 0 4 2 (<=) in
  assert (median = 4)

let test_cree_arbre_kd_aux () =
  let data = [|
    [|2.; 3.|];
    [|5.; 4.|];
    [|9.; 6.|];
    [|4.; 7.|];
    [|8.; 1.|];
    [|7.; 2.|]
  |] in
  let arr = Array.copy data in
  let arbre = cree_arbre_kd_aux arr 2 0 0 (Array.length arr - 1) in
  match arbre with
  | Node(0, v, g, d) ->
    assert (v.(0) = 7. || v.(0) = 5.);
    let rec check_left t =
      match t with
      | Vide -> ()
      | Node(i, v', g', d') ->
        assert (v'.(0) < v.(0));
        check_left g'; check_left d'
    in
    let rec check_right t =
      match t with
      | Vide -> ()
      | Node(i, v', g', d') ->
        assert (v'.(0) >= v.(0));
        check_right g'; check_right d'
    in
    check_left g;
    check_right d
  | _ -> assert false

let test_sq_distance () =
  let x = [|1.;2.;3.|] and y = [|4.;6.;3.|] in
  assert (sq_distance x y = 25.)

let test_nearest_among () =
  let x = [|0.;0.|] in
  let v1 = Some [|1.;1.|] and v2 = Some [|2.;2.|] and v3 = None in
  assert (nearest_among [v1; v2; v3] x = v1)

let test_pp_voisin () =
  let leaf = Node(0, [|1.;1.|], Vide, Vide) in
  let x = [|0.;0.|] in
  assert (pp_voisin leaf x = Some [|1.;1.|])

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

let test_k_nearest_among () =
  let x = [|0.;0.|] in
  let pts = [
    [|1.;1.|];
    [|2.;2.|];
    [|0.5;0.5|];
    [|5.;5.|];
    [|3.;4.|]
  ] in
  let res = k_nearest_among pts x 3 in
  assert (List.mem [|0.5;0.5|] res);
  assert (List.mem [|1.;1.|] res);
  assert (List.mem [|2.;2.|] res);
  assert (not (List.mem [|5.;5.|] res))


let test_pp_voisins () =
  let t =
    Node(0, [|5.;5.|],
      Node(1, [|2.;3.|], Vide, Vide),
      Node(1, [|8.;1.|], Vide, Vide)
    )
  in
  let x = [|3.;2.|] in
  let res = pp_voisins t x 2 in
  assert (List.length res = 2);
  assert (List.nth res 0 = [|2.;3.|]);
  assert (List.nth res 1 = [|5.;5.|])


let test_concat_4_until () =
  let s = [([|1.;1.|], 1.0)] in
  let ns = [] in
  let l = [([|3.;3.|], 3.0)] in
  let l' = [([|2.;2.|], 2.0); ([|4.;4.|], 4.0)] in
  let res = concat_4_until s ns l l' 3 in
  let dists = List.map snd res in
  assert (dists = [1.; 2.; 3.]);
  assert (List.map fst res = [[|1.;1.|]; [|2.;2.|]; [|3.;3.|]])


let test_pp_voisins_2_aux () =
  let t =
    Node(0, [|5.;5.|],
      Node(1, [|2.;3.|], Vide, Vide),
      Node(1, [|8.;1.|], Vide, Vide)
    )
  in
  let x = [|3.;2.|] in
  let res = pp_voisins_2_aux t x 2 in
  match res with
  | [ (v1, d1); (v2, d2) ] ->
    assert (v1 = [|2.;3.|]);
    assert (v2 = [|5.;5.|]);
    assert (d1 < d2)
  | _ -> assert false

let test_compare_vect_from () =
  let x = [|0.;0.|] in
  let v1 = [|1.;0.|] and v2 = [|2.;0.|] in
  assert (compare_vect_from x v1 v2 = -1);
  assert (compare_vect_from x v2 v1 = 1);
  assert (compare_vect_from x v1 v1 = 0)

let run_all_tests () =
  let tests = [
    ("partition", test_partition);
    ("selection_rapide", test_selection_rapide);
    ("cree_arbre_kd_aux", test_cree_arbre_kd_aux);
    ("sq_distance", test_sq_distance);
    ("nearest_among", test_nearest_among);
    ("pp_voisin", test_pp_voisin);
    ("insert", test_insert);
    ("k_nearest_among", test_k_nearest_among);
    ("pp_voisins", test_pp_voisins);
    ("concat_4_until", test_concat_4_until);
    ("pp_voisins_2_aux", test_pp_voisins_2_aux);
    ("compare_vect_from", test_compare_vect_from)
  ] in
  List.iter (fun (name, f) ->
    try
      f ();
      Printf.printf "✅ Test %s réussi\n" name
    with e ->
      Printf.printf "❌ Test %s échoué\n" name
  ) tests

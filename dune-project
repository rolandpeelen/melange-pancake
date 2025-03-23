(lang dune 3.8)
(using melange 0.1)
(name melange-pancake)

(generate_opam_files true)

(source (github rolandpeelen/melange-pancake))
(homepage "https://github.com/rolandpeelen/melange-pancake")
(bug_reports "https://github.com/rolandpeelen/melange-pancake")
(authors "Roland Peelen <rolandpeelen@gmail.com>")
(license "")
(documentation "") 

(package
 (name melange-pancake)
 (synopsis "Melange Pancake")
 (description "A tiny lens library that works with Melange")
 (allow_empty)
 (depends
  (ocaml (= 5.1.1))
  (dune (>= 3.17))

  (melange (>= 4.0 ))
  
  (reason (>= 3.15))
  (ppxlib (>= 0.31))

  (ocaml-lsp-server :dev)
  (melange-jest :with-test)
  (odoc :with-doc)))

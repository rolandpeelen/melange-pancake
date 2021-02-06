/*
 Semantically:
 - Every integer can be represented as a string
 - NOT Every string can be represented as an integer

 string
   ^
   |
   |
   |
  int

  Ie. You can always DOWN cast a string to an integer. But promoting (going UP)
  to a string is not always feasable. Hence the optional value.
 */

type t('a, 's) = {
  up: 's => option('a),
  down: 'a => 's,
};

let set = (prism: t('a, 's), state: 'a): 's => prism.down(state);
let view = (prism: t('a, 's), value: 's): option('a) => prism.up(value);

let over = (prism: t('a, 's), fn: 'a => 'a, value: 's): 's =>
  switch (prism.up(value)) {
  | Some(x) => fn(x) |> prism.down
  | None => value
  };

let compose = (p1: t('b, 'c), p0: t('a, 'b)): t('a, 'c) => {
  up: x =>
    switch (p0.up(x)) {
    | Some(y) => y |> p1.up
    | None => None
    },
  down: x => x |> p1.down |> p0.down,
};
let pipe = (p0, p1) => compose(p1, p0);

module Infix = {
  let (>-<<) = compose;
  let (>>-<) = pipe;
};

module Option = {
  let some: t('a, 's) = {up: Function.id, down: x => Some(x)};
};

module Result = {
  let ok: t('a, 's) = {
    up:
      fun
      | Ok(x) => Some(x)
      | Error(_) => None,
    down: x => Ok(x),
  };

  let error: t('a, 's) = {
    up:
      fun
      | Error(x) => Some(x)
      | Ok(_) => None,
    down: x => Error(x),
  };
};

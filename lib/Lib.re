module Function = {
  let compose = (f, g, x) => f(g(x));
  let pipe = (f, g, x) => g(f(x));
  let id = x => x;
  let const = (x, _) => x;

  let flip: 'a 'b 'c. (('a, 'b) => 'c, 'b, 'a) => 'c = (f, b, a) => f(a, b);

  module Infix = {
    let (>>) = pipe;
    let (<<) = compose;
  };
};

module Array = {
  let updateAtIndexUnsafe = (xs, i, x) =>
    Belt.Array.mapWithIndex(xs, (idx, y) => idx === i ? x : y);

  let updateAtIndex = (xs, i, x) => {
    switch (i) {
    | i when i > Belt.Array.length(xs) => xs
    | i when i < 0 =>
      let j = Belt.Array.length(xs) + i;
      updateAtIndexUnsafe(xs, j, x);
    | i => updateAtIndexUnsafe(xs, i, x)
    };
  };
};

module List = {
  let updateAtIndex = (xs, i, e) => {
    /* If 'i' < 0 -- we wrap around. Note that we do this with:
       (listLength + i) as i is a negative number (listLength - (-1))
       and we would be out of bounds otherwise.
       */
    i > 0
      ? Belt.List.mapWithIndex(xs, (j, x) => i === j ? e : x)
      : {
        let actualI = Belt.List.length(xs) + i;
        Belt.List.mapWithIndex(xs, (j, x) => actualI === j ? e : x);
      };
  };

  let rec findBy = fn =>
    fun
    | [] => None
    | [x, ..._] when fn(x) => Some(x)
    | [_, ...xs] => findBy(fn, xs);

  let replaceBy = (e, fn, xs) => Belt.List.map(xs, x => fn(x) ? e : x);
};

module Option = {
  let getOrElse = (x, o) => Belt.Option.getWithDefault(o, x);
  let sequence =
    fun
    | (Some(a), Some(b)) => Some((a, b))
    | _ => None;
};

module Result = {
  let getOrElse = (x, o) => Belt.Result.getWithDefault(o, x);
};

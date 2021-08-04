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
  let updateAtIndex = (xs, i, x) => {
    /* Instead of mapping like we do with the array,
       we splice the linked list and re-connect them with the element
       in the middle.
       If 'i' < 0 -- we wrap around. Not that we do this with:
       (listLength + i) as i is a negative number (listLength - (-1))
       and we would be out of bounds otherwise
       */
    let (h, t) =
      i > 0
        ? (Belt.List.take(xs, i), Belt.List.drop(xs, i + 1))
        : {
          let listLength = Belt.List.length(xs);
          (
            Belt.List.take(xs, listLength + i),
            Belt.List.drop(xs, listLength + i + 1),
          );
        };

    switch (h, t) {
    | (Some(h), Some(t)) => Belt.List.concatMany([|h, [x], t|])
    | (None, Some(t)) => Belt.List.concatMany([|[x], t|])
    | (Some(h), None) => Belt.List.concatMany([|h, [x]|])
    | (None, None) => xs
    };
  };

  let rec findBy = fn =>
    fun
    | [] => None
    | [x, ..._] when fn(x) => Some(x)
    | [_, ...xs] => findBy(fn, xs);

  let rec replaceBy = (e, fn) =>
    fun
    | [] => []
    | [x, ...xs] when fn(x) => [e, ...xs]
    | [x, ...xs] => [x, ...replaceBy(e, fn, xs)];
};

module Option = {
  let getOrElse = (x, o) => Belt.Option.getWithDefault(o, x);
};

module Result = {
  let getOrElse = (x, o) => Belt.Result.getWithDefault(o, x);
};

open Function.Infix;

type t('a, 'b) = {
  get: 'a => 'b,
  set: ('b, 'a) => 'a,
};

let view = (lens: t('a, 'b), state: 'a): 'b => lens.get(state);
let set = (lens: t('a, 'b), value: 'b, state: 'a): 'a =>
  lens.set(value, state);
let over = (lens: t('a, 'b), fn: 'b => 'b, state: 'a): 'a =>
  lens.set(lens.get(state) |> fn, state);

let compose = (l0: t('b, 'c), l1: t('a, 'b)): t('a, 'c) => {
  get: l1.get >> l0.get,
  set: l0.set >> over(l1),
};
let pipe = (l0, l1) => compose(l1, l0);

module Infix = {
  let (-<<) = compose;
  let (>>-) = pipe;
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

  /* Clones the full array when setting, no mutation returns the original
     array when setting out of bounds Belt.Array.set is a mutating operation.
     We want a new array.  Belt.Array.Copy uses splice under the hood, which
     is O(n), so mapping over the array and replacing the item at index i is
     just as fast, and infintely more clear than creating intermediate
     variables. Wraps around when using negative values.
  */
  let atOrElse = (i: int, default): t(array('a), 'a) => {
    get: (xs: array('a)) => {
      (
        i < 0
          ? Belt.Array.get(xs, Belt.Array.length(xs) + i)
          : Belt.Array.get(xs, i)
      )
      ->Belt.Option.getWithDefault(default);
    },
    set: (x: 'a, xs: array('a)) => updateAtIndex(xs, i, x),
  };

  let atOrExn = (i: int): t(array('a), 'a) => {
    get: (xs: array('a)) => {
      i < 0
        ? Belt.Array.getExn(xs, Belt.Array.length(xs) + i)
        : Belt.Array.getExn(xs, i);
    },
    set: (x: 'a, xs: array('a)) => updateAtIndex(xs, i, x),
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
    let listLength = Belt.List.length(xs);
    let (h, t) =
      i > 0
        ? (Belt.List.take(xs, i), Belt.List.drop(xs, i + 1))
        : (
          Belt.List.take(xs, listLength + i),
          Belt.List.drop(xs, listLength + i + 1),
        );

    switch (h, t) {
    | (Some(h), Some(t)) => Belt.List.concatMany([|h, [x], t|])
    | (None, Some(t)) => Belt.List.concatMany([|[x], t|])
    | (Some(h), None) => Belt.List.concatMany([|h, [x]|])
    | (None, None) => xs
    };
  };

  /* Clones the full list when setting, no mutation
     returns the original list when setting out of bounds */
  let atOrElse = (i: int, default): t(list('a), 'a) => {
    get: (xs: list('a)) => {
      (
        i < 0
          ? Belt.List.get(xs, Belt.List.length(xs) + i)
          : Belt.List.get(xs, i)
      )
      ->Belt.Option.getWithDefault(default);
    },
    set: (x: 'a, xs: list('a)) => updateAtIndex(xs, i, x),
  };

  let atOrExn = (i: int): t(list('a), 'a) => {
    get: (xs: list('a)) => (
      i < 0
        ? Belt.List.getExn(xs, Belt.List.length(xs) + i)
        : Belt.List.getExn(xs, i): 'a
    ),
    set: (x: 'a, xs: list('a)) => updateAtIndex(xs, i, x),
  };
};

module Option = {
  let orElse = (default: 'b): t('a, 'b) => {
    get: default |> Function.flip(Belt.Option.getWithDefault),
    set: (x, _) => Some(x),
  };

  let orExn: t('a, 'b) = {get: Belt.Option.getExn, set: (x, _) => Some(x)};
};

module Result = {
  let orElse = (default: 'b): t('a, 'b) => {
    get: default |> (Belt.Result.getWithDefault |> Function.flip),
    set: (x, _) => Ok(x),
  };

  let orExn: t('a, 'b) = {get: Belt.Result.getExn, set: (x, _) => Ok(x)};
};

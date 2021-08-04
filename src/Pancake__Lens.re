open Lib.Function.Infix;

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

module Option = {
  let orElse = (default: 'b): t('a, 'b) => {
    get: Lib.Option.getOrElse(default),
    set: (x, _) => Some(x),
  };

  let orExn: t('a, 'b) = {get: Belt.Option.getExn, set: (x, _) => Some(x)};
};

module Result = {
  let orElse = (default: 'b): t('a, 'b) => {
    get: Lib.Result.getOrElse(default),
    set: (x, _) => Ok(x),
  };

  let orExn: t('a, 'b) = {get: Belt.Result.getExn, set: (x, _) => Ok(x)};
};

module Array = {
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
    set: (x: 'a, xs: array('a)) => Lib.Array.updateAtIndex(xs, i, x),
  };

  let atOrExn = (i: int): t(array('a), 'a) => {
    get: (xs: array('a)) => {
      i < 0
        ? Belt.Array.getExn(xs, Belt.Array.length(xs) + i)
        : Belt.Array.getExn(xs, i);
    },
    set: (x: 'a, xs: array('a)) => Lib.Array.updateAtIndex(xs, i, x),
  };

  let find = (e: 'a): t(array('a), option('a)) => {
    get: Js.Array.find(y => y === e),
    set: (x: option('a), xs: array('a)) =>
      switch (x) {
      | Some(x) => Belt.Array.map(xs, y => y === e ? x : y)
      | None => xs
      },
  };

  let findByLens = (e: 'b, lens: t('a, 'b)): t(array('a), option('a)) => {
    get: Js.Array.find(y => view(lens, y) === e),
    set: (x: option('a), xs: array('a)) =>
      switch (x) {
      | Some(x) => Belt.Array.map(xs, y => view(lens, y) === e ? x : y)
      | None => xs
      },
  };
};

module List = {
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
    set: (x: 'a, xs: list('a)) => Lib.List.updateAtIndex(xs, i, x),
  };

  let atOrExn = (i: int): t(list('a), 'a) => {
    get: (xs: list('a)) => (
      i < 0
        ? Belt.List.getExn(xs, Belt.List.length(xs) + i)
        : Belt.List.getExn(xs, i): 'a
    ),
    set: (x: 'a, xs: list('a)) => Lib.List.updateAtIndex(xs, i, x),
  };

  let find = (e: 'a): t(list('a), option('a)) => {
    get: Lib.List.findBy(y => y === e),
    set: (x: option('a), xs: list('a)) =>
      switch (x) {
      | Some(x) => Lib.List.replaceBy(x, y => y === e, xs)
      | None => xs
      },
  };

  let findByLens = (e: 'b, lens: t('a, 'b)): t(list('a), option('a)) => {
    get: Lib.List.findBy(y => view(lens, y) === e),
    set: (x: option('a), xs: list('a)) =>
      switch (x) {
      | Some(x) => Lib.List.replaceBy(x, y => view(lens, y) === e, xs)
      | None => xs
      },
  };
};

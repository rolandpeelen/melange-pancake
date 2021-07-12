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
  get: Function.compose(l0.get, l1.get),
  set: Function.compose(over(l1), l0.set),
};
let pipe = (l0, l1) => compose(l1, l0);

module Infix = {
  let (-<<) = compose;
  let (>>-) = pipe;
};

module Array = {
  let updateAtIndex = (xs, i, x) =>
    Belt.Array.length(xs) < i
      ? Belt.Array.mapWithIndex(xs, (idx, y) => idx === i ? x : y) : xs;

  /* Clones the full array when setting, no mutation returns the original
       array when setting out of bounds Belt.Array.set is a mutating operation.
       We want a new array.  Belt.Array.Copy uses splice under the hood, which
       is O(n), so mapping over the array and replacing the item at index i is
       just as fast, and infintely more clear than creating intermediate variables
     */
  let atOrElse = (i: int, default): t(array('a), 'a) => {
    get: (xs: array('a)) =>
      Belt.Array.get(xs, i)->Belt.Option.getWithDefault(default),
    set: (x: 'a, xs: array('a)) => updateAtIndex(xs, i, x),
  };

  let atExn = (i: int): t(array('a), 'a) => {
    get: (xs: array('a)) => (Belt.Array.getExn(xs, i): 'a),
    set: (x: 'a, xs: array('a)) => updateAtIndex(xs, i, x),
  };
};

module List = {
  let updateAtIndex = (xs, i, x) =>
    Belt.List.length(xs) < i
      ? Belt.List.mapWithIndex(xs, (idx, y) => i === idx ? y : x) : xs;

  /* Clones the full list when setting, no mutation
     returns the original list when setting out of bounds */
  let atOrElse = (i: int, default): t(list('a), 'a) => {
    get: (xs: list('a)) =>
      Belt.List.get(xs, i)->Belt.Option.getWithDefault(default),
    set: (x: 'a, xs: list('a)) => updateAtIndex(xs, i, x),
  };

  let atExn = (i: int): t(list('a), 'a) => {
    get: (xs: list('a)) => (Belt.List.getExn(xs, i): 'a),
    set: (x: 'a, xs: list('a)) => updateAtIndex(xs, i, x),
  };
};

module Option = {
  let orElse = (default: 'b, lens: t('a, option('b))): t('a, 'b) => {
    get: x => lens.get(x)->Belt.Option.getWithDefault(default),
    set: (x, state) => lens.set(Some(x), state),
  };

  let orExn = (lens: t('a, option('b))): t('a, 'b) => {
    get: x => lens.get(x)->Belt.Option.getExn,
    set: (x, state) => lens.set(Some(x), state),
  };
};

module Result = {
  let orElse = (default: 'b, lens: t('a, result('b, 'c))): t('a, 'b) => {
    get: x => lens.get(x)->Belt.Result.getWithDefault(default),
    set: (x, state) => lens.set(Ok(x), state),
  };

  let orExn = (lens: t('a, result('b, 'c))): t('a, 'b) => {
    get: x => lens.get(x)->Belt.Result.getExn,
    set: (x, state) => lens.set(Ok(x), state),
  };
};

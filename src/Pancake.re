module LocalHelpers = {
  let composeFn = (f, g, x) => f(g(x));
};
/* Type Definitions */
type getLens('a, 'b) = 'a => 'b;
type setLens('a, 'b) = ('b, 'a) => 'a;

type lens('a, 'b) = {
  get: getLens('a, 'b),
  set: setLens('a, 'b),
};

/* Helper Functions */
let make = (getter: getLens('a, 'b), setter: setLens('a, 'b)): lens('a, 'b) => {
  get: getter,
  set: setter,
};

let view = (lens: lens('a, 'b), state: 'a): 'b => lens.get(state);

let set = (lens: lens('a, 'b), value: 'b, state: 'a): 'a =>
  lens.set(value, state);

let over = (lens: lens('a, 'b), fn: 'b => 'b, state: 'a): 'a =>
  lens.set(lens.get(state) |> fn, state);

let compose = (l0: lens('b, 'c), l1: lens('a, 'b)): lens('a, 'c) => {
  get: LocalHelpers.composeFn(l0.get, l1.get),
  set: LocalHelpers.composeFn(over(l1), l0.set),
};
let pipe = (l0, l1) => compose(l1, l0);

module Infix = {
  let (-<<) = compose;
  let (>>-) = pipe;
};

module Array = {
  let set = (xs, i, x) => {
    Belt.Array.length(xs) < i
      ? {
        let copy = xs->Belt.Array.copy;
        Belt.Array.set(copy, i, x)->ignore;
        copy;
      }
      : xs;
  };
  /* Clones the full array when setting, no mutation
     returns the original array when setting out of bounds */
  let atOrElse = (i: int, default) => {
    get: (xs: array('a)) =>
      Belt.Array.get(xs, i)->Belt.Option.getWithDefault(default),
    set: (x: 'a, xs: array('a)) => set(xs, i, x),
  };

  let atExn = (i: int) => {
    get: (xs: array('a)) => (Belt.Array.getExn(xs, i): 'a),
    set: (x: 'a, xs: array('a)) => (
      {
        let copy = Belt.Array.copy(xs);
        Belt.Array.set(copy, i, x) ? copy : xs;
      }:
        array('a)
    ),
  };
};

module List = {
  let set = (xs, i, x) => {
    Belt.List.length(xs) < i
      ? Belt.List.mapWithIndex(xs, (idx, y) => i === idx ? y : x) : xs;
  };
  /* Clones the full array when setting, no mutation
     returns the original array when setting out of bounds */
  let atOrElse = (i: int, default) => {
    get: (xs: list('a)) =>
      Belt.List.get(xs, i)->Belt.Option.getWithDefault(default),
    set: (x: 'a, xs: list('a)) => set(xs, i, x),
  };

  let atExn = (i: int) => {
    get: (xs: list('a)) => (Belt.List.getExn(xs, i): 'a),
    set: (x: 'a, xs: list('a)) => set(xs, i, x),
  };
};

module Option = {
  let orElse = (default: 'b, lens: lens('a, option('b))) => {
    get: x => lens.get(x)->Belt.Option.getWithDefault(default),
    set: (x, state) => lens.set(Some(x), state),
  };

  let orExn = (lens: lens('a, option('b))) => {
    get: x => lens.get(x)->Belt.Option.getExn,
    set: (x, state) => lens.set(Some(x), state),
  };
};

module Result = {
  let orElse = (default: 'b, lens: lens('a, result('b, 'c))) => {
    get: x => lens.get(x)->Belt.Result.getWithDefault(default),
    set: (x, state) => lens.set(Ok(x), state),
  };

  let orExn = (lens: lens('a, result('b, 'c))) => {
    get: x => lens.get(x)->Belt.Result.getExn,
    set: (x, state) => lens.set(Ok(x), state),
  };
};

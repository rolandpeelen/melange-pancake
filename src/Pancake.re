type getLens('a, 'b) = 'a => 'b;
type setLens('a, 'b) = ('b, 'a) => 'a;

type lens('a, 'b) = {
  get: getLens('a, 'b),
  set: setLens('a, 'b),
};

let make = (getter: getLens('a, 'b), setter: setLens('a, 'b)): lens('a, 'b) => {
  get: getter,
  set: setter,
};

let view = (lens: lens('a, 'b), state: 'a): 'b => lens.get(state);
let set = (lens: lens('a, 'b), value: 'b, state: 'a): 'a =>
  lens.set(value, state);
let over = (lens: lens('a, 'b), fn: 'b => 'b, state: 'a): 'a =>
  lens.set(lens.get(state) |> fn, state);

let composeFn = (f, g, x) => f(g(x));
let compose = (l0: lens('b, 'c), l1: lens('a, 'b)): lens('a, 'c) => {
  get: composeFn(l0.get, l1.get),
  set: composeFn(over(l1), l0.set),
};
let pipe = (l0, l1) => compose(l1, l0);

module Infix = {
  let (-<<) = compose;
  let (>>-) = pipe;
};

let compose = (f, g, x) => f(g(x));
let pipe = (f, g, x) => g(f(x));
let id = x => x;
let const = (x, _) => x;

let flip: 'a 'b 'c. (('a, 'b) => 'c, 'b, 'a) => 'c = (f, b, a) => f(a, b);

module Infix = {
  let (>>) = pipe;
  let (<<) = compose;
};

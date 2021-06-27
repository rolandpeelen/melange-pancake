let compose = (f, g, x) => f(g(x));
let id = x => x;
let const = (x, _) => x;

let flip: 'a 'b 'c. (('a, 'b) => 'c, 'b, 'a) => 'c = (f, b, a) => f(a, b);

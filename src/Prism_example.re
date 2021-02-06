open Prism.Infix;
Js.log("prisms...");

let add5 = x => x + 5;

let _assert = (x, expr) =>
  expr
    ? Js.log3("\x1b[32m", x ++ " -> OK", "\x1b[0m")
    : Js.log3("\x1b[31m", x ++ " -> FAIL", "\x1b[0m");

_assert(
  "View ok Ok(10) === Some(10)",
  Prism.view(Prism.Result.ok, Ok(10)) === Some(10),
);

_assert(
  "View ok Error(10) === None",
  Prism.view(Prism.Result.ok, Error(10)) === None,
);

_assert(
  "View ok Error(10) === None",
  Prism.view(Prism.Result.ok, Error(10)) === None,
);

_assert(
  "Over ok Ok(10), add5 === Ok(15)",
  Prism.over(Prism.Result.ok, add5, Ok(10)) == Ok(15),
);

_assert(
  "Over ok Error('foo'), add5 === None",
  Prism.over(Prism.Result.ok, add5, Error("foo")) == Error("foo"),
);

let stringToIntPrism: Prism.t(int, string) = {
  up: int_of_string_opt, // Can fail, not all strings are ints
  down: string_of_int // Never fails, all ints can be "stringified"
};

_assert(
  "Review stringInt \"25\" === 25",
  Prism.set(stringToIntPrism, 25) === "25",
);

_assert(
  "Preview stringInt \"25\" === Some(25)",
  Prism.view(stringToIntPrism, "25") === Some(25),
);

_assert(
  "Preview stringInt \"foo\" === None",
  Prism.view(stringToIntPrism, "foo") === None,
);

_assert(
  "Over stringInt \"5\", add 5 === Some(10)",
  Prism.over(stringToIntPrism, x => x + 5, "5") === "10",
);

_assert(
  "Over stringInt \"foo\", add 4 === None",
  Prism.over(stringToIntPrism, x => x + 5, "foo") === "foo",
);

type foo =
  | Bar(string)
  | Baz(int);

[@pancake]
type user = {
  username: string,
  foo,
};

let fooToString: Prism.t(string, foo) = {
  up:
    fun
    | Bar(x) => Some(x)
    | _ => None,
  down: x => Bar(x),
};
let fooToInt: Prism.t(int, foo) = {
  up:
    fun
    | Baz(x) => Some(x)
    | _ => None,
  down: x => Baz(x),
};

let user = {username: "bar", foo: Baz(5)};

let updateUserFooDouble = user =>
  Lens.over(userFooLens, Prism.over(fooToInt, add5), user);

let userDoubleOutput = {username: "bar", foo: Baz(10)};
_assert(
  "Update user foo is possible",
  Lens.over(userFooLens, Prism.over(fooToInt, add5), user)
  == userDoubleOutput,
);

let user = {username: "bar", foo: Bar("bar")};
let userUpperOutput = {username: "bar", foo: Bar("BAR")};
_assert(
  "Update user foo (uppercase) is possible",
  Lens.over(
    userFooLens,
    Prism.over(fooToString, Js.String.toUpperCase),
    user,
  )
  == userUpperOutput,
);

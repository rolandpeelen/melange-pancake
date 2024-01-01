open Pancake;

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
  Prism.over(Prism.Result.ok, add5, Error("foo"))
  == Error("foo"),
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

type age =
  | String(string)
  | Int(int);

[@pancake]
type user = {
  username: string,
  age,
};

let ageAsString: Prism.t(string, age) = {
  up:
    fun
    | String(x) => Some(x)
    | Int(_) => None,
  down: x => String(x),
};
let ageAsInt: Prism.t(int, age) = {
  up:
    fun
    | Int(x) => Some(x)
    | String(_) => None,
  down: x => Int(x),
};

let user = {username: "bar", age: Int(5)};

let userDoubleOutput = {username: "bar", age: Int(10)};
_assert(
  "Update user foo is possible",
  Lens.over(userAgeLens, Prism.over(ageAsInt, add5), user)
  == userDoubleOutput,
);

let user = {username: "bar", age: String("25")};
let userUpperOutput = {username: "bar", age: String("25")};
_assert(
  "Update user foo (uppercase) is possible",
  Lens.over(
    userAgeLens,
    Prism.over(ageAsString, Js.String.toUpperCase),
    user,
  )
  == userUpperOutput,
);


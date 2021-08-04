open Function;
open Jest;
open Expect;

open Pancake;
open Pancake.Lens.Infix;

[@pancake]
type metric = {speed: array(int)};

let metric = {speed: [|0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10|]};

describe("Lens Array atOrElse", () => {
  test(
    "Expect correct value when Lens.view(Array.atOrElse) is within bounds",
    Lens.view(metricSpeedLens >>- Lens.Array.atOrElse(1, 100), metric)
    |> expect
    |> toEqual(1)
    |> const,
  );

  test(
    "Expect fallback value when Lens.view(Array.atOrElse) is out of bounds",
    Lens.view(metricSpeedLens >>- Lens.Array.atOrElse(25, 100), metric)
    |> expect
    |> toEqual(100)
    |> const,
  );

  test(
    "Wrap around negative values when Lens.view(Array.atOrElse)",
    Lens.view(metricSpeedLens >>- Lens.Array.atOrElse(-1, 100), metric)
    |> expect
    |> toEqual(10)
    |> const,
  );

  test(
    "Set correct value when Lens.set(Array.atOrElse) is within bounds",
    Lens.set(metricSpeedLens >>- Lens.Array.atOrElse(1, 100), 100, metric)
    |> expect
    |> toEqual({speed: [|0, 100, 2, 3, 4, 5, 6, 7, 8, 9, 10|]})
    |> const,
  );

  test(
    "Return original value when Array.set(Array.atOrElse) is out of bounds",
    Lens.set(metricSpeedLens >>- Lens.Array.atOrElse(25, 100), 100, metric)
    |> expect
    |> toEqual({speed: [|0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10|]})
    |> const,
  );

  test(
    "Wrap around negative values when Array.set(Array.atOrElse)",
    Lens.set(metricSpeedLens >>- Lens.Array.atOrElse(-1, 100), 100, metric)
    |> expect
    |> toEqual({speed: [|0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 100|]})
    |> const,
  );
});

describe("Lens Array atOrExn", () => {
  test(
    "Expect correct value when Lens.view(Array.atOrExn) is within bounds",
    Lens.view(metricSpeedLens >>- Lens.Array.atOrExn(1), metric)
    |> expect
    |> toEqual(1)
    |> const,
  );

  // TODO - fix using toThrow ?
  test(
    "Expect an exception when Lens.view(Array.atOrExn) is out of bounds", () =>
    switch (Lens.view(metricSpeedLens >>- Lens.Array.atOrExn(25), metric)) {
    | exception _ => expect(1) |> toEqual(1)
    | _ => fail("Expected exception, but received other value")
    }
  );

  test(
    "Wrap around negative values when Lens.view(Array.atOrExn)",
    Lens.view(metricSpeedLens >>- Lens.Array.atOrExn(-1), metric)
    |> expect
    |> toEqual(10)
    |> const,
  );

  test(
    "Set correct value when Lens.set(Array.atOrExn) is within bounds",
    Lens.set(metricSpeedLens >>- Lens.Array.atOrExn(1), 100, metric)
    |> expect
    |> toEqual({speed: [|0, 100, 2, 3, 4, 5, 6, 7, 8, 9, 10|]})
    |> const,
  );

  test(
    "Return original value when Array.set(Array.atOrExn) is out of bounds",
    Lens.set(metricSpeedLens >>- Lens.Array.atOrExn(25), 100, metric)
    |> expect
    |> toEqual({speed: [|0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10|]})
    |> const,
  );

  test(
    "Wrap around negative values when Array.set(Array.atOrExn)",
    Lens.set(metricSpeedLens >>- Lens.Array.atOrExn(-1), 100, metric)
    |> expect
    |> toEqual({speed: [|0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 100|]})
    |> const,
  );
});

describe("Lens Array find", () => {
  test(
    "Expect correct update when found",
    Lens.set(metricSpeedLens >>- Lens.Array.find(5), Some(200), metric)
    |> expect
    |> toEqual({speed: [|0, 1, 2, 3, 4, 200, 6, 7, 8, 9, 10|]})
    |> const,
  );

  test(
    "Expect correct update of value when found, composed through orExn",
    Lens.set(
      metricSpeedLens >>- Lens.Array.find(5) >>- Lens.Option.orExn,
      200,
      metric,
    )
    |> expect
    |> toEqual({speed: [|0, 1, 2, 3, 4, 200, 6, 7, 8, 9, 10|]})
    |> const,
  );

  test(
    "Expect correct value when value is found",
    Lens.view(metricSpeedLens >>- Lens.Array.find(5), metric)
    |> expect
    |> toEqual(Some(5))
    |> const,
  );

  test(
    "Expect None when value isn't found",
    Lens.view(metricSpeedLens >>- Lens.Array.find(200), metric)
    |> expect
    |> toEqual(None)
    |> const,
  );
});

[@pancake]
type user = {
  id: int,
  username: string,
};
let users = [|
  {id: 0, username: "0"},
  {id: 1, username: "1"},
  {id: 2, username: "2"},
  {id: 3, username: "3"},
|];

describe("Lens Array findByLens", () => {
  test(
    "Expect to find user when user is correct",
    Lens.view(Lens.Array.findByLens("0", userUsernameLens), users)
    |> expect
    |> toEqual(Some({id: 0, username: "0"}))
    |> const,
  );

  test(
    "Expect to find user when id is correct",
    Lens.view(Lens.Array.findByLens(0, userIdLens), users)
    |> expect
    |> toEqual(Some({id: 0, username: "0"}))
    |> const,
  );

  test(
    "Expect not to find user when id is correct",
    Lens.view(Lens.Array.findByLens(25, userIdLens), users)
    |> expect
    |> toEqual(None)
    |> const,
  );
});

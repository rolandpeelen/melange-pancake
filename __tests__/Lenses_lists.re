open Function;
open Jest;
open Expect;

open Pancake;
open Pancake.Lens.Infix;

[@pancake]
type metric = {speed: list(int)};

let metric = {speed: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]};

describe("Lens List atOrElse", () => {
  test(
    "Expect correct value when Lens.view(List.atOrElse) is within bounds",
    Lens.view(metricSpeedLens >>- Lens.List.atOrElse(1, 100), metric)
    |> expect
    |> toEqual(1)
    |> const,
  );

  test(
    "Expect fallback value when Lens.view(List.atOrElse) is out of bounds",
    Lens.view(metricSpeedLens >>- Lens.List.atOrElse(25, 100), metric)
    |> expect
    |> toEqual(100)
    |> const,
  );

  test(
    "Wrap around negative values when Lens.view(List.atOrElse)",
    Lens.view(metricSpeedLens >>- Lens.List.atOrElse(-1, 100), metric)
    |> expect
    |> toEqual(10)
    |> const,
  );

  test(
    "Set correct value when Lens.set(List.atOrElse) is within bounds",
    Lens.set(metricSpeedLens >>- Lens.List.atOrElse(1, 100), 100, metric)
    |> expect
    |> toEqual({speed: [0, 100, 2, 3, 4, 5, 6, 7, 8, 9, 10]})
    |> const,
  );

  test(
    "Return original value when List.set(List.atOrElse) is out of bounds",
    Lens.set(metricSpeedLens >>- Lens.List.atOrElse(25, 100), 100, metric)
    |> expect
    |> toEqual({speed: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]})
    |> const,
  );

  test(
    "Wrap around negative values when List.set(List.atOrElse)",
    Lens.set(metricSpeedLens >>- Lens.List.atOrElse(-1, 100), 100, metric)
    |> expect
    |> toEqual({speed: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 100]})
    |> const,
  );
});

describe("Lens List atOrExn", () => {
  test(
    "Expect correct value when Lens.view(List.atOrExn) is within bounds",
    Lens.view(metricSpeedLens >>- Lens.List.atOrExn(1), metric)
    |> expect
    |> toEqual(1)
    |> const,
  );

  // TODO - fix using toThrow ?
  test("Expect an exception when Lens.view(List.atOrExn) is out of bounds", () =>
    switch (Lens.view(metricSpeedLens >>- Lens.List.atOrExn(25), metric)) {
    | exception _ => expect(1) |> toEqual(1)
    | _ => fail("Expected exception, but received other value")
    }
  );

  test(
    "Wrap around negative values when Lens.view(List.atOrExn)",
    Lens.view(metricSpeedLens >>- Lens.List.atOrExn(-1), metric)
    |> expect
    |> toEqual(10)
    |> const,
  );

  test(
    "Set correct value when Lens.set(List.atOrExn) is within bounds",
    Lens.set(metricSpeedLens >>- Lens.List.atOrExn(1), 100, metric)
    |> expect
    |> toEqual({speed: [0, 100, 2, 3, 4, 5, 6, 7, 8, 9, 10]})
    |> const,
  );

  test(
    "Return original value when List.set(List.atOrExn) is out of bounds",
    Lens.set(metricSpeedLens >>- Lens.List.atOrExn(25), 100, metric)
    |> expect
    |> toEqual({speed: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]})
    |> const,
  );

  test(
    "Wrap around negative values when List.set(List.atOrExn)",
    Lens.set(metricSpeedLens >>- Lens.List.atOrExn(-1), 100, metric)
    |> expect
    |> toEqual({speed: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 100]})
    |> const,
  );
});

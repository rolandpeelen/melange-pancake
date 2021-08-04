open Jest;
open Expect;
open Pancake;
open Pancake.Lens.Infix;

[@pancake]
type metric = {speed: option(int)};

let metric = {speed: Some(10)};

describe("Lens Array orElse", () => {
  test(
    "Expect correct value when Lens.view(Option.orElse)",
    Lens.view(metricSpeedLens >>- Lens.Option.orElse(1000), metric)
    |> expect
    |> toEqual(10)
    |> Lib.Function.const,
  );

  test(
    "Expect fallback value when None Lens.view(Option.orElse)",
    Lens.view(metricSpeedLens >>- Lens.Option.orElse(1000), {speed: None})
    |> expect
    |> toEqual(1000)
    |> Lib.Function.const,
  );

  test(
    "Expect correct value when Lens.set(Option.orElse)",
    Lens.set(metricSpeedLens >>- Lens.Option.orElse(1000), 10, metric)
    |> expect
    |> toEqual({speed: Some(10)})
    |> Lib.Function.const,
  );

  test(
    "Expect correct value when Lens.set(Option.orElse)",
    Lens.set(
      metricSpeedLens >>- Lens.Option.orElse(1000),
      10,
      {speed: None},
    )
    |> expect
    |> toEqual({speed: Some(10)})
    |> Lib.Function.const,
  );
});

describe("Lens Array orExn", () => {
  test(
    "Expect correct value when Lens.view(Option.orExn)",
    Lens.view(metricSpeedLens >>- Lens.Option.orExn, metric)
    |> expect
    |> toEqual(10)
    |> Lib.Function.const,
  );

  // TODO - fix using toThrow ?
  test("Expect an exception when Lens.view(List.atOrExn) is out of bounds", () =>
    switch (Lens.view(metricSpeedLens >>- Lens.Option.orExn, {speed: None})) {
    | exception _ => expect(1) |> toEqual(1)
    | _ => fail("Expected exception, but received other value")
    }
  );

  test(
    "Expect correct value when Lens.set(Option.orExn)",
    Lens.set(metricSpeedLens >>- Lens.Option.orExn, 10, metric)
    |> expect
    |> toEqual({speed: Some(10)})
    |> Lib.Function.const,
  );

  test(
    "Expect correct value when Lens.set(Option.orExn)",
    Lens.set(metricSpeedLens >>- Lens.Option.orExn, 10, {speed: None})
    |> expect
    |> toEqual({speed: Some(10)})
    |> Lib.Function.const,
  );
});

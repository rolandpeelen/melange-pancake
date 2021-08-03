open Function;
open Jest;
open Expect;

open Pancake;
open Pancake.Lens.Infix;

[@pancake]
type metric = {speed: result(int, string)};

let metric = {speed: Ok(10)};

describe("Lens Array orElse", () => {
  test(
    "Expect correct value when Lens.view(Result.orElse)",
    Lens.view(metricSpeedLens >>- Lens.Result.orElse(1000), metric)
    |> expect
    |> toEqual(10)
    |> const,
  );

  test(
    "Expect fallback value when Error Lens.view(Result.orElse)",
    Lens.view(
      metricSpeedLens >>- Lens.Result.orElse(1000),
      {speed: Error("Some Error")},
    )
    |> expect
    |> toEqual(1000)
    |> const,
  );

  test(
    "Expect correct value when Lens.set(Result.orElse)",
    Lens.set(metricSpeedLens >>- Lens.Result.orElse(1000), 10, metric)
    |> expect
    |> toEqual({speed: Ok(10)})
    |> const,
  );

  test(
    "Expect correct value when Lens.set(Result.orElse)",
    Lens.set(
      metricSpeedLens >>- Lens.Result.orElse(1000),
      10,
      {speed: Error("Some Error")},
    )
    |> expect
    |> toEqual({speed: Ok(10)})
    |> const,
  );
});

describe("Lens Array orExn", () => {
  test(
    "Expect correct value when Lens.view(Result.orExn)",
    Lens.view(metricSpeedLens >>- Lens.Result.orExn, metric)
    |> expect
    |> toEqual(10)
    |> const,
  );

  // TODO - fix using toThrow ?
  test("Expect an exception when Lens.view(List.atOrExn) is out of bounds", () =>
    switch (
      Lens.view(
        metricSpeedLens >>- Lens.Result.orExn,
        {speed: Error("Some Error")},
      )
    ) {
    | exception _ => expect(1) |> toEqual(1)
    | _ => fail("Expected exception, but received other value")
    }
  );

  test(
    "Expect correct value when Lens.set(Result.orExn)",
    Lens.set(metricSpeedLens >>- Lens.Result.orExn, 10, metric)
    |> expect
    |> toEqual({speed: Ok(10)})
    |> const,
  );

  test(
    "Expect correct value when Lens.set(Result.orExn)",
    Lens.set(
      metricSpeedLens >>- Lens.Result.orExn,
      10,
      {speed: Error("Some Error")},
    )
    |> expect
    |> toEqual({speed: Ok(10)})
    |> const,
  );
});

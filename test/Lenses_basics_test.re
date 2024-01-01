open Jest;
open Expect;
open Pancake;

[@pancake]
type blog = {
  id: int,
  title: string,
  body: option(string),
};

let blog = {id: 0, title: "title", body: Some("body")};

module View = {
  describe("Lens.view", () => {
    test(
      "id",
      Lens.view(blogIdLens, blog)
      |> expect
      |> toEqual(0)
      |> Lib.Function.const,
    );
    test(
      "title",
      Lens.view(blogTitleLens, blog)
      |> expect
      |> toEqual("title")
      |> Lib.Function.const,
    );
    test(
      "body",
      Lens.view(blogBodyLens, blog)
      |> expect
      |> toEqual(Some("body"))
      |> Lib.Function.const,
    );
  });
};
module Set = {
  describe("Lens.set", () => {
    test(
      "id",
      Lens.set(blogIdLens, 1, blog)
      |> expect
      |> toEqual({...blog, id: 1})
      |> Lib.Function.const,
    );
    test(
      "title",
      Lens.set(blogTitleLens, "newTitle", blog)
      |> expect
      |> toEqual({...blog, title: "newTitle"})
      |> Lib.Function.const,
    );
    test(
      "body",
      Lens.set(blogBodyLens, None, blog)
      |> expect
      |> toEqual({...blog, body: None})
      |> Lib.Function.const,
    );
  });
};
module Over = {
  describe("Lens.over", () => {
    test(
      "id",
      Lens.over(blogIdLens, x => x + 1, blog)
      |> expect
      |> toEqual({...blog, id: 1})
      |> Lib.Function.const,
    );
    test(
      "title",
      Lens.over(blogTitleLens, Js.String.toUpperCase, blog)
      |> expect
      |> toEqual({...blog, title: "TITLE"})
      |> Lib.Function.const,
    );
    test(
      "body",
      Lens.over(
        blogBodyLens,
        x => Belt.Option.map(x, Js.String.toUpperCase),
        blog,
      )
      |> expect
      |> toEqual({...blog, body: Some("BODY")})
      |> Lib.Function.const,
    );
  });
};

module Effect = {
  describe("Lens.effect", () => {
    let id = ref(None);
    test("Effect works and acceps functions returning unit", () => {
      Lens.effect(blogIdLens, x => id := Some(x), blog);

      expect(id^) |> toEqual(Some(0));
    });
  });
};

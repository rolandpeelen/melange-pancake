open Function;
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
      Lens.view(blogIdLens, blog) |> expect |> toEqual(0) |> const,
    );
    test(
      "title",
      Lens.view(blogTitleLens, blog) |> expect |> toEqual("title") |> const,
    );
    test(
      "body",
      Lens.view(blogBodyLens, blog)
      |> expect
      |> toEqual(Some("body"))
      |> const,
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
      |> const,
    );
    test(
      "title",
      Lens.set(blogTitleLens, "newTitle", blog)
      |> expect
      |> toEqual({...blog, title: "newTitle"})
      |> const,
    );
    test(
      "body",
      Lens.set(blogBodyLens, None, blog)
      |> expect
      |> toEqual({...blog, body: None})
      |> const,
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
      |> const,
    );
    test(
      "title",
      Lens.over(blogTitleLens, Js.String.toUpperCase, blog)
      |> expect
      |> toEqual({...blog, title: "TITLE"})
      |> const,
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
      |> const,
    );
  });
};

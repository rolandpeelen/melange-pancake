open Jest;
open Expect;
open Pancake;
open Pancake.Lens.Infix;

[@pancake]
type author = {
  id: int,
  name: string,
  email: option(string),
};

[@pancake]
type blog = {
  id: int,
  author: option(author),
};

let author = {id: 0, name: "Roland", email: None};
let blog = {id: 0, author: Some(author)};

let blogNoAuthor = {id: 0, author: None};

module View = {
  describe("Lens.view", () => {
    let fallbackAuthor = {id: 0, name: "No Author", email: None};
    test(
      "Author Name",
      Lens.view(blogAuthorLens >>- Lens.Option.orExn >>- authorNameLens, blog)
      |> expect
      |> toEqual("Roland")
      |> Lib.Function.const,
    );

    test(
      "Author Name when none",
      Lens.view(
        blogAuthorLens
        >>- Lens.Option.orElse(fallbackAuthor)
        >>- authorNameLens,
        blogNoAuthor,
      )
      |> expect
      |> toEqual("No Author")
      |> Lib.Function.const,
    );
  });
};
module Set = {
  describe("Lens.set", () => {
    test(
      "Author Name",
      Lens.set(
        blogAuthorLens >>- Lens.Option.orExn >>- authorNameLens,
        "New Author",
        blog,
      )
      |> expect
      |> toEqual({...blog, author: Some({...author, name: "New Author"})})
      |> Lib.Function.const,
    );

    test(
      "Author Email",
      Lens.set(
        blogAuthorLens
        >>- Lens.Option.orExn
        >>- authorEmailLens
        >>- Lens.Option.orExn,
        "email",
        blog,
      )
      |> expect
      |> toEqual({
           ...blog,
           author: Some({...author, email: Some("email")}),
         })
      |> Lib.Function.const,
    );
  });
};
module Over = {
  describe("Lens.over", () => {
    test(
      "Author Name",
      Lens.over(
        blogAuthorLens >>- Lens.Option.orExn >>- authorNameLens,
        Js.String.toUpperCase,
        blog,
      )
      |> expect
      |> toEqual({...blog, author: Some({...author, name: "ROLAND"})})
      |> Lib.Function.const,
    );

    test(
      "Author Email",
      Lens.over(
        blogAuthorLens
        >>- Lens.Option.orExn
        >>- authorEmailLens
        >>- Lens.Option.orExn,
        Js.String.toUpperCase,
        {...blog, author: Some({...author, email: Some("email")})},
      )
      |> expect
      |> toEqual({
           ...blog,
           author: Some({...author, email: Some("EMAIL")}),
         })
      |> Lib.Function.const,
    );
  });
};

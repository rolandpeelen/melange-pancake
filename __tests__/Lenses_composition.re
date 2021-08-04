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
    test(
      "Author Name",
      Lens.view(authorNameLens -<< Lens.Option.orExn -<< blogAuthorLens, blog)
      |> expect
      |> toEqual("Roland")
      |> Lib.Function.const,
    );

    let fallbackAuthor = {id: 0, name: "No Author", email: None};
    test(
      "Author Name when none",
      Lens.view(
        authorNameLens
        -<< Lens.Option.orElse(fallbackAuthor)
        -<< blogAuthorLens,
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
        authorNameLens -<< Lens.Option.orExn -<< blogAuthorLens,
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
        Lens.Option.orExn
        -<< authorEmailLens
        -<< Lens.Option.orExn
        -<< blogAuthorLens,
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
        authorNameLens -<< Lens.Option.orExn -<< blogAuthorLens,
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
        Lens.Option.orExn
        -<< authorEmailLens
        -<< Lens.Option.orExn
        -<< blogAuthorLens,
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

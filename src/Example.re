open Pancake.Lens.Infix;

// TODO -- Move to actual tests

module List = {
  /* ------------- */
  /* State Definition */
  /* ------------- */
  [@pancake]
  type blog = {
    id: int,
    title: string,
    body: string,
  };

  [@pancake]
  type state = {blogs: list(blog)};

  let blogs = [
    {id: 0, title: "Title 01", body: "Body 01"},
    {id: 1, title: "Title 02", body: "Body 02"},
    {id: 2, title: "Title 03", body: "Body 03"},
  ];

  let state = {blogs: blogs};
  /* ------------- */

  /* Create lens for a blog at a certain index (throwing if out of bounds) */
  let titleForBlogAtIndexLens = i =>
    stateBlogsLens >>- Pancake.Lens.List.atExn(i) >>- blogTitleLens;

  /* View, Set And, Over for that lens */
  (
    Pancake.Lens.view(titleForBlogAtIndexLens(0), state),
    Pancake.Lens.set(titleForBlogAtIndexLens(0), "Foobar", state),
    Pancake.Lens.over(
      titleForBlogAtIndexLens(0),
      Js.String.toUpperCase,
      state,
    ),
  )
  |> Js.log;

  /* Create lens for a blog at a certain index (falling back when out of bounds) */
  let titleForBlogAtIndexLensSafe = i =>
    stateBlogsLens
    >>- Pancake.Lens.List.atOrElse(
          i,
          {id: 0, title: "Fallback Title", body: "Fallback Body"},
        )
    >>- blogTitleLens;

  /* View, Set And, Over for that lens */
  (
    Pancake.Lens.view(titleForBlogAtIndexLensSafe(0), state),
    Pancake.Lens.set(
      titleForBlogAtIndexLensSafe(1),
      "New Title for id 1",
      state,
    ),
    Pancake.Lens.over(
      titleForBlogAtIndexLensSafe(0),
      Js.String.toUpperCase,
      state,
    ),
  )
  |> Js.log;

  /* View, Set And, Over for that lens, when out of bounds */
  (
    Pancake.Lens.view(titleForBlogAtIndexLensSafe(12), state),
    Pancake.Lens.set(
      titleForBlogAtIndexLensSafe(12),
      "This won't work",
      state,
    ),
    Pancake.Lens.over(
      titleForBlogAtIndexLensSafe(12),
      Js.String.toUpperCase,
      state,
    ),
  )
  |> Js.log;
};

module Array = {
  /* ------------- */
  /* State Definition */
  /* ------------- */
  [@pancake]
  type blog = {
    id: int,
    title: string,
    body: string,
  };

  [@pancake]
  type state = {blogs: array(blog)};

  let blogs = [|
    {id: 0, title: "Title 01", body: "Body 01"},
    {id: 1, title: "Title 02", body: "Body 02"},
    {id: 2, title: "Title 03", body: "Body 03"},
  |];

  let state = {blogs: blogs};
  /* ------------- */

  Js.log("---- ARRAYS ----");
  /* Create lens for a blog at a certain index (throwing if out of bounds) */
  let titleForBlogAtIndexLens = i =>
    stateBlogsLens >>- Pancake.Lens.Array.atExn(i) >>- blogTitleLens;

  /* View, Set And, Over for that lens */
  (
    Pancake.Lens.view(titleForBlogAtIndexLens(0), state),
    Pancake.Lens.set(titleForBlogAtIndexLens(0), "Foobar", state),
    Pancake.Lens.over(
      titleForBlogAtIndexLens(0),
      Js.String.toUpperCase,
      state,
    ),
  )
  |> Js.Json.stringifyAny
  |> Js.log;

  /* Create lens for a blog at a certain index (falling back when out of bounds) */
  let titleForBlogAtIndexLensSafe = i =>
    stateBlogsLens
    >>- Pancake.Lens.Array.atOrElse(
          i,
          {id: 0, title: "Fallback Title", body: "Fallback Body"},
        )
    >>- blogTitleLens;

  /* View, Set And, Over for that lens */
  (
    Pancake.Lens.view(titleForBlogAtIndexLensSafe(0), state),
    Pancake.Lens.set(
      titleForBlogAtIndexLensSafe(1),
      "New Title for id 1",
      state,
    ),
    Pancake.Lens.over(
      titleForBlogAtIndexLensSafe(0),
      Js.String.toUpperCase,
      state,
    ),
  )
  |> Js.Json.stringifyAny
  |> Js.log;

  /* View, Set And, Over for that lens, when out of bounds */
  (
    Pancake.Lens.view(titleForBlogAtIndexLensSafe(12), state),
    Pancake.Lens.set(
      titleForBlogAtIndexLensSafe(12),
      "This won't work",
      state,
    ),
    Pancake.Lens.over(
      titleForBlogAtIndexLensSafe(12),
      Js.String.toUpperCase,
      state,
    ),
  )
  |> Js.Json.stringifyAny
  |> Js.log;
};

module Option = {
  /* ------------- */
  /* State Definition */
  /* ------------- */
  [@pancake]
  type author = {name: string};

  [@pancake]
  type blog = {
    id: int,
    title: string,
    body: string,
    author: option(author),
  };

  [@pancake]
  type state = {blogs: array(blog)};

  let blogs = [|
    {id: 0, title: "Title 01", body: "Body 01", author: None},
    {id: 1, title: "Title 02", body: "Body 02", author: None},
    {
      id: 2,
      title: "Title 03",
      body: "Body 03",
      author: Some({name: "roland"}),
    },
  |];

  let state = {blogs: blogs};
  /* ------------- */

  Js.log("\n\n\n---- Options ----");

  /* Create lens for an autor's name, for a blog at a given index, falling back
     if there is no name */
  let stateAuthorNameLens = i =>
    stateBlogsLens
    >>- Pancake.Lens.Array.atExn(i)
    >>- (blogAuthorLens |> Pancake.Lens.Option.orElse({name: "No Author"}))
    >>- authorNameLens;

  /* View, Set And, Over for that lens */
  (
    Pancake.Lens.view(stateAuthorNameLens(0), state),
    Pancake.Lens.set(stateAuthorNameLens(0), "Bar", state),
    Pancake.Lens.over(stateAuthorNameLens(0), Js.String.toUpperCase, state),
  )
  |> Js.Json.stringifyAny
  |> Js.log;

  /* View, Set And, Over for that lens (with an author in place) */
  (
    Pancake.Lens.view(stateAuthorNameLens(2), state),
    Pancake.Lens.set(stateAuthorNameLens(2), "Bar", state),
    Pancake.Lens.over(stateAuthorNameLens(2), Js.String.toUpperCase, state),
  )
  |> Js.Json.stringifyAny
  |> Js.log;
};

module Result = {
  /* ------------- */
  /* State Definition */
  /* ------------- */
  [@pancake]
  type blog = {
    id: int,
    title: string,
    body: string,
  };

  [@pancake]
  type state = {
    blogs: array(blog),
    currentBlog: result(blog, string),
  };

  let currentBlog = Ok({id: 0, title: "Ok Title", body: "body"});

  let state = {blogs: [||], currentBlog};
  /* ------------- */

  Js.log("\n\n\n---- Result ----");

  /* Create lens for a current blog, throwing if it's not there */
  let currentBlogTitleLens =
    stateCurrentBlogLens |> Pancake.Lens.Result.orExn >>- blogTitleLens;

  (
    Pancake.Lens.view(currentBlogTitleLens, state),
    Pancake.Lens.set(currentBlogTitleLens, "New Title", state),
    Pancake.Lens.over(currentBlogTitleLens, Js.String.toUpperCase, state),
  )
  |> Js.Json.stringifyAny
  |> Js.log;

  /* Create lens for a current blog, falling back if it's not there */
  let state = {blogs: [||], currentBlog: Error("Fubar")};
  let currentBlogTitleLensFallback =
    stateCurrentBlogLens
    |> Pancake.Lens.Result.orElse({
         id: 0,
         title: "Fallback Title",
         body: "Fallback Body",
       })
    >>- blogTitleLens;

  (
    Pancake.Lens.view(currentBlogTitleLensFallback, state),
    Pancake.Lens.set(
      currentBlogTitleLensFallback,
      "New Title -- Updated fallback",
      state,
    ),
    Pancake.Lens.over(
      currentBlogTitleLensFallback,
      Js.String.toUpperCase,
      state,
    ),
  )
  |> Js.Json.stringifyAny
  |> Js.log;
};

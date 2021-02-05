open Pancake.Infix;

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
    stateBlogsLens >>- Pancake.List.atExn(i) >>- blogTitleLens;

  /* View, Set And, Over for that lens */
  (
    Pancake.view(titleForBlogAtIndexLens(0), state),
    Pancake.set(titleForBlogAtIndexLens(0), "Foobar", state),
    Pancake.over(titleForBlogAtIndexLens(0), Js.String.toUpperCase, state),
  )
  |> Js.log;

  /* Create lens for a blog at a certain index (falling back when out of bounds) */
  let titleForBlogAtIndexLensSafe = i =>
    stateBlogsLens
    >>- Pancake.List.atOrElse(
          i,
          {id: 0, title: "Fallback Title", body: "Fallback Body"},
        )
    >>- blogTitleLens;

  /* View, Set And, Over for that lens */
  (
    Pancake.view(titleForBlogAtIndexLensSafe(0), state),
    Pancake.set(titleForBlogAtIndexLensSafe(1), "New Title for id 1", state),
    Pancake.over(
      titleForBlogAtIndexLensSafe(0),
      Js.String.toUpperCase,
      state,
    ),
  )
  |> Js.log;

  /* View, Set And, Over for that lens, when out of bounds */
  (
    Pancake.view(titleForBlogAtIndexLensSafe(12), state),
    Pancake.set(titleForBlogAtIndexLensSafe(12), "This won't work", state),
    Pancake.over(
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
    stateBlogsLens >>- Pancake.Array.atExn(i) >>- blogTitleLens;

  /* View, Set And, Over for that lens */
  (
    Pancake.view(titleForBlogAtIndexLens(0), state),
    Pancake.set(titleForBlogAtIndexLens(0), "Foobar", state),
    Pancake.over(titleForBlogAtIndexLens(0), Js.String.toUpperCase, state),
  )
  |> Js.Json.stringifyAny
  |> Js.log;

  /* Create lens for a blog at a certain index (falling back when out of bounds) */
  let titleForBlogAtIndexLensSafe = i =>
    stateBlogsLens
    >>- Pancake.Array.atOrElse(
          i,
          {id: 0, title: "Fallback Title", body: "Fallback Body"},
        )
    >>- blogTitleLens;

  /* View, Set And, Over for that lens */
  (
    Pancake.view(titleForBlogAtIndexLensSafe(0), state),
    Pancake.set(titleForBlogAtIndexLensSafe(1), "New Title for id 1", state),
    Pancake.over(
      titleForBlogAtIndexLensSafe(0),
      Js.String.toUpperCase,
      state,
    ),
  )
  |> Js.Json.stringifyAny
  |> Js.log;

  /* View, Set And, Over for that lens, when out of bounds */
  (
    Pancake.view(titleForBlogAtIndexLensSafe(12), state),
    Pancake.set(titleForBlogAtIndexLensSafe(12), "This won't work", state),
    Pancake.over(
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
  let authorNameLens = i =>
    stateBlogsLens
    >>- Pancake.Array.atExn(i)
    >>- (blogAuthorLens |> Pancake.Option.orElse({name: "No Author"}))
    >>- authorNameLens;

  /* View, Set And, Over for that lens */
  (
    Pancake.view(authorNameLens(0), state),
    Pancake.set(authorNameLens(0), "Bar", state),
    Pancake.over(authorNameLens(0), Js.String.toUpperCase, state),
  )
  |> Js.Json.stringifyAny
  |> Js.log;

  /* View, Set And, Over for that lens (with an author in place) */
  (
    Pancake.view(authorNameLens(2), state),
    Pancake.set(authorNameLens(2), "Bar", state),
    Pancake.over(authorNameLens(2), Js.String.toUpperCase, state),
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
    stateCurrentBlogLens |> Pancake.Result.orExn >>- blogTitleLens;

  (
    Pancake.view(currentBlogTitleLens, state),
    Pancake.set(currentBlogTitleLens, "New Title", state),
    Pancake.over(currentBlogTitleLens, Js.String.toUpperCase, state),
  )
  |> Js.Json.stringifyAny
  |> Js.log;

  /* Create lens for a current blog, falling back if it's not there */
  let state = {blogs: [||], currentBlog: Error("Fubar")};
  let currentBlogTitleLensFallback =
    stateCurrentBlogLens
    |> Pancake.Result.orElse({
         id: 0,
         title: "Fallback Title",
         body: "Fallback Body",
       })
    >>- blogTitleLens;

  (
    Pancake.view(currentBlogTitleLensFallback, state),
    Pancake.set(
      currentBlogTitleLensFallback,
      "New Title -- Updated fallback",
      state,
    ),
    Pancake.over(currentBlogTitleLensFallback, Js.String.toUpperCase, state),
  )
  |> Js.Json.stringifyAny
  |> Js.log;
};

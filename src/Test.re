open Pancake.Infix;

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

  Js.log("---- ARRAYS ----");
  let titleForBlogAtIndexLens = i =>
    stateBlogsLens >>- Pancake.List.atExn(i) >>- blogTitleLens;

  Js.log("\n---- View title for blog at index 0:");
  Pancake.view(titleForBlogAtIndexLens(0), state) |> Js.log;

  Js.log("\n---- Set title for blog at index 0:");
  Pancake.set(titleForBlogAtIndexLens(0), "Foobar", state) |> Js.log;

  let titleForBlogAtIndexLensWithDefault = i =>
    stateBlogsLens
    >>- Pancake.List.atOrElse(
          i,
          {id: 0, title: "Fallback Title", body: "Fallback Body"},
        )
    >>- blogTitleLens;

  Js.log("\n---- View title for blog at index 0:");
  Pancake.view(titleForBlogAtIndexLensWithDefault(0), state) |> Js.log;
  Pancake.set(
    titleForBlogAtIndexLensWithDefault(1),
    "New Title for id 1",
    state,
  )
  |> Js.log;

  Js.log(
    "\n---- View title for blog at index out of bounds (revert to fallback):",
  );
  Pancake.view(titleForBlogAtIndexLensWithDefault(12), state) |> Js.log;
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
  let titleForBlogAtIndexLens = i =>
    stateBlogsLens >>- Pancake.Array.atExn(i) >>- blogTitleLens;

  Js.log("\n---- Uppercase all blog titles:");
  let upperCaseBlog = Pancake.over(blogTitleLens, Js.String.toUpperCase);
  Pancake.over(stateBlogsLens, Js.Array.map(upperCaseBlog), state) |> Js.log;

  Js.log("\n---- View title for blog at index 0:");
  Pancake.view(titleForBlogAtIndexLens(0), state) |> Js.log;

  Js.log("\n---- Set title for blog at index 0:");
  Pancake.set(titleForBlogAtIndexLens(0), "Foobar", state) |> Js.log;

  let titleForBlogAtIndexLensWithDefault = i =>
    stateBlogsLens
    >>- Pancake.Array.atOrElse(
          i,
          {id: 0, title: "Fallback Title", body: "Fallback Body"},
        )
    >>- blogTitleLens;

  Js.log("\n---- View title for blog at index 0:");
  Pancake.view(titleForBlogAtIndexLensWithDefault(0), state) |> Js.log;
  Pancake.set(
    titleForBlogAtIndexLensWithDefault(1),
    "New Title for id 1",
    state,
  )
  |> Js.log;

  Js.log(
    "\n---- View title for blog at index out of bounds (revert to fallback):",
  );
  Pancake.view(titleForBlogAtIndexLensWithDefault(12), state) |> Js.log;
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
  let authorNameLens = i =>
    stateBlogsLens
    >>- Pancake.Array.atExn(i)
    >>- (blogAuthorLens |> Pancake.Option.orElse({name: "No Author"}))
    >>- authorNameLens;

  Js.log("\n---- View author name for blog at index 0 -- no author:");
  Pancake.view(authorNameLens(0), state) |> Js.log;

  Js.log("\n---- Set author name for blog at index 0 -- shoul be bar:");
  Pancake.set(authorNameLens(0), "Bar", state)
  |> Pancake.view(authorNameLens(0))
  |> Js.log;

  Js.log("\n---- View author name for blog at index 2 -- no author:");
  Pancake.view(authorNameLens(2), state) |> Js.log;

  Js.log("\n---- Set author name for blog at index 2 -- shoul be baz:");
  Pancake.set(authorNameLens(2), "Baz", state)
  |> Pancake.view(authorNameLens(2))
  |> Js.log;

  Js.log("\n---- Uppercase author name for blog at index 2 -- ROLAND:");
  Pancake.over(authorNameLens(2), Js.String.toUpperCase, state)
  |> Pancake.view(authorNameLens(2))
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
  let currentBlogTitleLens =
    stateCurrentBlogLens |> Pancake.Result.orExn >>- blogTitleLens;

  Js.log("\n---- View / Update title for current blog within Result:");
  Js.log(Pancake.view(currentBlogTitleLens, state));
  Js.log(Pancake.set(currentBlogTitleLens, "New Title", state));

  let state = {blogs: [||], currentBlog: Error("Fubar")};
  let currentBlogTitleLensFallback =
    stateCurrentBlogLens
    |> Pancake.Result.orElse({
         id: 0,
         title: "Fallback Title",
         body: "Fallback Body",
       })
    >>- blogTitleLens;

  Js.log(
    "\n---- View / Update / Over title for current blog within Result in error state with fallback:",
  );
  Js.log(Pancake.view(currentBlogTitleLensFallback, state));
  Js.log(
    Pancake.set(
      currentBlogTitleLensFallback,
      "Title updated from fallback",
      state,
    ),
  );
  Js.log(
    Pancake.over(currentBlogTitleLensFallback, Js.String.toUpperCase, state),
  );
};

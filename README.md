# :pancakes: - Pancake
Named after the famous 40mm Canon 'pancake' lens, a lens that you'd always want
with you since its so small.

Builds upon:
- [Decco](https://github.com/reasonml-labs/decco)
- [Lenses-PPX](https://github.com/Astrocoders/lenses-ppx)

# Motivation
Internally at [Tenzir](https://tenzir.com/) and also in my side-project 
[Konfig](https://konfig.xyz/), lenses are heavily used to manage Redux state.
At Konfig this is plain JS and done with the help of Ramda lenses, at Tenzir 
this is done in ReasonML and we hand roll our lenses in the following way:

```reason
type lens('a, 'b) = {
  get: 'a => 'b,
  set: ('b, 'a) => 'a,
};
```
This is great. Because with some small helper functions, we can make these 
composeable, and get really powerful functionality. Unfortunately, this 
requires loads of boilerplate as we have to write one for each record entry.
This is cumbersome and in one of the Tenzir Hackathons, we decided to build 
something that fixes it. Similar to [Lenses-PPX](https://github.com/Astrocoders/lenses-ppx)
we wrote a PPX that adds this functionality. Unlike that library though, 
we don't share the sum-type approach, but instead generate lenses in the structure
we're used too. 
Combined with the helper functions defined in this library, you get fully 
typesafe lenses that are very easily composable for free (or well, at the 
cost of an annotation).

# Examples
Let's take the following example:

```reason
[@pancake]
type blog = {
  id: int,
  title: string,
  body: option(string),
};

```
Pancake will now generate all the needed lenses for the blog. So you can do
things like:
```reason
let blog = {id: 0, title: "title", body: Some("body")};

Lens.view(blogIdLens, blog); 
// -> 0
Lens.view(blogTitleLens, blog); 
// -> "title"

Lens.set(blogIdLens, 1, blog) 
// -> {id: 1, title: "title", body: Some("body")}
```

Pancake also gives you some Helper functions for dealing with `lists`, `options`,
`arrays`, and `results`. So you can do things like:

#### Array / List
*Lists and Arrays have pretty much the same syntax*
```reason
[@pancake]
type metric = {speed: array(int)};

let metric = {speed: [|0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10|]};

Lens.view(metricSpeedLens >>- Lens.Array.atOrElse(1, 100), metric) 
// -> 0
Lens.view(metricSpeedLens >>- Lens.Array.atOrElse(-1, 100), metric) 
// -> 10
Lens.view(metricSpeedLens >>- Lens.Array.atOrElse(25, 100), metric) 
// -> 100
// being out of bounds returns the fallback

Lens.set(metricSpeedLens >>- Lens.Array.atOrElse(1, 100), 100, metric)
// -> {speed: [|0, 100, 2, 3, 4, 5, 6, 7, 8, 9, 10|]}
Lens.set(metricSpeedLens >>- Lens.Array.atOrElse(-1, 100), 100, metric)
// -> {speed: [|0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 100|]}
Lens.set(metricSpeedLens >>- Lens.Array.atOrElse(25, 100), 100, metric)
// -> {speed: [|0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 100|]} 
// being out of bounds keeps the original array intact
```

#### Option
```reason
[@pancake]
type metric = {speed: option(int)};
let metric = {speed: Some(10)};

Lens.view(metricSpeedLens >>- Lens.Option.orElse(1000), metric)
// -> 10
let metric = {speed: None};
Lens.view(metricSpeedLens >>- Lens.Option.orElse(1000), metric)
// -> 1000
```

```reason
[@pancake]
type metric = {speed: result(int, string)};
let metric = {speed: Ok(10)};

Lens.view(metricSpeedLens >>- Lens.Result.orElse(1000), metric)
// -> 10
let metric = {speed: Error("Some Error")};
Lens.view(metricSpeedLens >>- Lens.Result.orElse(1000), metric)
// -> 1000
```

#### All the way...
Given some of the above elements, and the ability to compose lenses, it will give
you the ability to do some pretty powerfull stuff...

```reason
[@pancake]
type address = {country: option(string)};

[@pancake]
type company = {parsedAddress: result(address, string)};

[@pancake]
type account = {company: option(company)};

let emptyAccount = {company: None};

Lens.set(
  accountCompanyLens
  >>- Lens.Option.orElse({parsedAddress: Error("No Parsed Address")})
  >>- companyParsedAddressLens
  >>- Lens.Result.orElse({country: None})
  >>- addressCountryLens
  >>- Lens.Option.orElse("Some Fallback Country"),
  "Netherlands",
  accountThree,
);
// -> {company: Some({parsedAddress: Ok({country: Some("Netherlands")})})}
```

For more examples, check the tests.

# Todo:

[ ] - Automatically build Prisms for Sum Types

[ ] - Prism composition

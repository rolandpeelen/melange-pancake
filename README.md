# :pancakes: Pancake
Named after the famous 40mm Canon 'pancake' lens, a lens that you'd always want
with you since its so small.

Heavily builds upon:
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
compose-able, and get really powerfull functionality. Unfortunately, this 
requires loads of boilerplate as we have to write one for each record entry.
This is cumbersome and in one of the Tenzir Hackathons, we decided to build 
something that fixes it. Similar to [Lenses-PPX](https://github.com/Astrocoders/lenses-ppx)
we wrote a PPX that adds this functionality. Unlike that library though, 
we don't share the sum-type approach, but instead generate lenses in the structure
we're used too. 
Combined with the helper functions defined in this library, you get fully 
typesafe lenses that are very easily composable for free (or well, at the 
cost of an annotation).

# Example
Let's take the following example:

```reason
[@lens]
type address = {
    street: string,
    houseNo: int,
    currentlyLivesHere: bool
}

[@lens]
type user = {
    username: string,
    address
}
```

Now Pancake will generate the following lenses for you:
```reason
let addressStreetLens = {
  get: address => address.street,
  set: (street, address) => {...address, street}
}
let addressHouseNoLens = {
  get: address => address.houseNo,
  set: (street, houseNo) => {...address, houseNo}
}
let addressCurrentlyLivesHereLens = {
  get: address => address.currentlyLivesHere,
  set: (street, currentlyLivesHere) => {...address, currentlyLivesHere}
}
let userUsernameLens = {
  get: user => user.username,
  set: (username, user) => {...user, username}
}
let userAddressLens = {
  get: user => user.address,
  set: (address, user) => {...user, address}
}
```

Let's say that given a user, I want to be able to view and update his / her address;
```reason
let userAddressStreetLens = Pancake.pipe(userAddressLens, userStreetLens)

```

Or, with infix notation:
```reason
open Pancake.Infix;
let userAddressStreetLens = userAddressLens >>- userStreetLens
```

Now, there are three main helper functions we can use to to some things.
Given the following user:
```reason
let rolandsAddress = {
  street: "Some Street",
  houseNo: 42,
  currentlyLivesHere: false,
}
let roland = {
  username: "roland",
  address: rolandsAddress
};
```

1. View the users street
```reason
let rolandsStreet = Pancake.view(userAddressStreetLens, roland);
// "Some Street"
```

2. Set the street
```reason
let rolandWithNewStreet = Pancake.set(userAddressStreetLens, "My new street", roland);
// {username: "roland", address: {street: "My new street", houseNo: 42, currentlyLivesHere: false} 
```
Note that this returns a whole new user, with the new street applied.

3. Run functions over that adder
```reason
let rolandWithNewStreet = Pancake.over(userAddressStreetLens, Js.String.toUpperCase, roland);
// {username: "roland", address: {street: "MY NEW STREET", houseNo: 42, currentlyLivesHere: false} 
```

The great thing here is that both `set` and `over` return entirely new objects. So I can
do things like:

```reason
roland
|> Pancake.over(usernameLens, Js.String.toUpperCase)
|> Pancake.set(userAddressStreetLens, "My New Street")
|> Pancake.set(userAddressHouseNoLens, 55)
|> Pancake.over(userCurrentlyLivesHereLens, (liveshere) => true)

// {username: "ROLAND", address: {street: "My new street", houseNo: 42, currentlyLivesHere: true} 
```
Of course, at this point we're pretty much overwriting the whole address, so we 
might opt for another solution. But imagine this is a deeply nested state, in
which you'd have to update things in multiple locations, possibly with the same
data, all in an immutable way...


# Todo:
[ ] - Create GH action that builds for OSX and Linux, like [the decco travis one](https://github.com/reasonml-labs/decco/blob/master/.travis.yml)
[ ] - Handle composition of optional values
[ ] - Create get / set lens for Array, List

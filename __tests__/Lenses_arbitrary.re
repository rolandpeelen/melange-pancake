open Jest;
open Expect;
open Pancake;
open Pancake.Lens.Infix;

[@pancake]
type address = {country: option(string)};

[@pancake]
type company = {parsedAddress: result(address, string)};

[@pancake]
type account = {company: option(company)};

let country = {country: Some("Netherlands")};
let company = {parsedAddress: Ok(country)};
let accountAll = {company: Some(company)};

/* ---------------------------- */
/* Fully Complete */
/* ---------------------------- */
test(
  "AccountAll View",
  Lens.view(
    accountCompanyLens
    >>- Lens.Option.orExn
    >>- companyParsedAddressLens
    >>- Lens.Result.orExn
    >>- addressCountryLens
    >>- Lens.Option.orExn,
    accountAll,
  )
  |> expect
  |> toEqual("Netherlands")
  |> Lib.Function.const,
);

/* ---------------------------- */
/* Missing One */
/* ---------------------------- */
let accountOne = {company: Some({parsedAddress: Ok({country: None})})};
test(
  "AccountOne View",
  Lens.view(
    accountCompanyLens
    >>- Lens.Option.orExn
    >>- companyParsedAddressLens
    >>- Lens.Result.orExn
    >>- addressCountryLens
    >>- Lens.Option.orElse("Some Fallback Country"),
    accountOne,
  )
  |> expect
  |> toEqual("Some Fallback Country")
  |> Lib.Function.const,
);
test(
  "AccountOne Set",
  Lens.set(
    accountCompanyLens
    >>- Lens.Option.orExn
    >>- companyParsedAddressLens
    >>- Lens.Result.orExn
    >>- addressCountryLens
    >>- Lens.Option.orExn,
    "Netherlands",
    accountOne,
  )
  |> expect
  |> toEqual(accountAll)
  |> Lib.Function.const,
);

/* ---------------------------- */
/* Missing two */
/* ---------------------------- */
let accountTwo = {
  company: Some({parsedAddress: Error("ErrorParsingAddress")}),
};

test(
  "AccountTwo View",
  Lens.view(
    accountCompanyLens
    >>- Lens.Option.orExn
    >>- companyParsedAddressLens
    >>- Lens.Result.orElse({country: None})
    >>- addressCountryLens
    >>- Lens.Option.orElse("Some Fallback Country"),
    accountTwo,
  )
  |> expect
  |> toEqual("Some Fallback Country")
  |> Lib.Function.const,
);

test(
  "AccountTwo Set",
  Lens.set(
    accountCompanyLens
    >>- Lens.Option.orExn
    >>- companyParsedAddressLens
    >>- Lens.Result.orElse({country: None})
    >>- addressCountryLens
    >>- Lens.Option.orElse("Some Fallback Country"),
    "Netherlands",
    accountTwo,
  )
  |> expect
  |> toEqual(accountAll)
  |> Lib.Function.const,
);

/* ---------------------------- */
/* Missing All */
/* ---------------------------- */
let accountThree = {company: None};

test(
  "AccountThree View",
  Lens.view(
    accountCompanyLens
    >>- Lens.Option.orElse({parsedAddress: Error("No Parsed Address")})
    >>- companyParsedAddressLens
    >>- Lens.Result.orElse({country: None})
    >>- addressCountryLens
    >>- Lens.Option.orElse("Some Fallback Country"),
    accountThree,
  )
  |> expect
  |> toEqual("Some Fallback Country")
  |> Lib.Function.const,
);

test(
  "AccountThree Set",
  Lens.set(
    accountCompanyLens
    >>- Lens.Option.orElse({parsedAddress: Error("No Parsed Address")})
    >>- companyParsedAddressLens
    >>- Lens.Result.orElse({country: None})
    >>- addressCountryLens
    >>- Lens.Option.orElse("Some Fallback Country"),
    "Netherlands",
    accountThree,
  )
  |> expect
  |> toEqual(accountAll)
  |> Lib.Function.const,
);

[@pancake]
type userAccount = {
  id: int,
  company: option(company),
};

let input = [
  {id: 0, company: None},
  {
    id: 1,
    company: Some({parsedAddress: Ok({country: Some("Netherlands")})}),
  },
  {id: 2, company: None},
];

let output = [
  {id: 0, company: None},
  {
    id: 1,
    company: Some({parsedAddress: Ok({country: Some("NETHERLANDS")})}),
  },
  {id: 2, company: None},
];

test(
  "Account deeply nested",
  Lens.over(
    Lens.List.findByLens(1, userAccountIdLens)
    >>- Lens.Option.orElse({id: (-1), company: None})
    >>- userAccountCompanyLens
    >>- Lens.Option.orElse({parsedAddress: Error("No Parsed Address")})
    >>- companyParsedAddressLens
    >>- Lens.Result.orElse({country: None})
    >>- addressCountryLens
    >>- Lens.Option.orElse("Fallback Country"),
    Js.String.toUpperCase,
    input,
  )
  |> expect
  |> toEqual(output)
  |> Lib.Function.const,
);

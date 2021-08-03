open Function;
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
  |> const,
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
  |> const,
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
  |> const,
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
  |> const,
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
  |> const,
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
  |> const,
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
  |> const,
);

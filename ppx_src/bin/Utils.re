open Parsetree;

let getAttributeByName = (attributes: list(attribute), name) => {
  let filtered =
    attributes |> List.filter((({Location.txt}, _)) => txt == name);

  switch (filtered) {
  | [] => Ok(None)
  | [attribute] => Ok(Some(attribute))
  | _ => Error("Too many occurrences of \"" ++ name ++ "\" attribute")
  };
};

let annotationName = "pancake";
type generatorSettings = {pancake: bool};

let getSettingsFromAttributes = (attributes: list(attribute)) =>
  switch (getAttributeByName(attributes, annotationName)) {
  | Ok(Some(_)) => Ok(Some({pancake: true}))
  | Ok(None) => Ok(None)
  | Error(_) as e => e
  };

let fail = (loc, message) =>
  Location.error(~loc, message) |> (v => Location.Error(v) |> raise);

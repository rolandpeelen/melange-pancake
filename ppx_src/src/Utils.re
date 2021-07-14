open Ppxlib;
open Parsetree;

let annotationName = "pancake";
type generatorSettings = {pancake: bool};

let getAttributeByName = (attributes, name) => {
    let filtered = attributes
        |> List.filter(({attr_name: { Location.txt }}) => txt == name);

    switch filtered {
        | [] => Ok(None)
        | [attribute] => Ok(Some(attribute))
        | _ => Error("Too many occurrences of \"" ++ name ++ "\" attribute")
    };
};

let getSettingsFromAttributes = (attributes: list(attribute)) =>
  switch (getAttributeByName(attributes, annotationName)) {
  | Ok(Some(_)) => Ok(Some({pancake: true}))
  | Ok(None) => Ok(None)
  | Error(_) as e => e
  };

let fail = (loc, message) => Location.raise_errorf(~loc, "%s", message);

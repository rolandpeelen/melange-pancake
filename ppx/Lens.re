open Ppxlib;
open Parsetree;

let loc = Location.none;
let createLenses = (~typeName, ~fields, ()) => {
  List.map(
    field => {
      let setRecord =
        Ast_helper.Exp.record(
          [({loc, txt: Lident(field.pld_name.txt)}, [%expr value])],
          List.length(fields) > 1 ? Some([%expr values]) : None,
        );
      let setter = [%expr (value, values) => [%e setRecord]];

      let getAttr =
        Ast_helper.Exp.field(
          [%expr values],
          {loc, txt: Lident(field.pld_name.txt)},
        );
      let getter = [%expr values => [%e getAttr]];

      let getterAndSetter =
        Ast_helper.Exp.record(
          [
            ({loc, txt: Lident("set")}, setter),
            ({loc, txt: Lident("get")}, getter),
          ],
          None,
        );

      let recordType =
        Ast_helper.Typ.mk(Ptyp_constr({txt: Lident(typeName), loc}, []));

      let fieldType = Ast_helper.Typ.mk(field.pld_type.ptyp_desc);

      let typeDefinition = [%type:
        Pancake.Lens.t([%t recordType], [%t fieldType])
      ];

      let lensName =
        Ast_helper.Pat.var({
          txt:
            typeName ++ String.capitalize_ascii(field.pld_name.txt) ++ "Lens",
          loc,
        });

      let pat = Ast_helper.Pat.constraint_(lensName, typeDefinition);
      [%stri let [%p pat] = [%e getterAndSetter]];
    },
    fields,
  );
};

// Heavily borrowed from Decco's code
module StructureMapper = {
  open Utils;
  let mapTypeDecl = decl => {
    let {
      ptype_attributes,
      ptype_name: {txt: typeName, _},
      ptype_manifest,
      ptype_loc,
      ptype_kind,
      _,
    } = decl;

    switch (getSettingsFromAttributes(ptype_attributes)) {
    | Ok(Some({pancake: true})) =>
      switch (ptype_manifest, ptype_kind) {
      | (None, Ptype_abstract) =>
        fail(ptype_loc, "Can't generate lenses for unspecified type")
      | (None, Ptype_record(fields)) => createLenses(~typeName, ~fields, ())
      | _ => fail(ptype_loc, "This type is not handled by lenses-ppx")
      }
    | Ok(Some({pancake: false}))
    | Ok(None) => []
    | Error(s) => fail(ptype_loc, s)
    };
  };
  let mapStructureItem = (mapper, { pstr_desc } as structureItem) =>
    switch (pstr_desc) {
    | Pstr_type(_recFlag, decls) =>
      let valueBindings = decls |> List.map(mapTypeDecl) |> List.concat;
      [mapper#structure_item(structureItem)]
      @ (List.length(valueBindings) > 0 ? valueBindings : []);

    | _ => [mapper#structure_item(structureItem)]
    };
};

class mapper = {
  as self;
  inherit class Ast_traverse.map;
  pub! structure = structure => {
    structure
    |> List.map(StructureMapper.mapStructureItem(self))
    |> List.concat;
  };
};

let structure_mapper = (_, s) => (new mapper)#structure(s);

Ppxlib.Driver.V2.register_transformation(~impl=structure_mapper, "pancake");

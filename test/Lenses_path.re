open Pancake;

module Table = {
  [@pancake]
  type t = {
    str: string,
    bool,
  };
};

[@pancake]
type db = {table: Table.t};

module QueryBuilder = {
  type t;

  external make: unit => t = "make";
  [@mel.send.pipe: t] external run: unit => 'a = "make";
  [@mel.send.pipe: t] external from': string => t = "from";

  [@mel.send.pipe: t] external eq': (string, 'a) => t = "insert";

  let from = (l: Lens.t(db, 'table), builder: t) => {
    let path = Belt.Option.getExn(l.path);
    builder |> from'(path);
  };
  let eq = (l: Lens.t('table, 'column), c: 'column, builder: t) => {
    let path = Belt.Option.getExn(l.path);
    builder |> eq'(path, c);
  };
};

let query =
  QueryBuilder.make()
  |> QueryBuilder.from(dbTableLens)
  |> QueryBuilder.eq(Table.tStrLens, "some-string")
//  |> QueryBuilder.eq(Table.tBoolLens, "some-string")
  |> QueryBuilder.run();

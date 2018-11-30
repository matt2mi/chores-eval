include Chore;

let chore = json : chore =>
  Json.Decode.{
    id: json |> field("id", string),
    label: json |> field("label", string),
    duration: json |> field("duration", int),
    isMimi: json |> field("isMimi", bool),
  };
let chores = Json.Decode.list(chore);
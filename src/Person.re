let component = ReasonReact.statelessComponent("Person");

let make = (_person, _children) => {
  ...component,
  render: _self => <div> (ReasonReact.string("Nom: ")) </div>,
};
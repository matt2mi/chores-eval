let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self =>
    <div className="container"> <AddChores /> <hr /> <Analysis /> </div>,
};
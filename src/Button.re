let component = ReasonReact.statelessComponent("Button");

let make = (~message, ~action, _children) => {
  ...component,
  render: _self =>
    <button className="btn btn-success" onClick=action>
      (ReasonReact.string(message))
    </button>,
};
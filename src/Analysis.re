/* State declaration */
type state = {labelField: string};

/* Action declaration */
type action =
  | Yop;

/* Component template declaration.
   Needs to be **after** state and action declarations! */
let component = ReasonReact.reducerComponent("Analysis");

let make = _children => {
  ...component,
  initialState: () => {labelField: "dans keep : 4h 13 nov - 1h le 14 nov"},
  /* State transitions */
  reducer: (action, state) =>
    switch (action) {
    | Yop => ReasonReact.Update({labelField: state.labelField ++ "yop"})
    },
  render: self =>
    <div className="row text-center">
      <div className="col-sm-3" />
      <div className="col-sm-6">
        (ReasonReact.string(self.state.labelField))
      </div>
    </div>,
};
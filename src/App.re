type chore = {
  label: string,
  level: int,
};

/* State declaration */
type state = {
  labelField: string,
  chores: list(chore),
};

/* Action declaration */
type action =
  | AddChore
  | ModifyChoreLabelField(string);

/* Component template declaration.
   Needs to be **after** state and action declarations! */
let component = ReasonReact.reducerComponent("app");

let make = _children => {
  ...component,
  initialState: () => {labelField: "", chores: []},
  /* State transitions */
  reducer: (action, state) =>
    switch (action) {
    | AddChore =>
      ReasonReact.Update({
        labelField: "",
        chores: [{label: state.labelField, level: 4}, ...state.chores],
      })
    | ModifyChoreLabelField(label) =>
      ReasonReact.Update({...state, labelField: label})
    },
  render: self => {
    let nb = string_of_int(List.length(self.state.chores));
    <div className="container">
      <div className="row text-center">
        <div className="col-sm-3" />
        <div className="col-sm-6">
          <input
            type_="text"
            className="form-control"
            placeholder="4h"
            value=self.state.labelField
            onChange=(
              event =>
                self.send(
                  ModifyChoreLabelField(
                    ReactEvent.Form.target(event)##value,
                  ),
                )
            )
          />
        </div>
      </div>
      <div className="row text-center">
        <div className="col-sm-3" />
        <div className="col-sm-6">
          <button
            className="btn btn-primary"
            disabled=(String.length(self.state.labelField) < 1)
            onClick=(_event => self.send(AddChore))>
            (ReasonReact.string("Ajouter une corvee"))
          </button>
        </div>
      </div>
      (
        ReasonReact.array(
          Array.of_list(
            List.map(
              chore =>
                <div className="row text-center">
                  <div className="col-sm-3" />
                  <div className="col-sm-6">
                    (ReasonReact.string(chore.label))
                  </div>
                </div>,
              self.state.chores,
            ),
          ),
        )
      )
      <div className="row text-center">
        <div className="col-sm-3" />
        <div className="col-sm-6">
          (ReasonReact.string(nb ++ " corvees"))
        </div>
      </div>
    </div>;
  },
};
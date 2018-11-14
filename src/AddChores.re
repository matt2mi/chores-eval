type chore = {
  label: string,
  duration: int,
  isMimi: bool,
};

/* State declaration */
type state = {
  labelField: string,
  durationField: int,
  chores: list(chore),
};

/* Action declaration */
type action =
  | AddChoreToMimi
  | AddChoreToMatt
  | ModifyChoreLabelField(string)
  | ModifyChoreDurationField(int);

/* Component template declaration.
   Needs to be **after** state and action declarations! */
let component = ReasonReact.reducerComponent("AddChores");

let addChore = (state, isMimi) =>
  ReasonReact.Update({
    labelField: "",
    durationField: 0,
    chores: [
      {label: state.labelField, duration: state.durationField, isMimi},
      ...state.chores,
    ],
  });
let make = _children => {
  ...component,
  initialState: () => {labelField: "", durationField: 0, chores: []},
  /* State transitions */
  reducer: (action, state) =>
    switch (action) {
    | AddChoreToMatt => addChore(state, false)
    | AddChoreToMimi => addChore(state, true)
    | ModifyChoreLabelField(label) =>
      ReasonReact.Update({...state, labelField: label})
    | ModifyChoreDurationField(duration) =>
      ReasonReact.Update({...state, durationField: duration})
    },
  render: self =>
    <div>
      <div className="row text-center">
        <div className="col-sm-3" />
        <div className="col-sm-6">
          <input
            type_="text"
            className="form-control"
            placeholder="nom"
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
        <div className="col-sm-4" />
        <div className="col-sm-2 pt-7">
          (ReasonReact.string("Duree de la corvee :"))
        </div>
        <div className="col-sm-1">
          <input
            type_="number"
            className="form-control"
            min=0
            value=(string_of_int(self.state.durationField))
            onChange=(
              event =>
                self.send(
                  ModifyChoreDurationField(
                    ReactEvent.Form.target(event)##value,
                  ),
                )
            )
          />
        </div>
        <div className="col-sm-1 pt-7">
          (ReasonReact.string("minute(s)"))
        </div>
      </div>
      <div className="row text-center">
        <div className="col-sm-3" />
        <div className="col-sm-6">
          <button
            className="btn btn-primary"
            disabled=(
              String.length(self.state.labelField) < 1
              || self.state.durationField < 1
            )
            onClick=(_event => self.send(AddChoreToMimi))>
            (ReasonReact.string("Ajouter une corvee a mimi"))
          </button>
          <button
            className="btn btn-primary ml-2"
            disabled=(
              String.length(self.state.labelField) < 1
              || self.state.durationField < 1
            )
            onClick=(_event => self.send(AddChoreToMatt))>
            (ReasonReact.string("Ajouter une corvee a matt"))
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
                    (
                      ReasonReact.string(
                        (chore.isMimi ? "Mimi" : "Matt")
                        ++ " a "
                        ++ chore.label
                        ++ " pendant "
                        ++ string_of_int(chore.duration)
                        ++ " min",
                      )
                    )
                  </div>
                </div>,
              self.state.chores,
            ),
          ),
        )
      )
    </div>,
};
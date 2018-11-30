/**
 * TODO: ajouter date exacte
 */
include Chore;

/* State declaration */
type state = {
  labelField: string,
  durationField: string,
  chores: list(chore),
  error: string,
};

/* Action declaration */
type action =
  | AddChoreToMimi
  | AddChoreToMatt
  | ModifyChoreLabelField(string)
  | ModifyChoreDurationField(string)
  | ChoresFetched(list(chore))
  | ChoresFailedToFetch
  | GetChores;

let component = ReasonReact.reducerComponent("AddChores");

/* makes a get request */
let getChores = send =>
  Js.Promise.(
    Fetch.fetch("http://localhost:5000/api/chores")
    |> then_(Fetch.Response.json)
    |> then_(json =>
         json
         |> DecodeChore.chores
         |> (chores => send(ChoresFetched(chores)))
         |> resolve
       )
    |> catch(_err => resolve(send(ChoresFailedToFetch)))
    |> ignore
  );

/* makes a post request with the following json payload { label: "label" } */
let postChore = (label, duration, isMimi, send) => {
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "label", Js.Json.string(label));
  Js.Dict.set(
    payload,
    "duration",
    Js.Json.number(float_of_string(duration)),
  );
  Js.Dict.set(payload, "isMimi", Js.Json.boolean(isMimi));
  Js.Promise.(
    Fetch.fetchWithInit(
      "http://localhost:5000/api/chore",
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=
          Fetch.BodyInit.make(Js.Json.stringify(Js.Json.object_(payload))),
        ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
        (),
      ),
    )
    |> then_(Fetch.Response.json)
    |> then_(json =>
         json |> DecodeChore.chores |> (_chores => send(GetChores)) |> resolve
       )
    |> catch(_err => resolve(send(ChoresFailedToFetch)))
    |> ignore
  );
};

let make = _children => {
  ...component,
  initialState: () => {
    labelField: "",
    durationField: "0",
    chores: [],
    error: "",
  },
  /* State transitions */
  reducer: (action, state) =>
    switch (action) {
    | ModifyChoreLabelField(label) =>
      ReasonReact.Update({...state, labelField: label})
    | ModifyChoreDurationField(duration) =>
      ReasonReact.Update({...state, durationField: duration})
    | GetChores =>
      ReasonReact.UpdateWithSideEffects(
        {...state, labelField: "", durationField: "0"},
        (self => getChores(self.send)),
      )
    | ChoresFetched(chores) => ReasonReact.Update({...state, chores})
    | ChoresFailedToFetch =>
      ReasonReact.Update({...state, error: "error fetching chores"})
    | AddChoreToMimi =>
      ReasonReact.SideEffects(
        (
          self =>
            postChore(
              self.state.labelField,
              self.state.durationField,
              true,
              self.send,
            )
        ),
      )
    | AddChoreToMatt =>
      ReasonReact.SideEffects(
        (
          self =>
            postChore(
              self.state.labelField,
              self.state.durationField,
              false,
              self.send,
            )
        ),
      )
    },
  didMount: self => self.send(GetChores),
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
            value=self.state.durationField
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
        <div className="col-sm-3">
          <button
            className="btn btn-primary"
            disabled=(
              String.length(self.state.labelField) < 1
              || int_of_string(self.state.durationField) < 1
            )
            onClick=(_event => self.send(AddChoreToMimi))>
            (ReasonReact.string("Ajouter une corvee a Mimi"))
          </button>
        </div>
        <div className="col-sm-3">
          <button
            className="btn btn-primary"
            disabled=(
              String.length(self.state.labelField) < 1
              || int_of_string(self.state.durationField) < 1
            )
            onClick=(_event => self.send(AddChoreToMatt))>
            (ReasonReact.string("Ajouter une corvee a Matou"))
          </button>
        </div>
      </div>
      (
        ReasonReact.array(
          Array.of_list(
            List.map(
              chore =>
                <div className="row text-center" key=chore.label>
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
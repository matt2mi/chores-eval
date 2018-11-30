type chore = {
  id: string,
  label: string,
};

type state =
  | Loading
  | Error
  | Loaded(array(chore));

type action =
  | GetChores
  | ChoresFetched(array(chore))
  | ChoresFailedToFetch;

module Decode = {
  let chore = json : chore =>
    Json.Decode.{
      id: json |> field("id", string),
      label: json |> field("label", string),
    };
  let chores = Json.Decode.array(chore);
};

/* Component template declaration.
   Needs to be **after** state and action declarations! */
let component = ReasonReact.reducerComponent("Analysis");

/* makes a get request */
let getChores = send =>
  Js.Promise.(
    Fetch.fetch("http://localhost:5000/api/chores")
    |> then_(Fetch.Response.json)
    |> then_(json =>
         json
         |> Decode.chores
         |> (chores => send(ChoresFetched(chores)))
         |> resolve
       )
    |> catch(_err => resolve(send(ChoresFailedToFetch)))
    |> ignore
  );

/* makes a post request with the following json payload { label: "label" } */
let postChore = (chore, send) => {
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "label", Js.Json.string(chore.label));
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
         json
         |> Decode.chores
         |> (chores => send(ChoresFetched(chores)))
         |> resolve
       )
    |> catch(_err => resolve(send(ChoresFailedToFetch)))
    |> ignore
  );
};

let make = _children => {
  ...component,
  initialState: () => Loading,
  /* State transitions */
  reducer: (action, _state) =>
    switch (action) {
    | GetChores =>
      ReasonReact.UpdateWithSideEffects(
        Loading,
        (self => getChores(self.send)),
      )
    | ChoresFetched(chores) => ReasonReact.Update(Loaded(chores))
    | ChoresFailedToFetch => ReasonReact.Update(Error)
    },
  didMount: self => self.send(GetChores),
  render: self =>
    switch (self.state) {
    | Error => <div> (ReasonReact.string("An error occurred!")) </div>
    | Loading => <div> (ReasonReact.string("Loading...")) </div>
    | Loaded(chores) =>
      <div>
        <h1> (ReasonReact.string("Chores")) </h1>
        <ul>
          (
            ReasonReact.array(
              Array.map(
                chore =>
                  <li key=chore.id> (ReasonReact.string(chore.label)) </li>,
                chores,
              ),
            )
          )
        </ul>
      </div>
    },
};
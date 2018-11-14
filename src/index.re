[%bs.raw {|require('./index.css')|}];

[%bs.raw {|require('../node_modules/bootstrap/dist/css/bootstrap.css')|}];
[%bs.raw
  {|require('../node_modules/bootstrap/dist/css/bootstrap-theme.css')|}
];

[@bs.module "./serviceWorker"]
external register_service_worker : unit => unit = "register";
[@bs.module "./serviceWorker"]
external unregister_service_worker : unit => unit = "unregister";

ReactDOMRe.renderToElementWithId(<App />, "root");

unregister_service_worker();
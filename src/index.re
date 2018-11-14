[%bs.raw {|require('./index.css')|}];

[%bs.raw {|require('../node_modules/bootstrap/dist/css/bootstrap.css')|}];
[%bs.raw
  {|require('../node_modules/bootstrap/dist/css/bootstrap-theme.css')|}
];

ReactDOMRe.renderToElementWithId(<App />, "root");
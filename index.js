import Hapi from "@hapi/hapi";
import routes from "./src/routes.js";

const server = Hapi.server({ host: "localhost", port: 5000 });

server.route(routes);

server.start()
  .then(() => console.log(`Server listening on ${server.info.uri}`));

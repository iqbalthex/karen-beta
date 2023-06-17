import { getAll, getLatest, updateAll, updateLatest } from "./handlers.js";

export default [
  { method: "GET",  path: "/all",    handler: getAll },
  { method: "GET",  path: "/latest", handler: getLatest },
  { method: "POST", path: "/all",    handler: updateAll },
  { method: "POST", path: "/latest", handler: updateLatest },
];

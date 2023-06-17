import { initializeApp } from "@firebase/app";
import { getDatabase, get, ref, set, update } from "@firebase/database";

const app = initializeApp({ databaseURL: "https://karen-meter-beta-default-rtdb.firebaseio.com" });
const db = getDatabase(app);

export { getAll, getLatest, updateAll, updateLatest };


async function getAll() {
  let data;

  try {
    const snapshot = await get(ref(db, "wind/all"));
    data = snapshot.val();
  } catch (err) {
    console.log(err);
    return { error: err.message };
  }

  return JSON.stringify(data);
}


async function getLatest() {
  let data;

  try {
    const snapshot = await get(ref(db, "wind/latest"));
    data = snapshot.val();
  } catch (err) {
    console.log(err);
    return { error: err.message };
  }

  return JSON.stringify(data);
}


function updateAll(request, h) {
  const { datetime, direction, speed } = request.payload;

  const data = {};
  data[`all/${datetime}`] = { direction, speed };

  try {
    update(ref(db, "wind"), data);
  } catch (err) {
    console.log(err);
    return { error: err.message };
  }

  return { message: "Success updating data in wind/all." };
}


function updateLatest(request, h) {
  const { datetime, direction, speed } = request.payload;

  try {
    set(ref(db, "wind/latest"), { datetime, direction, speed });
  } catch (err) {
    console.log(err);
    return { error: err.message };
  }

  return { message: "Success updating data in wind/latest." };
}


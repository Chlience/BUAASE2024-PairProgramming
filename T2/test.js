import assert from "assert";

// Choose proper "import" depending on your PL.
// import { mancalaResult } from "./t2-as/build/release.js";
// import { mancala_result as mancalaResult } from "./t2_rust/pkg/t1_rust.js"
// [Write your own "import" for other PLs.]

import { mancalaResult } from "./load.js";


assert.strictEqual(mancalaResult(1,[11,12],2),30001);
assert.strictEqual(mancalaResult(1,[14],1),20001);

assert.strictEqual(mancalaResult(1, [12, 21, 16, 22, 11, 26, 15, 24, 15, 21, 16, 23, 14, 25, 16, 23, 13, 25, 15, 26, 14, 11, 23, 12, 21, 13, 24, 13, 22, 15, 23, 16, 21, 14, 23, 16, 12, 21, 15, 22, 11, 26, 14, 24, 11, 25], 46), 15011);

console.log("🎉 You have passed all the tests provided.");

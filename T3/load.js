import fs from "fs"

export function mancalaBoard(n, arr, length) {
	const binary = fs.readFileSync('./t3-1-c/t3-1.wasm');
	const module = new WebAssembly.Module(binary);
	const instance = new WebAssembly.Instance(module);

	const memory = instance.exports.memory;
	const ptr = instance.exports.stackAlloc(length * 4);
	const start = ptr >> 2;
	const heap = new Int32Array(memory.buffer);
	for (let i = 0; i < length; ++ i) {
		heap[start + i] = arr[i];
    }

    const ans = instance.exports.stackAlloc(15 * 4);
    const start_ans = ans >> 2;
    const heap_ans = new Int32Array(memory.buffer);

    instance.exports.mancalaBoard(n, ptr, length, ans);

    const resultArray = [];
    for (let i = 0; i < 15; ++i) {
        resultArray.push(heap_ans[start_ans + i]);
    }
    
    return resultArray;
}

export function mancalaOperator(n, status) {
	const binary = fs.readFileSync('./t3-2-c/t3-2.wasm');
	const module = new WebAssembly.Module(binary);
	const instance = new WebAssembly.Instance(module);

	const memory = instance.exports.memory;
	const ptr = instance.exports.stackAlloc(14 * 4);
	const start = ptr >> 2;
	const heap = new Int32Array(memory.buffer);
	for (let i = 0; i < 14; ++ i) {
		heap[start + i] = status[i];
    }
    return instance.exports.mancalaOperator(n, ptr)
}
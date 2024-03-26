import fs from "fs"

export function bocchiShutUp(n, arr, length) {
	const binary = fs.readFileSync('./t1_c/t1.wasm');
	const module = new WebAssembly.Module(binary);
	const instance = new WebAssembly.Instance(module);

	const memory = instance.exports.memory;
	const ptr = instance.exports.stackAlloc(length * 4);
	const start = ptr >> 2;
	const heap = new Int32Array(memory.buffer);
	for (let i = 0; i < length; ++ i) {
		heap[start + i] = arr[i];
	}
	return instance.exports.bocchiShutUp(n, ptr, length);
}

function test() {
	const binary = fs.readFileSync('./t1_c/t1.wasm');
	const module = new WebAssembly.Module(binary);
	const instance = new WebAssembly.Instance(module);
	const memory = instance.exports.memory;

	const ptr = instance.exports.stackAlloc(10 * 4);
	const start = ptr >> 2;
	const heap = new Int32Array(memory.buffer);
	for (let i = 0; i < 10; ++ i) {
		heap[start + i] = i;
	}
	for (let i = 0; i < 10; ++ i) {
		console.log(instance.exports.getArr(ptr, i));	
	}
}

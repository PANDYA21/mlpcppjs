const iterate = require('./build/Release/native.node').iterate;
const learning_rate = 0.1;
const layers = [2, 3, 1];
const input = [1, 0, 1];
const output = [0];
const iterations = 1e3;
const initial_weight = 0.5;
console.log(iterate(learning_rate, input, layers, output, iterations));
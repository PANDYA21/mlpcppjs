const train = require('.');
const learning_rate = 300;
const layers = [2, 3, 2];
const input = [1, 0];
const output = [0, 1];
const iterations = 2e4;
console.log(train(learning_rate, input, layers, output, iterations));
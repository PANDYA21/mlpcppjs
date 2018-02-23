const iterate = require('.');
// const learning_rate = 0.1;
// const input = [1, 0];
// const layers = [2, 2, 2];
// const output = [1];
// const iterations = 1e3;
// const initial_weight = 0.5;
// console.log(iterate(learning_rate, input, layers, output, iterations));

const layers = [2, 3, 1, 5];
console.log(iterate(layers));
const mlp = require('.');
const learning_rate = 0.3;
const layers = [2, 3, 1];
const input = [1, 0];
const output = [1];
const iterations = 2e4;

let trained_weights = mlp.train(learning_rate, input, layers, output, iterations);
console.log(trained_weights);

// for (let i in trained_weights) {
// 	Things[i]
// }

// console.log(mlp.activate(layers, input, trained_weights));

const mlp = require('.');
const learning_rate = 0.3;
const layers = [3, 3, 2];
const input = [[1, 0, 1]];
const output = [[0, 1]];
const iterations = 1e4;

let trained_weights = mlp.train(learning_rate, input, layers, output, iterations);

console.json = function (arg) {
	console.log(JSON.stringify(arg, null, 2));
}

console.json(trained_weights);

// for (let i in trained_weights) {
// 	Things[i]
// }

// console.log(mlp.activate(layers, input, trained_weights));

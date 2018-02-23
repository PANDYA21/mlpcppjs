const mlp = require('.');
const learning_rate = 0.3;
const layers = [2, 3, 1];
// XOR
const input = [
  [0, 0],
  [0, 1],
  [1, 0],
  [1, 1] 
];
const output = [
  [0],
  [1],
  [1],
  [0]
];
const iterations = 1e4;

let op = mlp.train(learning_rate, input, layers, output, iterations);

console.json = function () {
	for (let arg of arguments) {
		console.log(JSON.stringify(arg, null, 2));
	}
}

console.json(op.weights);
for (let i in op.activations)
	console.json(op.activations[i][layers.length - 1][0]);


function sigmoid(x) {
  return 1 / (1 + Math.exp(-x));
}

function activate(inputs, weights) {
  let input_layer = [];
  for (let i in inputs) {
    input_layer.push(sigmoid(inputs[i] * weights[0][i]));
  }
  let hidden_layer = [];
  for (let i in weights[1]) {
    let this_hid = 0;
    for (let j in input_layer) {
      this_hid += input_layer[j] * weights[1][i];
    }
    hidden_layer.push(sigmoid(this_hid));
  }
  let output_layer = [];
  for (let i in weights[2]) {
    let this_op = 0;
    for (let j in hidden_layer) {
      this_op += hidden_layer[j] * weights[2][i];
    }
    output_layer.push(sigmoid(this_op));
  }
  return output_layer;
}

console.log(activate([0,0], op.weights));
console.log(activate([0,1], op.weights));
console.log(activate([1,0], op.weights));
console.log(activate([1,1], op.weights));

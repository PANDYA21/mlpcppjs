// addon.cc
#include <node.h>
#include <math.h>

namespace demo {

using namespace v8; 


// activation function
double sigmoid(double x) {
  return 1 / (1 + exp(-x));
}

// differentiation of the activation function
double dsigmoid(double x) {
  return exp(-x) * pow (1 + exp(-x), -2);
}

// method to calculate gredient descent
double gradientDescent(double learning_rate, double target, double produced, double current_weight) {
  return 1 * learning_rate * ((target - produced) * dsigmoid(current_weight)) * produced;
}

// metho to calculate activation for a node
double activate(v8::Local<v8::Array> input, double weight) {
  double ans = 0;
  for (int i = 0; i < (input->Length()); ++i) {
    ans = ans + (input->Get(i)->NumberValue() * weight);
  }
  return sigmoid(ans);
}

// metho to calculate backpropagation for a node
double backpropagate(double learning_rate, v8::Local<v8::Array> input, double output, double iterations, double value) {
  for (int iter = 0; iter < iterations; ++iter) {
    double op = activate(input, value);
    value = value - gradientDescent(learning_rate, output, op, value);
  }
  return value;
}

// train a node over iterations
void iterate(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  // Check the number of arguments passed.
  if (args.Length() < 4) {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  // parse arguments
  double learning_rate = args[0]->NumberValue();
  Local<Array> input_array = Local<Array>::Cast(args[1]); 
  double output = args[2]->NumberValue();
  double iterations = args[3]->NumberValue();
  double value = 0.5;

  value = backpropagate(learning_rate, input_array, output, iterations, value);

  // export
  Local<Number> num = Number::New(isolate, value);

  // Set the return weight (using the passed in
  // FunctionCallbackInfo<Value>&)
  args.GetReturnValue().Set(num);
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "iterate", iterate);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace demo
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


// feed-forward the network for given settings and inputs
double** activate(v8::Local<v8::Array> input, v8::Local<v8::Array> layers, double** weights) {
  double** ans = 0;
  for (unsigned int i = 0; i < (layers->Length()); ++i) {
    for (unsigned int j = 0; j < (layers->Get(i)->NumberValue()); ++j) {
      if (i == 0) {
        ans[i][j] = sigmoid(input->Get(i)->NumberValue() * weights[i][j]); 
      } else {
        ans[i][j] = 0;
        for (unsigned int k = 0; k < input->Length(); ++k) {
          ans[i][j] = ans[i][j] + (ans[i-1][j] * weights[i][j]);
        }
        ans[i][j] = sigmoid(ans[i][j]);
      }
    }
  }
  return ans;
}

// // method to calculate backpropagation for a weight / connection
// double backpropagateOneWeight(double learning_rate, v8::Local<v8::Array> input, double target, double produced, double weight) {
//   return weight - gradientDescent(learning_rate, target, produced, weight);
// }

// initialize weights
double** initWeights(v8::Local<v8::Array> layers) {
  double *ans[3];

  for (unsigned int i = 0; i < 3; ++i) {
    ans[i] = double *[100];
    for (unsigned int j = 0; j < (layers->Get(i)->NumberValue()); ++j) {
      ans[i][j] = &(0.5); // initial weight
    }
  }
  return ans;
}

// one iteration
double** iterateOnce(double learning_rate, v8::Local<v8::Array> input, v8::Local<v8::Array> layers, double target, double** weights) {
  double** produced = activate(input, layers, weights);
  return produced;
  // return  backpropagateOneWeight(learning_rate, input, target, produced, weight);
}

// void packDoublesToObject(v8::Isolate* isolate, v8::Local<Array> & target, unsigned int index, double & result) {
//   target->Set(String::NewFromUtf8(isolate, index), Number::New(isolate, result));
// }

// void packObjectsToArray(v8::Isolate* isolate, v8::Local<Array> & target, unsigned int index, v8::Local<Array> & result) {
//   target->Set(String::NewFromUtf8(isolate, index), Array::New(isolate, result));
// }



// train a node over iterations
void iterate(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  // Check the number of arguments passed.
  if (args.Length() < 5) {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  // parse arguments
  double learning_rate = args[0]->NumberValue();
  Local<Array> input_array = Local<Array>::Cast(args[1]);
  Local<Array> layers_array = Local<Array>::Cast(args[2]);
  double output = args[3]->NumberValue();
  double iterations = args[4]->NumberValue();
  double value = 0.5;

  // value = backpropagate(learning_rate, input_array, output, iterations, value);
  double** ans[5][100];
  // for (unsigned int i = 0; i < iterations; ++i) {
    // if (i == 0) {
    //   ans = iterateOnce(learning_rate, input_array, layers_array, output, initWeights(layers_array));
    // }
    // ans = iterateOnce(learning_rate, input_array, layers_array, output, ans);
    ans = initWeights(layers_array);
  // }

  // export
  Local<Array> result = Array::New(isolate);
  for (unsigned int i = 0; i < layers_array->Length(); ++i) {
    Local<Array> this_result = Array::New(isolate);
    for (unsigned int j = 0; j < (layers->Get(i)->NumberValue()); ++j) {
      this_result->Set(j, Number::New(**ans[i][j]));
    }
    result->Set(i, Array::New(isolate, this_result));
  }

  // Set the return weight (using the passed in FunctionCallbackInfo<Value>&)
  args.GetReturnValue().Set(result);
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "iterate", iterate);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace demo
// addon.cc
#include <node.h>
#include <math.h>
#include <string>
#include <sstream>
#include <iostream>

namespace demo {

using namespace v8; 

double sigmoid(double x) {
  return 1 / (1 + exp(-x));
}

double **getWeights(v8::Local<v8::Array> layers) {
  double **ans = 0;
  unsigned int length = layers->Length();
  ans = new double *[length];
  for (unsigned int i = 0; i < length; ++i)
  {
    unsigned int this_length = layers->Get(i)->NumberValue();
    ans[i] = new double [this_length];
    for (unsigned int j = 0; j < this_length; ++j)
    {
      ans[i][j] = 0.5;
    }
  }
  return ans;
}

double **activate(v8::Local<v8::Array> layers, v8::Local<v8::Array> input, double **weights) {
  double **activations = 0;
  unsigned int length = layers->Length();
  activations = new double *[length];
  for (unsigned int i = 0; i < length; ++i)
  {
    unsigned int this_length = layers->Get(i)->NumberValue();
    activations[i] = new double [this_length];
    for (unsigned int j = 0; j < this_length; ++j)
    {
      if (i == 0)
      {
        activations[i][j] = 0;
        for (unsigned int k = 0; k < input->Length(); ++k)
        {
          activations[i][j] = activations[i][j] + (input->Get(k)->NumberValue() * weights[i][j]);
        }
        activations[i][j] = sigmoid(activations[i][j]);
      } else 
      {
        activations[i][j] = 0;
        for (unsigned int k = 0; k < layers->Get(i-1)->NumberValue(); ++k)
        {
          activations[i][j] = activations[i][j] + (activations[i-1][k] * weights[i][j]);
        }
        activations[i][j] = sigmoid(activations[i][j]);
      }
    }
  }

  return activations;
}

double dsigmoid(double x) {
  return 0 - exp(-x) * pow (1 + exp(-x), -2);
}

double gradientDescent(double learning_rate, double target, double produced, double current_weight) {
  return 0 - (1 * learning_rate * ((target - produced) * dsigmoid(current_weight)) * produced);
}

double **learn(double learning_rate, v8::Local<v8::Array> layers, v8::Local<v8::Array> input, v8::Local<v8::Array> output, double **weights) {
  double **activations = activate(layers, input, weights);

  unsigned int this_length = layers->Get(layers->Length() - 1)->NumberValue();
  for (unsigned int i = 0; i < this_length; ++i)
  {
    weights[layers->Length() - 1][i] = weights[layers->Length() - 1][i] + gradientDescent(learning_rate, output->Get(i)->NumberValue(), activations[this_length][i], weights[layers->Length() - 1][i]);
  }
  
  return weights;
}

double **iterate(double learning_rate, v8::Local<v8::Array> layers, v8::Local<v8::Array> input, v8::Local<v8::Array> output, unsigned int iterations) {
  double **weights = getWeights(layers);
  for (unsigned int i = 0; i < iterations; ++i)
  {
    weights = learn(learning_rate, layers, input, output, weights);
  }

  return activate(layers, input, weights);
}


void train(const FunctionCallbackInfo<Value>& args) {
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
  Local<Array> output_array = Local<Array>::Cast(args[3]);
  unsigned int iterations = args[4]->NumberValue();

  double **ans = iterate(learning_rate, layers_array, input_array, output_array, iterations);

  // export
  Local<Array> finans = Array::New(isolate);
  for (unsigned int i = 0; i < layers_array->Length(); ++i)
  {
    Local<Array> this_ans = Array::New(isolate);
    for (unsigned int j = 0; j < layers_array->Get(i)->NumberValue(); ++j)
    {
      this_ans->Set(j, Number::New(isolate, ans[i][j]));
    }
    finans->Set(i, this_ans);
  }

  // Set the return weight (using the passed in FunctionCallbackInfo<Value>&)
  args.GetReturnValue().Set(finans);
}

double *convertToCArray(v8::Local<v8::Array> arr) {
  double *carr = 0;
  unsigned int length = arr->Length();
  carr = new double [length];
  for (unsigned int i = 0; i < length; ++i)
  {
    carr[i] = arr->Get(i)->NumberValue();
  }

  return carr;
}

double **convert2DToCArray(v8::Local<v8::Array> arr) {
  double **carr = 0;
  unsigned int length = arr->Length();
  carr = new double *[length];
  for (unsigned int i = 0; i < length; ++i)
  {
    Local<Array> this_arr = Local<Array>::Cast(arr->Get(i));
    unsigned int this_length = this_arr->Length();
    carr[i] = new double [this_length];
    carr[i] = convertToCArray(this_arr);
  }

  return carr;
}

void activateExport(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  // Check the number of arguments passed.
  if (args.Length() < 3) {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  // parse arguments
  Local<Array> layers_array = Local<Array>::Cast(args[0]);
  Local<Array> input_array = Local<Array>::Cast(args[1]);
  Local<Array> weights_array = Local<Array>::Cast(args[2]);

  double **ans = activate(layers_array, input_array, convert2DToCArray(weights_array));

  // export
  // Local<Number> ans = Number::New(isolate, ans[2][0]);
  Local<Object> finans2 = Object::New(isolate);
  unsigned int index = layers_array->Get(0)->NumberValue();
  finans2->Set(String::NewFromUtf8(isolate, "prediction"), Number::New(isolate, ans[index][0]));

  // Set the return weight (using the passed in FunctionCallbackInfo<Value>&)
  args.GetReturnValue().Set(finans2);
}


void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "train", train);
  NODE_SET_METHOD(exports, "activate", activateExport);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace demo`
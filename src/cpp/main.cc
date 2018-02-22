// addon.cc
#include <node.h>
#include <math.h>
#include "gradientdescent.h"
#include "mlp.h"
#include "typecastings.h"
#include <string>
#include <sstream>
#include <iostream>

namespace demo {

using namespace v8; 


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

  double **trained_weights = iterate(learning_rate, layers_array, input_array, output_array, iterations);

  // export
  Local<Array> weights = cpp2DArrayToJs2DArray(isolate, layers_array, trained_weights);
  Local<Array> activations = cpp2DArrayToJs2DArray(isolate, layers_array, activate(layers_array, input_array, trained_weights));
  Local<Object> finans = Object::New(isolate);
  finans->Set(String::NewFromUtf8(isolate, "weights"), weights);
  finans->Set(String::NewFromUtf8(isolate, "activations"), activations);

  // Set the return weight (using the passed in FunctionCallbackInfo<Value>&)
  args.GetReturnValue().Set(finans);
}


void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "train", train);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace demo`
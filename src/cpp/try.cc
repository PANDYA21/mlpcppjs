// addon.cc
#include <node.h>
#include <math.h>

namespace demo {

using namespace v8; 

// double *getWeight(v8::Local<v8::Array> layers) {
//   double *ans = 0;
//   ans = new double [layers->Length()];
//   for (unsigned int i = 0; i < layers->Length(); ++i)
//   {
//     ans[i] = layers->Get(i)->NumberValue();
//   }
//   return ans;
// }

double **getWeight(v8::Local<v8::Array> layers) {
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
  // double learning_rate = args[0]->NumberValue();
  // Local<Array> input_array = Local<Array>::Cast(args[1]);
  Local<Array> layers_array = Local<Array>::Cast(args[2]);
  // double output = args[3]->NumberValue();
  // double iterations = args[4]->NumberValue();
  // double value = 0.5;

  double **ans = getWeight(layers_array);

  // export
  Local<Number> num = Number::New(isolate, ans[1][1]);

  // Set the return weight (using the passed in FunctionCallbackInfo<Value>&)
  args.GetReturnValue().Set(num);
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "iterate", iterate);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace demo
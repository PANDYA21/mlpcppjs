// addon.cc
#include <node.h>
#include <math.h>

namespace demo {

using namespace v8; 

double sigmoid(double x) {
  return 1 / (1 + exp(-x));
}

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

double **activate(v8::Local<v8::Array> layers, v8::Local<v8::Array> input) {
  double **weights = getWeight(layers);
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
  Local<Array> input_array = Local<Array>::Cast(args[1]);
  Local<Array> layers_array = Local<Array>::Cast(args[2]);
  // double output = args[3]->NumberValue();
  // double iterations = args[4]->NumberValue();
  // double value = 0.5;

  double **ans = activate(layers_array, input_array);

  // export
  // Local<Number> ans = Number::New(isolate, ans[2][0]);
  Local<Object> finans = Object::New(isolate);
  finans->Set(String::NewFromUtf8(isolate, "01"), Number::New(isolate, ans[0][1]));
  finans->Set(String::NewFromUtf8(isolate, "00"), Number::New(isolate, ans[0][0]));
  finans->Set(String::NewFromUtf8(isolate, "10"), Number::New(isolate, ans[1][0]));
  finans->Set(String::NewFromUtf8(isolate, "11"), Number::New(isolate, ans[1][1]));
  finans->Set(String::NewFromUtf8(isolate, "12"), Number::New(isolate, ans[1][2]));
  finans->Set(String::NewFromUtf8(isolate, "20"), Number::New(isolate, ans[2][0]));


  // Set the return weight (using the passed in FunctionCallbackInfo<Value>&)
  args.GetReturnValue().Set(finans);
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "iterate", iterate);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace demo`
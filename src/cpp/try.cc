// addon.cc
#include <node.h>
#include <math.h>

namespace demo {

using namespace v8; 

// train a node over iterations
void iterate(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // double val[2][5] = {
  //   {0.1, 0.2, 0.3, 0.4, 0.5},
  //   {0.6, 0.7, 0.8}
  // };
  // double **ptr[2][5];
  // for (int i = 0; i < 2; ++i)
  // {
  //   for (int j = 0; j < 5; ++j)
  //   {
  //     ptr[i][j] = &val[i][j];
  //   }
  // }

  // // export
  // Local<Array> result = Array::New(isolate);
  // for (unsigned int i = 0; i < 2; ++i) {
  //   Local<Array> this_result = Array::New(isolate);
  //   for (unsigned int j = 0; j < 5; ++j) {
  //     this_result->Set(j, Number::New(isolate, *ptr[i][j]));
  //   }
  //   result->Set(i, this_result);
  // }

  // double val[3] = {0.1, 0.2, 0.3};
  // double *ptr[3];
  // for (int i = 0; i < 3; ++i)
  // {
  //   ptr[i] = &val[i];
  // }

  // // export
  // Local<Array> result = Array::New(isolate);
  // for (unsigned int i = 0; i < 3; ++i) {
  //   // result->Set(i, Number::New(isolate, val[i])); // works perfectly fine
  //   result->Set(i, Number::New(isolate, *ptr[i])); // also works!
  // }

  double val[2][3] = {
    {0.4, 0.5, 0.6},
    {0.1, 0.2, 0.3}
  };
  double *ptr[2][3];
  for (int i = 0; i < 2; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
      ptr[i][j] = &val[i][j];
    }
  }

  // export
  // Local<Array> result = Array::New(isolate);
  // for (unsigned int i = 0; i < 2; ++i) {
  //   Local<Array> this_result = Array::New(isolate);
  //   for (unsigned int j = 0; j < 3; ++i) {
  //     this_result->Set(j, Number::New(isolate, val[i][j])); // works?
  //     // this_result->Set(j, Number::New(isolate, *ptr[i][j])); // also works!
  //   }
  //   result->Set(i, this_result);
  // }

  // Local<Number> result = Number::New(isolate, val[1][1]);
  Local<Number> result = Number::New(isolate, *ptr[1][1]);

  // return
  args.GetReturnValue().Set(result);
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "iterate", iterate);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace demo
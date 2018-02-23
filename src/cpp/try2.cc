// addon.cc
#include <node.h>
#include <math.h>

namespace demo {

using namespace v8; 

double *getLayers(v8::Local<v8::Array> layers) {
	double *ans;

	for (unsigned int i = 0; i < layers->Length(); ++i) {
		ans[i] = layers->Get(i)->NumberValue();
	}

	return ans;
}

double **initWeights(v8::Local<v8::Array> layers) {
  double **ans;
  double initial_weight = 0.5;
  double *layers_c = getLayers(layers);
  const unsigned int n_layers = sizeof layers_c;

  for (unsigned int i = 0; i < n_layers; ++i) {
  	ans[i] = new double [10];
    for (unsigned int j = 0; j < layers_c[i]; ++j) {
      // ans[i][j] = &initial_weight; // initial weight
      ans[i][j] = initial_weight; // initial weight
    }
  }

  return ans;
}

// v8::Local<v8::Array> assignResults(v8::Isolate* isolate, double **ans, v8::Local<v8::Array> layers) {
// 	Local<Array> result = Array::New(isolate);

// 	for (unsigned int i = 0; i < layers->Length(); ++i) {
// 		Local<Array> this_result = Array::New(isolate);
// 		for (unsigned int j = 0; j < (layers->Get(i)->NumberValue()); ++j) {
//       this_result->Set(j, Number::New(isolate, ans[i][j]));
//     }
//     result->Set(i, this_result);
// 	}

// 	return result;
// }



// train a node over iterations
void iterate(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Array> layers = Local<Array>::Cast(args[1]);

  double **ans = initWeights(layers);

  // Local<Array> result = assignResults(isolate, ans, layers);
  Local<Array> result = Array::New(isolate);

	for (unsigned int i = 0; i < layers->Length(); ++i) {
		Local<Array> this_result = Array::New(isolate);
		for (unsigned int j = 0; j < (layers->Get(i)->NumberValue()); ++j) {
      this_result->Set(j, Number::New(isolate, ans[i][j]));
    }
    result->Set(i, this_result);
	}

  // return
  args.GetReturnValue().Set(result);
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "iterate", iterate);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace demo
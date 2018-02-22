#ifndef TYPECASTING_H_INCLUDED
#define TYPECASTING_H_INCLUDED
#include <node.h>
using namespace v8; 

double *jsArrayToCppArray(v8::Local<v8::Array> arr) {
  double *carr = 0;
  unsigned int length = arr->Length();
  carr = new double [length];
  for (unsigned int i = 0; i < length; ++i)
  {
    carr[i] = arr->Get(i)->NumberValue();
  }

  return carr;
}

double **jsArray2DTo2DCppArray(v8::Local<v8::Array> arr) {
  double **carr = 0;
  unsigned int length = arr->Length();
  carr = new double *[length];
  for (unsigned int i = 0; i < length; ++i)
  {
    v8::Local<v8::Array> this_arr = v8::Local<v8::Array>::Cast(arr->Get(i));
    unsigned int this_length = this_arr->Length();
    carr[i] = new double [this_length];
    carr[i] = jsArrayToCppArray(this_arr);
  }

  return carr;
}

v8::Local<v8::Array> cpp2DArrayToJs2DArray(Isolate *isolate, v8::Local<v8::Array> layers_array, double **ans) {
  v8::Local<v8::Array> finans = v8::Array::New(isolate);
  for (unsigned int i = 0; i < layers_array->Length(); ++i)
  {
    v8::Local<v8::Array> this_ans = v8::Array::New(isolate);
    for (unsigned int j = 0; j < layers_array->Get(i)->NumberValue(); ++j)
    {
      this_ans->Set(j, Number::New(isolate, ans[i][j]));
    }
    finans->Set(i, this_ans);
  }

  return finans;
}

v8::Local<v8::Array> cpp3DArrayToJs3DArray(Isolate *isolate, v8::Local<v8::Array> layers_array, v8::Local<v8::Array> array_of_inputs, double ***ans) {
  v8::Local<v8::Array> finans = v8::Array::New(isolate);
  for (unsigned int k = 0; k < array_of_inputs->Length(); ++k)
  {
    v8::Local<v8::Array> this_output = v8::Array::New(isolate);
    for (unsigned int i = 0; i < layers_array->Length(); ++i)
    {
      v8::Local<v8::Array> this_ans = v8::Array::New(isolate);
      for (unsigned int j = 0; j < layers_array->Get(i)->NumberValue(); ++j)
      {
        this_ans->Set(j, Number::New(isolate, ans[k][i][j]));
      }
      this_output->Set(i, this_ans);
    }
    finans->Set(k, this_output);
  }

  return finans;
}

#endif
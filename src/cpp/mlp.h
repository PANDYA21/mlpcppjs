#ifndef MLP_H_INCLUDED
#define MLP_H_INCLUDED

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

double ***activateMulti(v8::Local<v8::Array> layers, v8::Local<v8::Array> array_of_inputs, double **weights) {
	double ***activations = 0;
	unsigned int length = array_of_inputs->Length();
  activations = new double **[length];
  for (unsigned int i = 0; i < length; ++i)
  {
  	unsigned int this_length = array_of_inputs->Get(i)->NumberValue();
    activations[i] = new double *[this_length];
    v8::Local<v8::Array> this_input = v8::Local<v8::Array>::Cast(array_of_inputs->Get(i));
    activations[i] = activate(layers, this_input, weights);
  }

  return activations;
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

double **iterate(double learning_rate, v8::Local<v8::Array> layers, v8::Local<v8::Array> array_of_inputs, v8::Local<v8::Array> array_of_outputs, unsigned int iterations) {
  double **weights = getWeights(layers);
  for (unsigned int i = 0; i < iterations; ++i)
  {
  	for (unsigned int j = 0; j < array_of_inputs->Length(); ++j)
  	{
  		v8::Local<v8::Array> this_input = v8::Local<v8::Array>::Cast(array_of_inputs->Get(j));
  		v8::Local<v8::Array> this_output = v8::Local<v8::Array>::Cast(array_of_outputs->Get(j));
  		weights = learn(learning_rate, layers, this_input, this_output, weights);
  	}
  }

  return weights;
}

#endif
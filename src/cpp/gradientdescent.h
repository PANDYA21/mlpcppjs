#ifndef GRADIENTDESCENT_H_INCLUDED
#define GRADIENTDESCENT_H_INCLUDED

double sumMultipliedArrays(double *arr1, double *arr2) {
	double ans = 0;
	for (unsigned int i = 0; i < sizeof arr1; ++i)
	{
		ans = ans + (arr1[i] * arr2[i]);
	}

	return ans;
}

double sigmoid(double x) {
  return 1 / (1 + exp(-x));
}

double dsigmoid(double x) {
	return sigmoid(x) / (1 - sigmoid(x));
}

double gradientDescent(double learning_rate, double target, double produced, double current_weight) {
  double del = (target - produced) * dsigmoid(current_weight);
  return learning_rate * del * produced;
}

double gradientDescentHidden(double learning_rate, double produced, double current_weight, double *dels_of_output_layer, double *output_weights) {
  double del = 0 - dsigmoid(current_weight) * sumMultipliedArrays(dels_of_output_layer, output_weights);
  return learning_rate * del * produced;
}

#endif
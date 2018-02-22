#ifndef GRADIENTDESCENT_H_INCLUDED
#define GRADIENTDESCENT_H_INCLUDED

double sigmoid(double x) {
  return 1 / (1 + exp(-x));
}

double dsigmoid(double x) {
  return 0 - exp(-x) * pow (1 + exp(-x), -2);
}

double gradientDescent(double learning_rate, double target, double produced, double current_weight) {
  return 0 - (1 * learning_rate * ((target - produced) * dsigmoid(current_weight)) * produced);
}

#endif
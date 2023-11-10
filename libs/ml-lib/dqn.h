#ifndef DQN_H
#define DQN_H

#include "ml-lib_global.h"
// #include <torch/torch.h>

namespace ml {

class MLLIB_EXPORT DQN /*: torch::nn::Module*/ {
public:
  DQN(int inChannels, int outputDim);

  /* torch::Tensor forward(torch::Tensor x);

  torch::nn::Conv2d l1{nullptr};
  torch::nn::Conv2d l2{nullptr};
  torch::nn::Conv2d l3{nullptr};
  torch::nn::Linear l4{nullptr};
  torch::nn::Linear outputLayer{nullptr};
  */
};

} // namespace ml
#endif // DQN_H

#include "dqn.h"

namespace ml {

DQN::DQN(int inChannels, int outputDim)
{
    l1 = register_module("l1", torch::nn::Conv2d(torch::nn::Conv2dOptions(inChannels, 64, 8).stride(4)));
    l2 = register_module("l2", torch::nn::Conv2d(torch::nn::Conv2dOptions(32, 64, 4).stride(2)));
    l3 = register_module("l3", torch::nn::Conv2d(torch::nn::Conv2dOptions(64, 64, 3).stride(1)));
    l4 = register_module("l4", torch::nn::Linear(3136, 512));
    outputLayer = register_module("outputLayer", torch::nn::Linear(512, outputDim));
}

// https://pytorch.org/cppdocs/frontend.html#end-to-end-example
// input -> (conv2d + relu) x 3 -> flatten -> (dense + relu) x 2 -> output
at::Tensor DQN::forward(at::Tensor x)
{
    x = l1->forward(x);
    x = torch::relu(x);
    x = l2->forward(x);
    x = torch::relu(x);
    x = l3->forward(x);
    x = torch::flatten(x);
    x = l4->forward(x);
    x = torch::relu(x);
    x = outputLayer->forward(x);

    return x;
}

}

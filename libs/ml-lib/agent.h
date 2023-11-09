#ifndef AGENT_H
#define AGENT_H

#include "ml-lib_global.h"
#include "dqn.h"
#include <memory>

namespace ml {

struct MLLIB_EXPORT Hyperparameters {
    double explorationRate = 0.5;
    double explorationRateDecay = 0.99999975;
    double explorationRateMin = 0.01;

    int batchSize = 32;
    double gamma = 0.9;
    int burnin = 1e4; // min. experiences before training
    int learnEvery = 3; // no. of experiences between updates to Q_online
    double syncEvery = 1e4; // no. of experiences between Q_target & Q_online
};

class MLLIB_EXPORT Agent
{
public:
    Agent(std::shared_ptr<DQN> net);
    int act(std::string environmentState);
    void learn();
    std::string recall();
    void save();
    void cache();
    void updateQOnline(std::string tdEstimate, std::string tdTarget);
    void syncQTarget();

    Hyperparameters pars{};

private:
    std::shared_ptr<DQN> net_;
};

}

#endif // AGENT_H

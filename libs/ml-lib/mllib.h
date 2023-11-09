#ifndef MLLIB_H
#define MLLIB_H

#include "ml-lib_global.h"
#include <string>

namespace ml {

class MLLIB_EXPORT MLLib
{
public:
    MLLib();
    std::string getTestStringFromTorch();
};

}
#endif // MLLIB_H

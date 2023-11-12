#include "doodleenv.h"

DoodleEnv::DoodleEnv() {}

void DoodleEnv::make() { view_ = std::make_unique<doodlelib::View>(); }

void DoodleEnv::reset() {
  view_.reset();
  this->make();
}

std::tuple<Screen *, double, bool> DoodleEnv::step(Actions action) {}

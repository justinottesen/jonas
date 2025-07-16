#pragma once

#include <common/singleton.hpp>

namespace jonas {

// TODO: Actually load configurable values
class Config : public Singleton<Config> {
public:
    DEFAULT_CTOR_DTOR(Config);
    DELETE_COPY_MOVE(Config);

    auto localRoot() noexcept -> std::string { return "~/.jonas/data"; }
};

}

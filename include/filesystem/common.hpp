#pragma once

#include <common/macros.hpp>

namespace jonas::fs {

struct Stat {
    explicit Stat(const struct stat& statBuf) { UNUSED(statBuf); }
};

}    // namespace jonas::fs

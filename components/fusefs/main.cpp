#include <fuse3/fuse.h>

#include <common/logger.hpp>
#include "fusefs.hpp"

auto main(int argc, char* argv[]) -> int {
    FuseFS fuse;
    return fuse.main(argc, argv);
}

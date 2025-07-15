#include <fuse3/fuse.h>

#include <common/logger.hpp>
#include <filesystem/posix_filesystem.hpp>

#include "fusefs.hpp"

auto main(int argc, char* argv[]) -> int {
    jonas::fusefs::FuseFS<jonas::fs::PosixFS> fuse;
    return fuse.main(argc, argv);
}

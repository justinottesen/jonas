#include "fusefs.hpp"

auto FuseFS::main(int argc, char* argv[]) -> int {
    return fuse_main(argc, argv, &m_fuseOps, this);
}

auto FuseFS::get() -> FuseFS& {
    return *static_cast<FuseFS*>(fuse_get_context()->private_data);
}

auto FuseFS::getattr(const char* path, struct stat* stat, struct fuse_file_info* fileInfo) -> int {
    UNUSED(path);
    UNUSED(stat);
    UNUSED(fileInfo);
    return 0;
}

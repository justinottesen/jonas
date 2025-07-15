#pragma once

#include <fuse3/fuse.h>

#include <common/macros.hpp>
#include <filesystem/filesystem.hpp>

namespace jonas::fusefs {

template <fs::FileSystemT FS> class FuseFS {
public:
    DEFAULT_CTOR_DTOR(FuseFS);
    DELETE_COPY_MOVE(FuseFS);

    auto main(int argc, char* argv[]) -> int;

    static auto get() -> FuseFS& { return *static_cast<FuseFS*>(fuse_get_context()->private_data); }

private:
    template <auto Method>
    static auto callBridge(auto... args) -> int {
        return (get().*Method)(args...);
    }

    auto getattr(const char* path, struct stat* stat, struct fuse_file_info* fileInfo) -> int;

    FS m_fs;

#define FUSE_OP(OP) .OP = &callBridge<&FuseFS::OP>
    const struct fuse_operations m_fuseOps{
        FUSE_OP(getattr),
        .readlink        = nullptr,
        .mknod           = nullptr,
        .mkdir           = nullptr,
        .unlink          = nullptr,
        .rmdir           = nullptr,
        .symlink         = nullptr,
        .rename          = nullptr,
        .link            = nullptr,
        .chmod           = nullptr,
        .chown           = nullptr,
        .truncate        = nullptr,
        .open            = nullptr,
        .read            = nullptr,
        .write           = nullptr,
        .statfs          = nullptr,
        .flush           = nullptr,
        .release         = nullptr,
        .fsync           = nullptr,
        .setxattr        = nullptr,
        .getxattr        = nullptr,
        .listxattr       = nullptr,
        .removexattr     = nullptr,
        .opendir         = nullptr,
        .readdir         = nullptr,
        .releasedir      = nullptr,
        .fsyncdir        = nullptr,
        .init            = nullptr,
        .destroy         = nullptr,
        .access          = nullptr,
        .create          = nullptr,
        .lock            = nullptr,
        .utimens         = nullptr,
        .bmap            = nullptr,
        .ioctl           = nullptr,
        .poll            = nullptr,
        .write_buf       = nullptr,
        .read_buf        = nullptr,
        .flock           = nullptr,
        .fallocate       = nullptr,
        .copy_file_range = nullptr,
        .lseek           = nullptr,
    };
#undef FUSE_OP
};

template <fs::FileSystemT FS> auto FuseFS<FS>::main(int argc, char* argv[]) -> int {
    return fuse_main(argc, argv, &m_fuseOps, this);
}

template <fs::FileSystemT FS>
auto FuseFS<FS>::getattr(const char* path, struct stat* stat, struct fuse_file_info* fileInfo)
    -> int {
    UNUSED(path);
    UNUSED(stat);
    UNUSED(fileInfo);
    return 0;
}

}    // namespace jonas::fusefs

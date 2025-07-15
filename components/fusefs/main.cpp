#include <fuse3/fuse.h>

#include <common/logger.hpp>

namespace {

constexpr struct fuse_operations fuseOps{
    .getattr         = nullptr,
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

}

auto main(int argc, char* argv[]) -> int { return fuse_main(argc, argv, &fuseOps, nullptr); }

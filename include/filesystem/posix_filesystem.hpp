#pragma once

#include <filesystem/common.hpp>
#include <filesystem/filesystem.hpp>

namespace jonas::fs {

class FileHandle {
public:
    auto open(const std::string_view& path) noexcept -> Result<void>;
    auto close() noexcept -> Result<void>;

private:
};
static_assert(FileHandleT<FileHandle>);

class PosixFS {
public:
    auto getattr(const std::string_view& path) noexcept -> Result<Stat>;

private:
};
static_assert(FileSystemT<PosixFS>);

}    // namespace jonas::fs

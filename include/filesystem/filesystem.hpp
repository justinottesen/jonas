#pragma once

#include <sys/stat.h>

#include <common/macros.hpp>
#include <common/result.hpp>
#include <filesystem>

ENUM(Example, uint8_t, FOO, BAR, XYZ);

namespace fs {

CONCEPT_INTERFACE(FileSystemT) {
    CONCEPT_METHOD(getattr, Result<std::filesystem::file_status>, const std::filesystem::path&);
    CONCEPT_METHOD(readlink, Result<std::filesystem::path>, const std::filesystem::path&);
    CONCEPT_METHOD(mknod, Result<void>, const std::filesystem::path&, mode_t, dev_t);
    CONCEPT_METHOD(mkdir, Result<void>, const std::filesystem::path&, mode_t);
    CONCEPT_METHOD(unlink, Result<void>, const std::filesystem::path&);
    CONCEPT_METHOD(rmdir, Result<void>, const std::filesystem::path&);
    CONCEPT_METHOD(symlink, Result<void>, const std::filesystem::path&,
                   const std::filesystem::path&);
    CONCEPT_METHOD(rename, Result<void>, const std::filesystem::path&,
                   const std::filesystem::path&);
    CONCEPT_METHOD(link, Result<void>, const std::filesystem::path&, const std::filesystem::path&);
};

class FileSystem {
public:
    auto getattr(const std::filesystem::path& path) noexcept
        -> Result<std::filesystem::file_status>;
    auto readlink(const std::filesystem::path& path) noexcept -> Result<std::filesystem::path>;
    auto mknod(const std::filesystem::path& path, mode_t mode, dev_t rdev) noexcept -> Result<void>;
    auto mkdir(const std::filesystem::path& path, mode_t mode) noexcept -> Result<void>;
    auto unlink(const std::filesystem::path& path) noexcept -> Result<void>;
    auto rmdir(const std::filesystem::path& path) noexcept -> Result<void>;
    auto symlink(const std::filesystem::path& from, const std::filesystem::path& to) noexcept
        -> Result<void>;
    auto rename(const std::filesystem::path& from, const std::filesystem::path& to) noexcept
        -> Result<void>;
    auto link(const std::filesystem::path& from, const std::filesystem::path& to) noexcept
        -> Result<void>;
};

static_assert(FileSystemT<FileSystem>);

}    // namespace fs

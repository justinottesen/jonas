#pragma once

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>

#include <common/macros.hpp>
#include <common/result.hpp>
#include <concepts>
#include <cstdint>
#include <vector>

class Buffer;
class OwnedBuffer;

namespace fs {

struct FileTimes;

struct FileMetadata;

ENUM(XAttrFlag, uint8_t, NONE, CREATE, REPLACE);

enum class FileMode {
    NONE       = 0,
    UID        = S_ISUID,
    GID        = S_ISGID,
    STICKY_BIT = S_ISVTX,

    FULL_OWNER  = S_IRWXU,
    READ_OWNER  = S_IRUSR,
    WRITE_OWNER = S_IWUSR,
    EXEC_OWNER  = S_IXUSR,

    FULL_GROUP  = S_IRWXG,
    READ_GROUP  = S_IRGRP,
    WRITE_GROUP = S_IWGRP,
    EXEC_GROUP  = S_IXGRP,

    FULL_OTHERS  = S_IRWXO,
    READ_OTHERS  = S_IROTH,
    WRITE_OTHERS = S_IWOTH,
    EXEC_OTHERS  = S_IXOTH,
};

enum class SeekMode {
    SET = SEEK_SET,
    CUR = SEEK_CUR,
    END = SEEK_END,
};

CONCEPT_INTERFACE(FileDescriptorT) {

    /*
        { fd.listxattr() } noexcept -> std::same_as<Result<std::vector<std::string>>>;
        { fd.getxattr(std::declval<std::string_view>) } noexcept ->
    std::same_as<Result<OwnedBuffer>>;
        {
            fd.setxattr(std::declval<std::string_view>, std::declval<const Buffer&>,
                        std::declval<XAttrFlag>)
        } noexcept -> std::same_as<Result<void>>;
        {
            fd.setxattr(std::declval<std::string_view>, std::declval<const Buffer&>,
                        std::declval<XAttrFlag>)
        } noexcept -> std::same_as<Result<void>>;
        { fd.chown(std::declval<uid_t>, std::declval<gid_t>) } noexcept ->
    std::same_as<Result<void>>; { fd.chmod(std::declval<FileMode>) } noexcept ->
    std::same_as<Result<void>>; { fd.utimes(std::declval<const FileTimes &>) } noexcept ->
    std::same_as<Result<void>>; { fd.utimens(std::declval<const FileTimes &>) } noexcept ->
    std::same_as<Result<void>>; auto stat() noexcept -> Result<FileMetadata>; auto sync() noexcept
    -> Result<void>; auto truncate(size_t length) noexcept -> Result<void>; auto lseek(off_t offset,
    SeekMode whence) noexcept -> Result<size_t>; auto write(const Buffer & value) noexcept ->
    Result<size_t>;
    */
};

class FileDescriptor {
public:
    auto listxattr() noexcept -> Result<std::vector<std::string>>;
    auto getxattr(std::string_view name) noexcept -> Result<OwnedBuffer>;
    auto setxattr(std::string_view name, const Buffer& value, XAttrFlag flags) noexcept
        -> Result<void>;
    auto chown(uid_t owner, gid_t group) noexcept -> Result<void>;
    auto chmod(FileMode mode) noexcept -> Result<void>;
    auto utimes(const FileTimes& ftimes) noexcept -> Result<void>;
    auto utimens(const FileTimes& ftimes) noexcept -> Result<void>;
    auto stat() noexcept -> Result<FileMetadata>;
    auto sync() noexcept -> Result<void>;
    auto truncate(size_t length) noexcept -> Result<void>;
    auto lseek(off_t offset, SeekMode whence) noexcept -> Result<size_t>;
    auto write(const Buffer& value) noexcept -> Result<size_t>;
};

class FileSystemInterface {
public:
};

}    // namespace fs

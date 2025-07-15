#include <common/logger.hpp>
#include <filesystem/filesystem.hpp>
#include <filesystem>

namespace fs {

auto FileSystem::getattr(const std::filesystem::path& path) noexcept
    -> Result<std::filesystem::file_status> {
    LOG(FINE) << "Calling getattr on " << path;
    std::error_code err;
    auto            status = std::filesystem::status(path, err);
    if (err) {
        LOG(ERROR) << "Failed getattr on " << path << ": " << err;
        return std::unexpected(err);
    }
    return status;
}

auto FileSystem::readlink(const std::filesystem::path& path) noexcept
    -> Result<std::filesystem::path> {
    LOG(FINE) << "Calling readlink on " << path;
    std::error_code err;
    auto            linkPath = std::filesystem::read_symlink(path, err);
    if (err) {
        LOG(ERROR) << "Failed readlink on " << path << ": " << err;
        return std::unexpected(err);
    }
    return linkPath;
}

// TODO: Make a mode enum class that is convertible to mode_t
auto FileSystem::mknod(const std::filesystem::path& path, mode_t mode, dev_t rdev) noexcept
    -> Result<void> {
    LOG(FINE) << "Calling mknod at " << path << " with " << mode << ", " << rdev;
    if ((S_ISFIFO(mode) ? mkfifo(path.c_str(), mode) : ::mknod(path.c_str(), mode, rdev)) == -1) {
        std::error_code err = std::make_error_code(std::errc(errno));
        LOG(ERROR) << "Failed mkdnod at " << path << " with " << mode << ", " << rdev << ": "
                   << err;
        return std::unexpected(err);
    }
    return {};
}

auto FileSystem::mkdir(const std::filesystem::path& path, mode_t mode) noexcept -> Result<void> {
    LOG(FINE) << "Calling mkdir at " << path << " with " << mode;
    std::error_code err;
    bool            created = std::filesystem::create_directory(path, err);
    if (err) {
        LOG(ERROR) << "Failed mkdir at " << path << " with " << mode << ": " << err;
        return std::unexpected(err);
    }
    if (!created) { LOG(WARN) << "Called mkdir at " << path << " but dir already exists"; }
    // TODO: If this doesn't work, make some conversion function. It *should* work
    std::filesystem::permissions(path, static_cast<std::filesystem::perms>(mode), err);
    if (err) {
        LOG(ERROR) << "Failed to set permissions " << mode << " at " << path << ": " << err;
        return std::unexpected(err);
    }
    return {};
}

auto FileSystem::unlink(const std::filesystem::path& path) noexcept -> Result<void> {
    LOG(FINE) << "Calling unlink at " << path;
    std::error_code err;
    bool            removed = std::filesystem::remove(path, err);
    if (err) {
        LOG(ERROR) << "Failed unlink at " << path << ": " << err;
        return std::unexpected(err);
    }
    if (!removed) { LOG(WARN) << "Called unlink at " << path << " but path does not exist"; }
    return {};
}

auto FileSystem::rmdir(const std::filesystem::path& path) noexcept -> Result<void> {
    LOG(FINE) << "Calling rmdir at " << path;
    std::error_code err;
    bool            removed = std::filesystem::remove(path, err);
    if (err) {
        LOG(ERROR) << "Failed rmdir at " << path << ": " << err;
        return std::unexpected(err);
    }
    if (!removed) { LOG(WARN) << "Called rmdir at " << path << " but path does not exist"; }
    return {};
}

// TODO: Update this to care about file vs directory link. POSIX doesn't care allegedly
auto FileSystem::symlink(const std::filesystem::path& from,
                         const std::filesystem::path& to) noexcept -> Result<void> {
    LOG(FINE) << "Calling symlink " << from << " -> " << to;
    std::error_code err;
    std::filesystem::create_symlink(to, from, err);
    if (err) {
        LOG(ERROR) << "Failed symlink " << from << " -> " << to << ": " << err;
        return std::unexpected(err);
    }
    return {};
}

// TODO: FUSE rename takes flags, see if I need to worry about that
auto FileSystem::rename(const std::filesystem::path& from, const std::filesystem::path& to) noexcept
    -> Result<void> {
    LOG(FINE) << "Calling rename " << from << " -> " << to;
    std::error_code err;
    std::filesystem::rename(from, to, err);
    if (err) {
        LOG(ERROR) << "Failed rename " << from << " -> " << to << ": " << err;
        return std::unexpected(err);
    }
    return {};
}

auto FileSystem::link(const std::filesystem::path& from, const std::filesystem::path& to) noexcept
    -> Result<void> {
    LOG(FINE) << "Calling link " << from << " -> " << to;
    std::error_code err;
    std::filesystem::create_hard_link(to, from, err);
    if (err) {
        LOG(ERROR) << "Failed link " << from << " -> " << to << ": " << err;
        return std::unexpected(err);
    }
    return {};
}

};    // namespace fs

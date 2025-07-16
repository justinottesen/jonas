#include <sys/stat.h>
#include <common/logger.hpp>
#include <common/storage.hpp>
#include <filesystem/posix_filesystem.hpp>

namespace jonas::fs {

auto PosixFS::getattr(const std::string& path) noexcept -> Result<Stat> {
    struct stat statBuf{};
    int res = ::lstat(path.c_str(), &statBuf);
    if (res == -1) {
        return std::unexpected(std::make_error_code(std::errc(errno)));
    }
    return Stat(statBuf);
}

auto PosixFS::joinPaths(std::string_view begin, std::string_view end) noexcept -> std::string {
    if (begin.empty()) { return std::string(begin); }
    if (end.empty()) { return std::string(end); }

    std::string_view trimBegin = (begin.back() == '/') ? begin.substr(0, begin.size() - 1) : begin;
    
    return std::string(trimBegin).append(end.front() == '/' ? "" : "/").append(end);
}

};    // namespace fs

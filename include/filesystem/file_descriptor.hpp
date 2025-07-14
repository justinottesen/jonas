#pragma once

#include <common/macros.hpp>
#include <common/result.hpp>
#include <cstdint>
#include <string>
#include <vector>

struct Buffer;

namespace fs {

ENUM(XAttrFlags, uint8_t, NONE, CREATE, REPLACE);

CONCEPT_INTERFACE(FileDescriptorLike) {
    CONCEPT_METHOD(listxattr, Result<std::vector<std::string>>);
    CONCEPT_METHOD(getxattr, Result<Buffer>, std::string_view);
    CONCEPT_METHOD(setxattr, Result<void>, std::string_view, const Buffer&, XAttrFlags);
    CONCEPT_METHOD(chown, Result<void>, uid_t, gid_t);
    CONCEPT_METHOD(chmod, Result<void>, FileMode);
};

};    // namespace fs

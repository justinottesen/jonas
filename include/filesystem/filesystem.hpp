#pragma once

#include <sys/stat.h>

#include <common/macros.hpp>
#include <common/result.hpp>

namespace jonas::fs {

struct Stat;

CONCEPT_INTERFACE(FileHandleT) {
    CONCEPT_METHOD(open, Result<void>, std::string_view);
    CONCEPT_METHOD(close, Result<void>);
};

CONCEPT_INTERFACE(FileSystemT) {
    CONCEPT_METHOD(getattr, Result<Stat>, const std::string&);
    CONCEPT_METHOD(joinPaths, std::string, std::string_view, std::string_view);
};

}    // namespace jonas::fs

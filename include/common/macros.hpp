#pragma once

// Silence an unused parameter warning
#define UNUSED(X) (void)(X)

// Rescan macro tokens up to 256 times
#define _EXPAND4(...) __VA_OPT__(__VA_ARGS__)
#define _EXPAND3(...) __VA_OPT__(_EXPAND4(_EXPAND4(_EXPAND4(_EXPAND4(__VA_ARGS__)))))
#define _EXPAND2(...) __VA_OPT__(_EXPAND3(_EXPAND3(_EXPAND3(_EXPAND3(__VA_ARGS__)))))
#define _EXPAND1(...) __VA_OPT__(_EXPAND2(_EXPAND2(_EXPAND2(_EXPAND2(__VA_ARGS__)))))
#define EXPAND(...)   __VA_OPT__(_EXPAND1(_EXPAND1(_EXPAND1(_EXPAND1(__VA_ARGS__)))))
#define PARENS        ()

// For each macro and helpers
#define _FOR_EACH_HELPER(MACRO, ARG_1, ...) \
    MACRO(ARG_1) __VA_OPT__(_FOR_EACH_AGAIN PARENS(MACRO, __VA_ARGS__))
#define _FOR_EACH_AGAIN()    _FOR_EACH_HELPER
#define FOR_EACH(MACRO, ...) __VA_OPT__(EXPAND(_FOR_EACH_HELPER(MACRO, __VA_ARGS__)))
#define _FOR_EACH_LIST_HELPER(MACRO, ARG_1, ...) \
    MACRO(ARG_1) __VA_OPT__(, _FOR_EACH_LIST_AGAIN PARENS(MACRO, __VA_ARGS__))
#define _FOR_EACH_LIST_AGAIN()    _FOR_EACH_LIST_HELPER
#define FOR_EACH_LIST(MACRO, ...) __VA_OPT__(EXPAND(_FOR_EACH_LIST_HELPER(MACRO, __VA_ARGS__)))

// Enum with `to_string` and `from_string` methods
#define _ENUM_CASE(NAME) \
    case NAME: return #NAME;
#define _ENUM_IF(NAME) \
    if (_str == #NAME) { return NAME; }
#define ENUM(TYPE, BASE, ...)                                                                   \
    enum class TYPE : BASE { __VA_ARGS__ };                                                     \
    [[nodiscard]] constexpr auto to_string(TYPE _enum) noexcept -> Result<std::string_view> {   \
        using enum TYPE;                                                                        \
        switch (_enum) {                                                                        \
            FOR_EACH(_ENUM_CASE, __VA_ARGS__)                                                   \
            default: return std::unexpected(std::make_error_code(std::errc::invalid_argument)); \
        }                                                                                       \
    }                                                                                           \
    [[nodiscard]] constexpr auto from_string(std::string_view _str) noexcept -> Result<TYPE> {  \
        using enum TYPE;                                                                        \
        FOR_EACH(_ENUM_IF, __VA_ARGS__)                                                         \
        return std::unexpected(std::make_error_code(std::errc::invalid_argument));              \
    }

// Cconstructors, Assignment, Destrcutors
#define CTOR(CLASSNAME, MODIFIER) CLASSNAME() noexcept = MODIFIER
#define COPY_CTOR_ASSIGN(CLASSNAME, MODIFIER)                           \
    CLASSNAME(const CLASSNAME&) noexcept                    = MODIFIER; \
    auto operator=(const CLASSNAME&) noexcept -> CLASSNAME& = MODIFIER
#define MOVE_CTOR_ASSIGN(CLASSNAME, MODIFIER)                      \
    CLASSNAME(CLASSNAME&&) noexcept                    = MODIFIER; \
    auto operator=(CLASSNAME&&) noexcept -> CLASSNAME& = MODIFIER

#define DEFAULT_DTOR(CLASSNAME)         ~CLASSNAME() noexcept = default
#define VIRTUAL_DEFAULT_DTOR(CLASSNAME) virtual DEFAULT_DTOR(CLASSNAME)

#define DEFAULT_CTOR(CLASSNAME) CTOR(CLASSNAME, default)
#define DEFAULT_COPY(CLASSNAME) COPY_CTOR_ASSIGN(CLASSNAME, default)
#define DEFAULT_MOVE(CLASSNAME) MOVE_CTOR_ASSIGN(CLASSNAME, default)

#define DELETE_CTOR(CLASSNAME) CTOR(CLASSNAME, delete)
#define DELETE_COPY(CLASSNAME) COPY_CTOR_ASSIGN(CLASSNAME, delete)
#define DELETE_MOVE(CLASSNAME) MOVE_CTOR_ASSIGN(CLASSNAME, delete)

#define DEFAULT_CTOR_DTOR(CLASSNAME) \
    DEFAULT_CTOR(CLASSNAME);         \
    DEFAULT_DTOR(CLASSNAME)

#define DEFAULT_COPY_MOVE(CLASSNAME) \
    DEFAULT_COPY(CLASSNAME);         \
    DEFAULT_MOVE(CLASSNAME);

#define DELETE_COPY_MOVE(CLASSNAME) \
    DELETE_COPY(CLASSNAME);         \
    DELETE_MOVE(CLASSNAME)

#define DELETE_CTOR_COPY_MOVE(CLASSNAME) \
    DELETE_CTOR(CLASSNAME);              \
    DELETE_COPY_MOVE(CLASSNAME)

// Concept method macros
#define CONCEPT_INTERFACE(CONCEPTNAME) \
    template <typename T>              \
    concept CONCEPTNAME =              \
        requires

#define _WRAP_DECLVAL(TYPE) std::declval<TYPE>
#define CONCEPT_METHOD(NAME, RETURN, ...)                               \
    {                                                                   \
        std::declval<T>.NAME(FOR_EACH_LIST(_WRAP_DECLVAL, __VA_ARGS__)) \
    } noexcept -> std::same_as<RETURN>

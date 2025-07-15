#pragma once

#include <common/macros.hpp>

namespace jonas {

/* WARN: This is not fleshed out, just an idea I had. Wrapper around a std::variant<T, T&> might be
 * better? That approach avoids having two different types.
 */
CONCEPT_INTERFACE(StoragePolicyT, typename ValueType) {
    CONCEPT_METHOD(get, ValueType&);
    CONCEPT_METHOD_CONST(get, const ValueType&);
};

template <typename ValueType> class StoragePolicy {
public:
    using RefType      = std::conditional_t<std::is_reference_v<ValueType>, ValueType, ValueType&>;
    using ConstRefType = std::conditional_t<std::is_reference_v<ValueType>,
                                            std::add_const_t<ValueType>, const ValueType&>;

    [[nodiscard]] auto get() noexcept -> RefType;
    [[nodiscard]] auto get() const noexcept -> ConstRefType;

private:
    ValueType m_value;
};
static_assert(StoragePolicyT<StoragePolicy<int>, int>);

template <typename T> using OwningStorage = StoragePolicy<T>;
template <typename T> using ViewStorage   = StoragePolicy<T&>;

}    // namespace jonas

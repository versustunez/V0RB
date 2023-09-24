#pragma once

#include <VUtils/Memory/Memory.h>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace VSTZ {

template <typename T> using Scope = Memory::Scope<T>;
template <typename T> using Ref = Memory::Ref<T>;
template <typename T> using Weak = Memory::Weak<T>;

using Mutex = std::mutex;
using Guard = std::lock_guard<Mutex>;

template <typename T, typename S> using Map = std::unordered_map<T, S>;
template <typename T> using Vector = std::vector<T>;

using InstanceID = std::uint32_t;

using u64 = std::uint64_t;
using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8 = std::uint8_t;

using i64 = std::int64_t;
using i32 = std::int32_t;
using i16 = std::int16_t;
using i8 = std::int8_t;
} // namespace VSTZ

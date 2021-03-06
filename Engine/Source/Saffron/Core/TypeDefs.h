#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <functional>
#include <tuple>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>
#include <atomic>
#include <optional>
#include <bitset>
#include <initializer_list>

namespace Se
{
// Primitives

// Unsigned typedef
using wchar = wchar_t;
using ubyte = unsigned char;
using uchar = unsigned char;
using ushort = unsigned short;
using uint = unsigned int;
#if defined(_MSC_VER)
using ulong = unsigned __int64;
#else
using ulong = unsigned long long;
#endif

template <typename T>
using List = std::vector<T>;
template <typename Type, size_t size>
using Array = std::array<Type, size>;
template <typename Key, typename Value>
using TreeMap = std::map<Key, Value>;
template <typename Key, typename Value>
using MultiMap = std::multimap<Key, Value>;
template <typename Key, typename Value>
using HashMap = std::unordered_map<Key, Value>;
template <typename T>
using TreeSet = std::set<T>;
template <typename T>
using HashSet = std::unordered_set<T>;
template <class FunctionType>
using Function = std::function<FunctionType>;
template <class... Types>
using Tuple = std::tuple<Types...>;
template <typename ClassType>
using Atomic = std::atomic<ClassType>;
template <typename FirstClass, typename SecondClass>
using Pair = std::pair<FirstClass, SecondClass>;
template <typename T>
using Optional = std::optional<T>;
template <size_t t_NumBits>
using Bitset = std::bitset<t_NumBits>;
template <class t_ElemType>
using InitializerList = std::initializer_list<t_ElemType>;

// Memory
template <typename T>
using Unique = std::unique_ptr<T>;
//template <typename T>
//using Shared = std::shared_ptr<T>;
template <typename T>
using Weak = std::weak_ptr<T>;

using String = std::string;
using WString = std::wstring;
using Path = std::filesystem::path;
using DirEntry = std::filesystem::directory_entry;
using OStream = std::ofstream;
using IStream = std::ifstream;
using StringStream = std::stringstream;
using OStringStream = std::ostringstream;
using IStringStream = std::istringstream;
using Thread = std::thread;
using Mutex = std::mutex;
using ConditionVariable = std::condition_variable;

template <class MoveClass>
constexpr auto&& Move(MoveClass&& moveClass) noexcept
{
	return static_cast<std::remove_reference_t<MoveClass>&&>(moveClass);
}

template <class FirstClass, class SecondClass>
constexpr auto CreatePair(FirstClass&& first, SecondClass&& second) noexcept
{
	using Mypair = std::pair<std::remove_reference_t<FirstClass>, std::remove_reference_t<SecondClass>>;
	return Mypair(std::forward<FirstClass>(first), std::forward<SecondClass>(second));
}

// Memory
template <typename T, typename ... Args>
constexpr Unique<T> CreateUnique(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

//template <typename T, typename ... Args>
//constexpr Shared<T> CreateShared(Args&& ... args)
//{
//	return std::make_shared<T>(std::forward<Args>(args)...);
//}
}

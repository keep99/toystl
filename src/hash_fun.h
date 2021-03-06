#ifndef TOYSTL_SRC_HASH_FUN_H_
#define TOYSTL_SRC_HASH_FUN_H_

#include <cstddef>

namespace toystl {
// 对于大部分类型，hash function 什么也不做，直接返回原值
template <class Key>
struct hash {};

// 对于字符字符串，设计了一个转换函数
inline size_t hash_string(const char* s) {
  unsigned long h = 0;
  for (; *s; ++s) {
    h = 5 * h + *s;
  }

  return size_t(h);
}

template <>
struct hash<char*> {
  size_t operator()(const char* s) const { return hash_string(s); }
};

template <>
struct hash<const char*> {
  size_t operator()(const char* s) const { return hash_string(s); }
};

template <>
struct hash<char> {
  size_t operator()(char x) const { return x; }
};

template <>
struct hash<unsigned char> {
  size_t operator()(unsigned char x) const { return x; }
};

template <>
struct hash<signed char> {
  size_t operator()(signed char x) const { return x; }
};

template <>
struct hash<short> {
  size_t operator()(short x) const { return x; }
};

template <>
struct hash<unsigned short> {
  size_t operator()(unsigned short x) const { return x; }
};

template <>
struct hash<int> {
  size_t operator()(int x) const { return x; }
};

template <>
struct hash<unsigned int> {
  size_t operator()(unsigned int x) const { return x; }
};

template <>
struct hash<long> {
  size_t operator()(long x) const { return x; }
};

template <>
struct hash<unsigned long> {
  size_t operator()(unsigned long x) const { return x; }
};
}  // namespace toystl

#endif  // TOYSTL_SRC_HASH_FUN_H_
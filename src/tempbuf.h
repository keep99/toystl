#ifndef TOYSTL_SRC_TEMPBUF_H_
#define TOYSTL_SRC_TEMPBUF_H_

#include <climits>
#include <cstddef>
#include <cstdlib>

#include "construct.h"
#include "iterator_base.h"
#include "memory_function.h"

namespace toystl {
// 类模板 : temporary_buffer
// 进行临时缓冲区的申请与释放
template <class ForwardIterator, class T>
class temporary_buffer {
 private:
  ptrdiff_t original_len;  // 缓冲区申请的大小
  ptrdiff_t len;           // 缓冲区实际的大小
  T* buffer;               // 指向缓冲区的指针

 public:
  // 构造、析构函数
  temporary_buffer(ForwardIterator first, ForwardIterator last);

  ~temporary_buffer() {
    toystl::destroy(buffer, buffer + len);
    free(buffer);
  }

 public:
  ptrdiff_t size() const noexcept { return len; }
  ptrdiff_t requested_size() const noexcept { return original_len; }
  T* begin() noexcept { return buffer; }
  T* end() noexcept { return buffer + len; }

 private:
  void allocate_buffer();
  void initialize_buffer(const T&, std::true_type) {}
  void initialize_buffer(const T& value, std::false_type) {
    toystl::uninitialized_fill_n(buffer, len, value);
  }

 private:
  temporary_buffer(const temporary_buffer&);
  void operator=(const temporary_buffer&);
};

// 构造函数
template <class ForwardIterator, class T>
temporary_buffer<ForwardIterator, T>::temporary_buffer(ForwardIterator first,
                                                       ForwardIterator last) {
  try {
    len = toystl::distance(first, last);
    allocate_buffer();
    if (len > 0) {
      initialize_buffer(*first, std::is_trivially_default_constructible<T>());
    }
  } catch (...) {
    free(buffer);
    buffer = nullptr;
    len = 0;
  }
}

// allocate_buffer 函数
template <class ForwardIterator, class T>
void temporary_buffer<ForwardIterator, T>::allocate_buffer() {
  original_len = len;
  if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
    len = INT_MAX / sizeof(T);
  while (len > 0) {
    buffer = static_cast<T*>(malloc(len * sizeof(T)));
    if (buffer) break;
    len /= 2;  // 申请失败时减少申请空间大小
  }
}
}  // namespace toystl

#endif  // TOYSTL_SRC_TEMPBUF_H_

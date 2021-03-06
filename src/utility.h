#ifndef TOYSTL_SRC_UTILITY_H_
#define TOYSTL_SRC_UTILITY_H_

#include "type_traits.h"

namespace toystl {
// forward 和 move 的实现参考 C++ Primer P610 - 614
// 关键技术：引用折叠 & + & = &; && + & = &; & + && = &; && + && = &&。

// forward
// 完美转发。完美转发实现了参数在传递过程中保持其值属性的功能，即若是左值，则传递之后仍然是左值，若是右值，则传递之后仍然是右值。
// forward执行到右值的有条件转换(传递右值，返回右值引用；传递左值，返回左值引用)

// 参数如果是左值，传入此模板函数
template <class T>
T&& forward(typename toystl::remove_reference<T>::type& t) noexcept {
  return static_cast<T&&>(t);
}

// 参数如果是右值，传入此模板函数
template <class T>
T&& forward(typename toystl::remove_reference<T>::type&& t) noexcept {
  return static_cast<T&&>(t);
}

// move
// move执行到右值的无条件转换(不管传递左值还是右值，最后都返回右值引用)
template <class T>
typename toystl::remove_reference<T>::type&& move(T&& t) noexcept {
  using return_type = typename toystl::remove_reference<T>::type&&;
  return static_cast<return_type>(t);
}

// swap
template <class T>
void swap(T& lhs, T& rhs) {
  auto tmp(toystl::move(lhs));
  lhs = toystl::move(rhs);
  rhs = toystl::move(tmp);
}

template <class ForwardIterator1, class ForwardIterator2>
ForwardIterator2 swap_range(ForwardIterator1 first1, ForwardIterator1 last1,
                            ForwardIterator2 first2) {
  for (; first1 != last1; first1++, first2++) {
    toystl::swap(*first1, *first2);
  }

  return first2;
}

template <class Tp, size_t N>
void swap(Tp (&a)[N], Tp (&b)[N]) {
  toystl::swap_range(a, a + N, b);
}

// pair
template <class T1, class T2>
struct pair {
  typedef T1 first_type;
  typedef T2 second_type;

  T1 first;
  T2 second;

  // 构造函数
  pair() : first(), second() {}
  pair(const T1& a, const T2& b) : first(a), second(b) {}

  template <class U1, class U2>
  explicit pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}

  template <class U1, class U2>
  pair(U1&& x, U2&& y)
      : first(toystl::forward<U1>(x)), second(toystl::forward<U2>(y)) {}

  template <class U1, class U2>
  explicit pair(pair<U1, U2>&& p)
      : first(toystl::forward<U1>(p.first)),
        second(toystl::forward<U2>(p.second)) {}

  pair(const pair& p) = default;
  pair(pair&& p) = default;

  template <class U1, class U2>
  explicit pair(pair&& p)
      : first(toystl::move(p.first)), second(toystl::move(p.second)) {}

  // operator=
  pair& operator=(const pair& rhs) {
    if (this != rhs) {
      first = rhs.first;
      second = rhs.second;
    }

    return *this;
  }

  pair& operator=(pair&& rhs) {
    if (this != rhs) {
      first = toystl::move(rhs.first);
      second = toystl::move(rhs.second);
    }

    return *this;
  }

  template <class Other1, class Other2>
  pair& operator=(const pair<Other1, Other2>& other) {
    first = other.first;
    second = other.second;
  }

  template <class Other1, class Other2>
  pair& operator=(const pair<Other1, Other2>&& other) {
    first = toystl::forward<Other1>(other.first);
    second = toystl::forward<Other2>(other.second);

    return *this;
  }

  ~pair() = default;

  void swap(pair& other) {
    if (this != other) {
      toystl::swap(first, other.first);
      toystl::swap(second, other.second);
    }
  }
};

// 重载比较操作符
template <class Ty1, class Ty2>
bool operator==(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <class Ty1, class Ty2>
bool operator<(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
  return lhs.first < rhs.first ||
         (lhs.first == rhs.first && lhs.second < rhs.second);
}

template <class Ty1, class Ty2>
bool operator!=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
  return !(lhs == rhs);
}

template <class Ty1, class Ty2>
bool operator>(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
  return rhs < lhs;
}

template <class Ty1, class Ty2>
bool operator<=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
  return !(rhs < lhs);
}

template <class Ty1, class Ty2>
bool operator>=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
  return !(lhs < rhs);
}

// 重载 toystl 的 swap
template <class Ty1, class Ty2>
void swap(pair<Ty1, Ty2>& lhs, pair<Ty1, Ty2>& rhs) {
  lhs.swap(rhs);
}

// 创建一个 pair 对象
template <class Ty1, class Ty2>
pair<Ty1, Ty2> make_pair(Ty1&& first, Ty2&& second) {
  return pair<Ty1, Ty2>(toystl::forward<Ty1>(first),
                        toystl::forward<Ty2>(second));
}
}  // namespace toystl

#endif  // TOYSTL_SRC_UTILITY_H_
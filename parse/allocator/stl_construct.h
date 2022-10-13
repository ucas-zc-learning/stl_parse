/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef __SGI_STL_INTERNAL_CONSTRUCT_H
#define __SGI_STL_INTERNAL_CONSTRUCT_H

#include <new.h>                      // 使用placement new必须引入的头文件

__STL_BEGIN_NAMESPACE

// 构造工具，用__value初始化__p
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __value) {
  // placement new，调用_T1::_T1(__value)
  // placement new的作用是将初值设定到指针所指的空间上
  new ((void*) __p) _T1(__value);
}

// 构造工具，用默认构造函数初始化
template <class _T1>
inline void _Construct(_T1* __p) {
  // placement new，调用_T1::_T1()
  new ((void*) __p) _T1();
}

// 析构工具，接受一个指针__pointer，这是第一个版本
template <class _Tp>
inline void _Destroy(_Tp* __pointer) {
  // 调用默认的_Tp::~_Tp()
  __pointer->~_Tp();
}

// 如果元素的数值类别有non-trivial destructor
template <class _ForwardIterator>
void
__destroy_aux(_ForwardIterator __first, _ForwardIterator __last, __false_type)
{
  for ( ; __first != __last; ++__first)
    destroy(&*__first);
}

// 如果元素的数值类别有trivial destructor
template <class _ForwardIterator> 
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, __true_type) {}

// 根据找出的元素的数值类别判断是否有trivial destructor（无关紧要的析构），从而提高运行效率
template <class _ForwardIterator, class _Tp>
inline void 
__destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
  // __type_traits，提供了一种机制，允许针对不同的型别属性，在编译期间完成函数派送决定
  typedef typename __type_traits<_Tp>::has_trivial_destructor
          _Trivial_destructor;
  __destroy_aux(__first, __last, _Trivial_destructor());
}

// 析构工具，这是第二版本，接收两个迭代器，试图找出元素的数值类别
// 析构掉[__first, __last)之间的元素
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) {
  __destroy(__first, __last, __VALUE_TYPE(__first));
}

// char*，int*，long*，float*，double*这些类型析构什么都不需要做
inline void _Destroy(char*, char*) {}
inline void _Destroy(int*, int*) {}
inline void _Destroy(long*, long*) {}
inline void _Destroy(float*, float*) {}
inline void _Destroy(double*, double*) {}
#ifdef __STL_HAS_WCHAR_T
inline void _Destroy(wchar_t*, wchar_t*) {}
#endif /* __STL_HAS_WCHAR_T */

// --------------------------------------------------
// 为了兼容HP STL，给_Construct、_Destroy()等包了一层壳

template <class _T1, class _T2>
inline void construct(_T1* __p, const _T2& __value) {
  _Construct(__p, __value);
}

template <class _T1>
inline void construct(_T1* __p) {
  _Construct(__p);
}

template <class _Tp>
inline void destroy(_Tp* __pointer) {
  _Destroy(__pointer);
}

template <class _ForwardIterator>
inline void destroy(_ForwardIterator __first, _ForwardIterator __last) {
  _Destroy(__first, __last);
}

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_CONSTRUCT_H */

// Local Variables:
// mode:C++
// End:

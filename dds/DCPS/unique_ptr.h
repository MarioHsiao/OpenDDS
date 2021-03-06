#ifndef UNIQUE_PTR_H_18C6F30C
#define UNIQUE_PTR_H_18C6F30C

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "dds/Versioned_Namespace.h"

#include "ace/config-lite.h"

#ifdef ACE_HAS_CPP11
#include <utility>
#else
#include <algorithm>
#endif

OPENDDS_BEGIN_VERSIONED_NAMESPACE_DECL

namespace OpenDDS {
namespace DCPS {

template <typename T>
class rv : public T {
  rv();
  ~rv();
  rv(const rv &);
  void operator=(const rv&);
};

template <typename T>
struct default_deleter
{
  void operator()(T* ptr) const
  {
    delete ptr;
  }
};

template <typename T, typename Deleter = default_deleter<T> >
class unique_ptr {
public:
  typedef T element_type;
  typedef Deleter deleter_type;

  explicit unique_ptr(T* p = 0) // never throws
    : ptr_(p)
  {}

#ifndef __SUNPRO_CC
  unique_ptr(rv<unique_ptr>& other)
    : ptr_(other.release())
  {}
#else
  unique_ptr(unique_ptr& other)
    : ptr_(other.release())
  {}
#endif

  ~unique_ptr() // never throws
  {
    Deleter()(ptr_);
  }

  unique_ptr& operator=(rv<unique_ptr>& other)
  {
    reset(other.release());
    return *this;
  }

  void reset(T* p = 0) // never throws
  {
    Deleter()(ptr_);
    ptr_ = p;
  }

  T* release()
  {
    T* p = ptr_;
    ptr_ = 0;
    return p;
  }

  T& operator*() const // never throws
  {
    return *get();
  }

  T* operator->() const // never throws
  {
    return get();
  }

  T* get() const // never throws
  {
    return ptr_;
  }

  operator bool() const // never throws
  {
    return get() != 0;
  }

  void swap(unique_ptr& b) // never throws
  {
    std::swap(ptr_, b.ptr_);
  }

private:
  unique_ptr(const unique_ptr&);
  unique_ptr& operator=(const unique_ptr&);

  T* ptr_;
};

#ifndef __SUNPRO_CC
template <typename T>
rv<T>& move(T& p)
{
  return static_cast<rv<T>&>(p);
}
#else
template <typename T, typename Deleter>
unique_ptr<T, Deleter>& move(unique_ptr<T, Deleter>& p)
{
  return p;
}
#endif

template <typename T, typename Deleter>
void swap(unique_ptr<T, Deleter>& a, unique_ptr<T, Deleter>& b) // never throws
{
  return a.swap(b);
}

} // namespace DCPS
} // namespace OpenDDS

OPENDDS_END_VERSIONED_NAMESPACE_DECL
#endif /* end of include guard: UNIQUE_PTR_H_18C6F30C */

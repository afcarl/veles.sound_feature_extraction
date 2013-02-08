/*! @file buffers_base.h
 *  @brief Base class for buffer formats which implements BufferFormat.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#ifndef BUFFERS_BASE_H_
#define BUFFERS_BASE_H_

#include "src/buffer_format.h"
#include "src/buffers.h"

namespace SpeechFeatureExtraction {

template <typename T>
class BufferFormatBase : public BufferFormat {
 public:
  typedef T ClassType;

  BufferFormatBase() noexcept
  : BufferFormat(typeid(T).name()) {
  }

  virtual std::function<void(void*)> Destructor() const noexcept {
    return [](void* ptr) {
      auto instance = reinterpret_cast<T*>(ptr);
      delete instance;
    };
  }
};

template <typename T>
class BuffersBase : public Buffers {
 public:
  template <typename... TArgs>
  explicit BuffersBase(int size, TArgs... args) noexcept
  : Buffers(size, BufferFormatBase<T>()) {
    for (int i = 0; i < Size(); i++) {
      Set(i, new T(args...));
    }
  }

  T* operator[](int index) noexcept {
    return reinterpret_cast<T*>(Buffers::operator [](index));
  }

  const T* operator[](int index) const noexcept {
    return reinterpret_cast<const T*>(Buffers::operator [](index));
  }

  const BufferFormat& Format() const {
    static const BufferFormatBase<T> bf;
    return bf;
  }
};

}  // namespace SpeechFeatureExtraction

#endif  // INCLUDE_BUFFERS_BASE_H_

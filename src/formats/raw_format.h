/*! @file raw_format.h
 *  @brief New file description.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#ifndef SRC_FORMATS_RAW_FORMAT_H_
#define SRC_FORMATS_RAW_FORMAT_H_

#include "src/config.h"
#include <memory>
#include "src/buffers_base.h"
#include "src/exceptions.h"
#include "src/formats/format_limits.h"
#include "src/primitives/memory.h"

namespace SpeechFeatureExtraction {
namespace Formats {

class InvalidRawFormatSizeException : public ExceptionBase {
 public:
  explicit InvalidRawFormatSizeException(size_t size)
  : ExceptionBase("Buffer size " + std::to_string(size) +
                  " is not supported or invalid.") {}
};

class InvalidRawFormatSamplingRateException : public ExceptionBase {
 public:
  explicit InvalidRawFormatSamplingRateException(int samplingRate)
  : ExceptionBase("Sampling rate " + std::to_string(samplingRate) +
                  " is not supported or invalid.") {}
};

template <typename T>
struct Raw {
  std::shared_ptr<T> Data;

#ifdef __AVX__
  explicit Raw(size_t size, int alignmentOffset)
  : Data(reinterpret_cast<T*>(malloc_aligned_offset(
      size * sizeof(T), alignmentOffset)),
      [=](T *ptr) {
    free(reinterpret_cast<char*>(ptr) - alignmentOffset);
  }) {
  }
#else
  explicit Raw(size_t size, int alignmentOffset UNUSED)
  : Data(reinterpret_cast<T*>(
      malloc_aligned(size * sizeof(T))),
      [](T *ptr) { free(ptr); }) {
  }
#endif

  Raw(const Raw& other) : Data(other.Data) {
  }

  explicit Raw(T* ptr) : Data(ptr, [](T* p) {}) {
  }

  Raw& operator=(const Raw& other) {
    Data = other.Data;
    return *this;
  }

  int AlignmentOffset() const {
#ifdef __AVX__
    return reinterpret_cast<uintptr_t>(Data.get()) % 32;
#else
    return 0;
#endif
  }
};

typedef Raw<int16_t> Raw16;
typedef Raw<int32_t> Raw32;
typedef Raw<float> RawF;

template <typename T>
class RawFormat : public BufferFormatBase<Raw<T>> {
 public:
  RawFormat() noexcept
  : size_(DEFAULT_SIZE)
  , samplingRate_(DEFAULT_SAMPLING_RATE) {
  }

  RawFormat(const RawFormat& other) noexcept
  : size_(other.size_)
  , samplingRate_(other.samplingRate_) {
  }

  RawFormat(size_t size, int samplingRate)
  : size_(size)
  , samplingRate_(samplingRate) {
    ValidateSize(size_);
    ValidateSamplingRate(samplingRate_);
  }

  BufferFormat& operator=(const BufferFormat& other) {
    if (other.Id() != RawFormat<T>::Id()) {
      throw InvalidFormatException(RawFormat<T>::Id(), other.Id());
    }
    *this = reinterpret_cast<const RawFormat&>(other);
    return *this;
  }

  int SamplingRate() const noexcept {
    return samplingRate_;
  }

  void SetSamplingRate(int value) {
    ValidateSamplingRate(value);
    samplingRate_ = value;
  }

  /// @brief Returns the raw buffer size in samples.
  size_t Size() const noexcept {
    return size_;
  }

  void SetSize(size_t value) {
    ValidateSize(value);
    size_ = value;
  }

 protected:
  virtual bool MustReallocate(const BufferFormatBase<Raw<T>>& other) {
    auto inst = reinterpret_cast<const RawFormat<T>&>(other);
    return inst.size_ < size_;
  }

 private:
  size_t size_;
  int samplingRate_;

  static void ValidateSize(size_t value) {
    if (value < MIN_RAW_SIZE || value > MAX_RAW_SIZE) {
      throw InvalidRawFormatSizeException(value);
    }
  }

  static void ValidateSamplingRate(int value) {
    if (value < MIN_SAMPLING_RATE || value > MAX_SAMPLING_RATE) {
      throw InvalidRawFormatSamplingRateException(value);
    }
  }
};

typedef RawFormat<int16_t> RawFormat16;
typedef RawFormat<int32_t> RawFormat32;
typedef RawFormat<float> RawFormatF;

}  // namespace Formats
}  // namespace SpeechFeatureExtraction
#endif  // SRC_FORMATS_RAW_FORMAT_H_

/*! @file array_format.h
 *  @brief Raw frames format.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright © 2013 Samsung R&D Institute Russia
 *
 *  @section License
 *  Licensed to the Apache Software Foundation (ASF) under one
 *  or more contributor license agreements.  See the NOTICE file
 *  distributed with this work for additional information
 *  regarding copyright ownership.  The ASF licenses this file
 *  to you under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in compliance
 *  with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing,
 *  software distributed under the License is distributed on an
 *  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, either express or implied.  See the License for the
 *  specific language governing permissions and limitations
 *  under the License.
 */

#ifndef SRC_FORMATS_ARRAY_FORMAT_H_
#define SRC_FORMATS_ARRAY_FORMAT_H_

#include <simd/memory.h>
#include <sstream>
#include "src/buffers_base.h"

namespace sound_feature_extraction {
namespace formats {

class InvalidDurationException : public ExceptionBase {
 public:
  explicit InvalidDurationException(float duration)
  : ExceptionBase("Duration " + std::to_string(duration) +
                  " is not supported or invalid.") {}
};

template <class T>
class ArrayFormat : public BufferFormatBase<T*>,
                    public FormatLogger<ArrayFormat<T>> {
 public:
  typedef T BufferElementType;

  ArrayFormat()
      : size_(0) {
  }

  ArrayFormat(size_t size, int samplingRate)
      : BufferFormatBase<T*>(samplingRate),
        size_(size) {
  }

  ArrayFormat(const ArrayFormat& other)
      : BufferFormatBase<T*>(other),
        size_(other.size_) {
  }

  virtual ~ArrayFormat() noexcept {
  }

  BufferFormat& operator=(const BufferFormat& other) {
    if (other.Id() != ArrayFormat<T>::Id()) {
      throw InvalidFormatException(ArrayFormat<T>::Id(), other.Id());
    }
    *this = reinterpret_cast<const ArrayFormat&>(other);
    return *this;
  }

  /// @brief Gets the time for which the signal lasts according to the sampling
  /// rate.
  /// @note This method does not always make sense, of course.
  float Duration() const noexcept {
    return size_ * 2.f / this->SamplingRate();
  }

  /// @brief Sets the signal length according to the sampling rate and the
  /// desired time for which it should last.
  /// @note This method does not always make sense, of course.
  void SetDuration(float value) {
    ValidateDuration(value);
    size_ = value * this->SamplingRate() / 2;
  }

  /// @brief Returns the current buffer size in data units (sizeof(T)).
  size_t Size() const noexcept {
    return size_;
  }

  void SetSize(size_t value) {
    size_ = value;
  }

  virtual size_t UnalignedSizeInBytes() const noexcept override {
    return size_ * sizeof(T);
  }

  static const int kMinSamplesCount = 32;
  static const int kMaxSamplesCount = (1 << 30);

 protected:
  virtual void Validate(const BuffersBase<T*>& buffers) const {
    for (size_t i = 0; i < buffers.Count(); i++) {
      bool allZeros = true;
      for (size_t j = 0; j < size_; j++) {
        T value = buffers[i][j];
        if (!validation::Validator<T>::Validate(value)) {
          throw InvalidBuffersException(this->Id(), i,
                                        std::string("[") + std::to_string(j) +
                                        "] = " + std::to_string(value));
        }
        allZeros &= (value == 0);
      }
      if (allZeros) {
        WRN("%s", InvalidBuffersException(this->Id(), i, "all zeros").what());
      }
    }
  }

  virtual std::string Dump(const BuffersBase<T*>& buffers, size_t index)
      const noexcept override {
    std::stringstream ret;
    ret << "----";
    ret << std::to_string(index) << "----\n";
    for (size_t j = 0; j < size_; j++) {
      auto strval = std::to_string(buffers[index][j]);
      int size_limit = 24;
      if (strval[0] != '-') {
        ret << ' ';
        size_limit--;
      }
      int skip_size = size_limit - strval.size();
      if (skip_size > 0) {
        ret << strval + std::string(skip_size, ' ');
      } else {
        ret << strval;
      }
      if (((j + 1) % 10) == 0) {
        ret << "\n";
      }
    }
    ret << "\n----------------\n";
    return ret.str();
  }

  virtual std::string ToString() const noexcept override {
    return this->Id() + " of size " + std::to_string(Size());
  }

  static constexpr size_t MIN_DURATION() {
    return 1 + kMinSamplesCount * 1000 /
        BufferFormatBase<T*>::MAX_SAMPLING_RATE;
  }

  static constexpr size_t MAX_DURATION() {
    return kMaxSamplesCount * 1000 /
        BufferFormatBase<T*>::MIN_SAMPLING_RATE;
  }

 private:
  size_t size_;

  static void ValidateDuration(size_t value) {
    if (value < MIN_DURATION() || value > MAX_DURATION()) {
      throw InvalidDurationException(value);
    }
  }
};

typedef ArrayFormat<int16_t> ArrayFormat16;
typedef ArrayFormat<int32_t> ArrayFormat32;
typedef ArrayFormat<float> ArrayFormatF;

}  // namespace formats
}  // namespace sound_feature_extraction
#endif  // SRC_FORMATS_ARRAY_FORMAT_H_

/*! @file unpack_rdft.cc
 *  @brief Unpacks data after applying RDFT.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#include "src/transforms/unpack_rdft.h"

namespace sound_feature_extraction {
namespace transforms {

size_t UnpackRDFT::OnFormatChanged(size_t buffersCount) {
  if (input_format_->Size() % 2 == 1) {
    output_format_->SetSize((input_format_->Size() - 1) * 2);
  } else {
    output_format_->SetSize((input_format_->Size() - 2) * 2);
  }
  return buffersCount;
}

void UnpackRDFT::Initialize() const {
  realMode_ = input_format_->Size() % 2 == 1;
  offset_ = input_format_->Size();
  length_ = output_format_->Size() - offset_;
}

void UnpackRDFT::Do(const float* in,
                    float* out) const noexcept {
  if (in != out) {
    memcpy(out, in, offset_ * sizeof(in[0]));
  }
  if (realMode_) {
    rmemcpyf(out + offset_, in + 1, length_);
  } else {
    crmemcpyf(out + offset_, in + 2, length_);
  }
}

REGISTER_TRANSFORM(UnpackRDFT);

}  // namespace transforms
}  // namespace sound_feature_extraction

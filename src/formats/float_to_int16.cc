/*! @file float_to_int16.cc
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

#include "src/formats/float_to_int16.h"
#include "src/primitives/arithmetic-inl.h"

namespace SpeechFeatureExtraction {
namespace Formats {

void FloatToInt16Raw::TypeSafeDo(const BuffersBase<RawF>& in,
                                 BuffersBase<Raw16> *out) const noexcept {
  for (size_t i = 0; i < in.Size(); i++) {
    float_to_int16(in[i]->Data.get(), inputFormat_->Size(),
                   (*out)[i]->Data.get());
  }
}

void FloatToInt16Window::TypeSafeDo(
    const BuffersBase<WindowF>& in,
    BuffersBase<Window16> *out) const noexcept {
  for (size_t i = 0; i < in.Size(); i++) {
    float_to_int16(in[i]->Data.get(), inputFormat_->SamplesCount(),
                   (*out)[i]->Data.get());
  }
}

REGISTER_TRANSFORM(FloatToInt16Raw);
REGISTER_TRANSFORM(FloatToInt16Window);

}  // namespace Formats
}  // namespace SpeechFeatureExtraction
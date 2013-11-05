/*! @file int16_to_int32.cc
 *  @brief int16 to int32 converter.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#include "src/formats/int16_to_int32.h"
#include <simd/arithmetic-inl.h>

namespace sound_feature_extraction {
namespace formats {

void Int16ToInt32Raw::Do(const int16_t* in,
                         int32_t* out) const noexcept {
  int16_to_int32(in, input_format_->Size(), out);
}

REGISTER_TRANSFORM(Int16ToInt32Raw);

}  // namespace formats
}  // namespace sound_feature_extraction

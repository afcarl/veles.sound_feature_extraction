/*! @file single_converters.h
 *  @brief Converters for SingleFormat.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#ifndef SRC_FORMATS_SINGLE_CONVERTERS_H_
#define SRC_FORMATS_SINGLE_CONVERTERS_H_

#include "src/format_converter_base.h"
#include "src/formats/single_format.h"

namespace SoundFeatureExtraction {

template <typename FIN, typename FOUT>
class SingleFormatConverterBase : public FormatConverterBase<FIN, FOUT> {
 protected:
  virtual BuffersCountChange OnInputFormatChanged() override final {
   return BuffersCountChange::Identity;
  }
};

namespace Formats {

class Int32ToFloatConverter
    : public SingleFormatConverterBase<SingleFormat32, SingleFormatF> {
 protected:
  virtual void Do(const int32_t& in,
                  float *out) const noexcept override;
};

class FloatToInt32Converter
    : public SingleFormatConverterBase<SingleFormatF, SingleFormat32> {
 protected:
  virtual void Do(const float& in,
                  int32_t *out) const noexcept override;
};

}  // namespace Formats
}  // namespace SoundFeatureExtraction

#endif  // SRC_FORMATS_SINGLE_CONVERTERS_H_
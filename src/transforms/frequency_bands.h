/*! @file frequency_bands.h
 *  @brief Cut the frequency bands from a signal.
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

#ifndef SRC_TRANSFORMS_FREQUENCY_BANDS_H_
#define SRC_TRANSFORMS_FREQUENCY_BANDS_H_

#include <vector>
#include "src/transforms/iir_filter_base.h"
#include "src/transforms/fork.h"

namespace sound_feature_extraction {

typedef std::vector<int> FilterOrders;

FilterOrders Parse(const std::string& value, identity<FilterOrders>);

}  // namespace sound_feature_extraction

namespace std {
  inline string
  to_string(const sound_feature_extraction::FilterOrders& fo) noexcept {
    if (fo.size() == 0) {
      return "auto";
    }
    std::string str;
    for (int i : fo) {
      str += std::to_string(i) + " ";
    }
    if (!str.empty()) {
      str.resize(str.size() - 1);
    }
    return str;
  }
}  // namespace std

namespace sound_feature_extraction {
namespace transforms {

class FrequencyBands
    : public UniformFormatOmpAwareTransform<formats::ArrayFormatF>,
      public TransformLogger<FrequencyBands> {
 public:
  FrequencyBands();

  TRANSFORM_INTRO("FrequencyBands",
                  "Cut the frequency bands from sequential windows.",
                  FrequencyBands)

  TP(number, int, kDefaultBandsNumber,
     "The number of bands. It should be the same as the cloning "
     "factor of Fork. You may set the bands configuration by hand "
     "with \"bands\" parameter, \"number\" will be ignored then.")
  TP(bands, std::string, "", "Bands configuration. Overrides \"number\".")
  TP(filter, IIRFilterType, kDefaultFilterType, "IIR filter type to apply.")
  TP(lengths, FilterOrders, FilterOrders(),
     "IIR filter orders. \"auto\" for automatic selection.")

  virtual bool BufferInvariant() const noexcept override final {
    return false;
  }

  virtual void Initialize() const override;

 protected:
  static constexpr IIRFilterType kDefaultFilterType =
      IIRFilterType::kChebyshevII;
  static constexpr int kDefaultBandsNumber = Fork::kDefaultFactor;

  virtual void Do(const BuffersBase<float*>& in,
                  BuffersBase<float*>* out) const noexcept override;
  void SetupFilter(size_t index, int frequency, IIRFilterBase* filter) const;

  const std::vector<std::shared_ptr<IIRFilterBase>>& filters() const;

 private:
  mutable std::vector<std::shared_ptr<IIRFilterBase>> filters_;
};

}  // namespace transforms
}  // namespace sound_feature_extraction

#endif  // SRC_TRANSFORMS_FREQUENCY_BANDS_H_

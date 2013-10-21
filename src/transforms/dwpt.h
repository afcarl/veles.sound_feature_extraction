/*! @file dwpt.h
 *  @brief Discrete Wavelet Packet Transform (see http://en.wikipedia.org/wiki/Wavelet_packet_decomposition).
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#ifndef SRC_TRANSFORMS_DWPT_H_
#define SRC_TRANSFORMS_DWPT_H_

#include <vector>
#include <simd/wavelet_types.h>
#include "src/transforms/common.h"
#include "src/primitives/wavelet_filter_bank.h"

namespace SoundFeatureExtraction {
namespace Transforms {

/// @brief Discrete Wavelet Packet Transform.
/// @details Here is an example of the "tree" parameter:
///
///             ------ 3
///             |
///          ---|
///          |  |
///          |  ------ 3
///       ---|
///       |  |  ------ 3
///       |  |  |
///       |  ---|
///       |     |  --- 4
///     --|     ---|               "3, 3, 3, 4, 4, 2, 4, 4, 3"
///       |        --- 4
///       |  --------- 2
///       |  |
///       |  |
///       ---|     --- 4
///          |  ---|
///          |  |  --- 4
///          ---|
///             |
///             ------ 3
///
class DWPT : public OmpUniformFormatTransform<Formats::ArrayFormatF> {
 public:
  DWPT();

  TRANSFORM_INTRO("DWPT", "Discrete Wavelet Packet Transform")

  OMP_TRANSFORM_PARAMETERS(
    TP("tree", "The wavelet packet binary tree fingerprint.",
       Primitives::WaveletFilterBank::DescriptionToString(
           kDefaultTreeFingerprint))
    TP("type", "The type of the wavelet to apply. Supported values are "
               "daub (Daubechies), coif (Coiflet) and sym (Symlet).",
       kDefaultWaveletType)
    TP("order", "The number of coefficients in the wavelet.",
        std::to_string(kDefaultWaveletOrder))
  )

  virtual void Initialize() const override;

 protected:
  virtual size_t OnFormatChanged(size_t buffersCount) override;

  virtual void Do(const float* in,
                  float* out) const noexcept override;

 private:
  static const std::vector<int> kDefaultTreeFingerprint;
  static const std::string kDefaultWaveletType;
  static const WaveletType kDefaultWaveletTypeEnum;
  static const int kDefaultWaveletOrder;

  std::vector<int> treeFingerprint_;
  WaveletType waveletType_;
  int waveletOrder_;
  mutable std::unique_ptr<Primitives::WaveletFilterBank> filterBank_;
};

}  // namespace Transforms
}  // namespace SoundFeatureExtraction
#endif  // SRC_TRANSFORMS_DWPT_H_

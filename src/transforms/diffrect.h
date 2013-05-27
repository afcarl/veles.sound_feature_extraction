/*! @file diffrect.h
 *  @brief Find the difference from one sample to the next.
 *  @author Shapichev Alexey <a.shapichev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */

#ifndef SRC_TRANSFORMS_DIFFRECT_H_
#define SRC_TRANSFORMS_DIFFRECT_H_

#include "src/formats/window_format.h"
#include "src/omp_transform_base.h"

namespace SoundFeatureExtraction {
namespace Transforms {

class Diffrect
    : public OmpUniformFormatTransform<Formats::WindowFormatF> {
 public:
  TRANSFORM_INTRO("Diffrect", "Find the difference from one sample to the next.")

  OMP_TRANSFORM_PARAMETERS()

 protected:
  virtual void OnFormatChanged();

  virtual void InitializeBuffers(
      const BuffersBase<Formats::WindowF>& in,
      BuffersBase<Formats::WindowF>* buffers) const noexcept;

  virtual void Do(const Formats::WindowF& in,
                  Formats::WindowF* out) const noexcept;

  static void Do(bool simd, const float* input, int length,
                 float* output) noexcept;
};

}  // namespace Transforms
}  // namespace SoundFeatureExtraction
#endif  // SRC_TRANSFORMS_DIFFRECT_H_

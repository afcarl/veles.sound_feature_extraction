/*! @file stretch.cc
 *  @brief Tests for sound_feature_extraction::transforms::Stretch.
 *  @author Markovtsev Vadim <v.markovtsev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright 2013 Samsung R&D Institute Russia
 */


#include "src/transforms/stretch.h"
#include "tests/transforms/transform_test.h"

using sound_feature_extraction::formats::ArrayFormatF;
using sound_feature_extraction::BuffersBase;
using sound_feature_extraction::transforms::Stretch;

class StretchTest : public TransformTest<Stretch> {
 public:
  int Size;

  virtual void SetUp() {
    Size = 6;
    SetUpTransform(1, Size, 18000);
    for (int i = 0; i < Size; i++) {
      (*Input)[0][i] = i;
    }
  }
};

TEST_F(StretchTest, Do) {
  set_factor(1);
  Do((*Input)[0], (*Output)[0]);
  for (int i = 0; i < Size ; i++) {
    ASSERT_FLOAT_EQ((*Input)[0][i], (*Output)[0][i]) << i;
  }
  set_factor(2);
  SetUpTransform(1, Size, 18000);
  Do((*Input)[0], (*Output)[0]);
  for (int i = 0; i < Size ; i++) {
    ASSERT_FLOAT_EQ((*Input)[0][i], (*Output)[0][i * 2]) << i;
  }
  set_factor(10);
  SetUpTransform(1, Size, 18000);
  Do((*Input)[0], (*Output)[0]);
  for (int i = 0; i < Size ; i++) {
    ASSERT_FLOAT_EQ((*Input)[0][i], (*Output)[0][i * 10]) << i;
  }
}
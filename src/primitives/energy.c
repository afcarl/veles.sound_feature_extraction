/*! @file energy.c
 *  @brief Low level energy calculation.
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

#include "src/primitives/energy.h"
#include <simd/instruction_set.h>
#include <simd/memory.h>

float calculate_energy(int simd, int norm, const float *signal, size_t length) {
  float energy = 0.f;
  int ilength = (int)length;
  if (simd) {
#ifdef __AVX__
    __m256 accum = _mm256_setzero_ps();
    int startIndex = align_complement_f32(signal);
    for (int j = 0; j < startIndex; j++) {
      float val = signal[j];
      energy += val * val;
    }

    for (int j = startIndex; j < ilength - 7; j += 8) {
      __m256 vec = _mm256_load_ps(signal + j);
      vec = _mm256_mul_ps(vec, vec);
      accum = _mm256_add_ps(accum, vec);
    }
    accum = _mm256_hadd_ps(accum, accum);
    accum = _mm256_hadd_ps(accum, accum);
    energy += _mm256_get_ps(accum, 0) + _mm256_get_ps(accum, 4);

    for (int j = startIndex + ((ilength - startIndex) & ~0x7);
        j < ilength; j++) {
      float val = signal[j];
      energy += val * val;
    }
  } else {
#elif defined(__ARM_NEON__)
    float32x4_t accum = { 0.f };
    for (int j = 0; j < ilength - 3; j += 4) {
      float32x4_t vec = vld1q_f32(signal + j);
      accum = vmlaq_f32(accum, vec, vec);
    }
    float32x2_t sums = vpadd_f32(vget_high_f32(accum), vget_low_f32(accum));
    energy += vget_lane_f32(sums, 0) + vget_lane_f32(sums, 1);
    for (int j = (ilength & ~0x3); j < ilength; j++) {
      float val = signal[j];
      energy += val * val;
    }
  } else {
#else
  } {
#endif
    for (int j = 0; j < ilength; j++) {
      float val = signal[j];
      energy += val * val;
    }
  }
  return norm? energy / length : energy;
}

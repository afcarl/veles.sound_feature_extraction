/*! @file memory_protector.cc
 *  @brief Write memory protection based on mprotect().
 *  @author Vadim Markovtsev <v.markovtsev@samsung.com>
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

#include "src/memory_protector.h"
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <unistd.h>
#include <sys/mman.h>

namespace sound_feature_extraction {

MemoryProtector::MemoryProtector(const void* cptr, size_t size) noexcept
    : page_(nullptr), size_(0) {
  // mprotect() requires void*, not const void*
  auto ptr = const_cast<char*>(reinterpret_cast<const char*>(cptr));
  auto rem = reinterpret_cast<uintptr_t>(cptr) % PageSize();
  if (rem > 0) {
    auto offset = PageSize() - rem;
    if (offset > size) {
      // too small memory block, cannot protect
      return;
    }
    ptr += offset;
    size -= offset;
  }
  page_ = ptr;
  auto num_pages = size / PageSize();
  if (num_pages < 1) {
    // too small memory block, cannot protect
    return;
  }
  size_ = num_pages * PageSize();
  int res = mprotect(page_, size_, PROT_READ);
  if (res != 0) {
    fprintf(stderr, "mprotect(%p, %zu, PROT_READ) failed with errno %d\n",
            page_, size_, errno);
    size_ = 0;
  }
}

void* MemoryProtector::page() const noexcept {
  return page_;
}

size_t MemoryProtector::size() const noexcept {
  return size_;
}

long int MemoryProtector::PageSize() noexcept {
  static auto page_size = sysconf(_SC_PAGE_SIZE);
  return page_size;
}

MemoryProtector::~MemoryProtector() noexcept {
  if (page_ != nullptr) {
    int res = mprotect(page_, size_, PROT_READ | PROT_WRITE);
    if (res != 0) {
      fprintf(stderr, "mprotect(%p, %zu, PROT_READ | PROT_WRITE) failed with "
                      "errno %d\n",
              page_, size_, errno);
    }
  }
}

}  // namespace sound_feature_extraction

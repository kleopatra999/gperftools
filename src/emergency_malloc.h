// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*-
// Copyright (c) 2014, gperftools Contributors
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef EMERGENCY_MALLOC_H
#define EMERGENCY_MALLOC_H
#include "config.h"
#include <stddef.h>

namespace tcmalloc {

  extern char *emergency_arena_start;
  extern ptrdiff_t emergency_arena_size;

  PERFTOOLS_DLL_DECL void *EmergencyMalloc(size_t size);
  PERFTOOLS_DLL_DECL void EmergencyFree(void *p);
  // NOTE: the following must hold: old_ptr != NULL && new_size != 0
  PERFTOOLS_DLL_DECL void *EmergencyDoRealloc(void *old_ptr, size_t new_size);

  static inline bool IsEmergencyPtr(void *_ptr) {
    if (emergency_arena_start == NULL) {
      return false;
    }
    char *ptr = static_cast<char *>(_ptr);
    ptrdiff_t diff = ptr - emergency_arena_start;
    return (diff >= 0 && diff <= emergency_arena_size);
  }

  static inline bool TryEmergencyFree(void *ptr) {
    if (!IsEmergencyPtr(ptr)) {
      return false;
    }
    EmergencyFree(ptr);
    return true;
  }

  // NOTE: higher layers of malloc already checked old_ptr for NULL
  // and new_size for 0
  // static inline bool TryEmergencyRealloc(void *old_ptr, size_t new_size, void **p_new_ptr) {
  //   CHECK_CONDITION(old_ptr != 0);
  //   CHECK_CONDITION(new_size != 0);
  //   if (!IsEmergencyPtr(ptr)) {
  //     return false;
  //   }
  //   *p_new_ptr = EmergencyDoRealloc(old_ptr, new_size);
  //   return true;
  // }

} // namespace tcmalloc

#endif

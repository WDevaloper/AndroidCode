/*
 * Copyright (C) 2018 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

/** WARNING WARNING WARNING
 **
 ** This header file is *NOT* part of the public Bionic ABI/API and should not
 ** be used/included by user-serviceable parts of the system (e.g.
 ** applications).
 **
 ** It is only provided here for the benefit of Android components that need a
 ** pre-allocated slot for performance reasons (including ART, the OpenGL
 ** subsystem, and sanitizers).
 **/

// Bionic TCB / TLS slots:
//
//  - TLS_SLOT_SELF: On x86-{32,64}, the kernel makes TLS memory available via
//    the gs/fs segments. To get the address of a TLS variable, the first slot
//    of TLS memory (accessed using %gs:0 / %fs:0) holds the address of the
//    gs/fs segment. This slot is used by:
//     - OpenGL and compiler-rt
//     - Accesses of x86 ELF TLS variables
//
//  - TLS_SLOT_OPENGL and TLS_SLOT_OPENGL_API: These two aren't used by bionic
//    itself, but allow the graphics code to access TLS directly rather than
//    using the pthread API.
//
//  - TLS_SLOT_STACK_GUARD: Used for -fstack-protector by:
//     - Clang targeting Android/arm64
//     - gcc targeting Linux/x86-{32,64}
//
//  - TLS_SLOT_SANITIZER: Lets sanitizers avoid using pthread_getspecific for
//    finding the current thread state.
//
//  - TLS_SLOT_DTV: Pointer to ELF TLS dynamic thread vector.
//
//  - TLS_SLOT_ART_THREAD_SELF: Fast storage for Thread::Current() in ART.
//
//  - TLS_SLOT_BIONIC_TLS: Optimizes accesses to bionic_tls by one load versus
//    finding it using __get_thread().
//
//  - TLS_SLOT_APP: Available for use by apps in API level 29 and later.
//    (This slot was used for errno in API level 28 and earlier.)
//
//  - TLS_SLOT_NATIVE_BRIDGE_GUEST_STATE: Pointer to the guest state for native
//    bridge implementations. It is (to be) used by debuggerd to access this
//    state for guest aware crash reporting of the binary translated code.
//    (Introduced in V)

#if defined(__arm__) || defined(__aarch64__)

// The ARM ELF TLS ABI specifies[1] that the thread pointer points at a 2-word
// TCB followed by the executable's TLS segment. Both the TCB and the
// executable's segment are aligned according to the segment, so Bionic requires
// a minimum segment alignment, which effectively reserves an 8-word TCB. The
// ARM spec allocates the first TCB word to the DTV.
//
// [1] "Addenda to, and Errata in, the ABI for the ARM Architecture". Section 3.
// http://infocenter.arm.com/help/topic/com.arm.doc.ihi0045e/IHI0045E_ABI_addenda.pdf

#define MIN_TLS_SLOT (-3)  // update this value when reserving a slot
#define TLS_SLOT_STACK_MTE (-3)
#define TLS_SLOT_NATIVE_BRIDGE_GUEST_STATE (-2)
#define TLS_SLOT_BIONIC_TLS     (-1)
#define TLS_SLOT_DTV              0
#define TLS_SLOT_THREAD_ID        1
#define TLS_SLOT_APP              2 // was historically used for errno
#define TLS_SLOT_OPENGL           3
#define TLS_SLOT_OPENGL_API       4
#define TLS_SLOT_STACK_GUARD      5
#define TLS_SLOT_SANITIZER        6 // was historically used for dlerror
#define TLS_SLOT_ART_THREAD_SELF  7

// The maximum slot is fixed by the minimum TLS alignment in Bionic executables.
#define MAX_TLS_SLOT              7

#elif defined(__i386__) || defined(__x86_64__)

// x86 uses variant 2 ELF TLS layout, which places the executable's TLS segment
// immediately before the thread pointer. New slots are allocated at positive
// offsets from the thread pointer.

#define MIN_TLS_SLOT              0

#define TLS_SLOT_SELF             0
#define TLS_SLOT_THREAD_ID        1
#define TLS_SLOT_APP              2 // was historically used for errno
#define TLS_SLOT_OPENGL           3
#define TLS_SLOT_OPENGL_API       4
#define TLS_SLOT_STACK_GUARD      5
#define TLS_SLOT_SANITIZER        6 // was historically used for dlerror
#define TLS_SLOT_ART_THREAD_SELF  7
#define TLS_SLOT_DTV              8
#define TLS_SLOT_BIONIC_TLS       9
#define TLS_SLOT_NATIVE_BRIDGE_GUEST_STATE 10
#define MAX_TLS_SLOT 10  // update this value when reserving a slot

#elif defined(__riscv)

// RISC-V ELF Specification[1] specifies that RISC-V uses Variant I as described
// by the ELF TLS specification, with tp containing the address one past the end
// of the TCB.
//
// [1]: RISC-V ELF Specification. Section: Thread Local Storage
// https://github.com/riscv-non-isa/riscv-elf-psabi-doc/blob/master/riscv-elf.adoc#thread-local-storage

#define MIN_TLS_SLOT (-10)  // update this value when reserving a slot

#define TLS_SLOT_NATIVE_BRIDGE_GUEST_STATE (-10)
#define TLS_SLOT_BIONIC_TLS      (-9)
#define TLS_SLOT_DTV             (-8)
#define TLS_SLOT_THREAD_ID       (-7)
#define TLS_SLOT_APP             (-6)
#define TLS_SLOT_OPENGL          (-5)
#define TLS_SLOT_OPENGL_API      (-4)
#define TLS_SLOT_STACK_GUARD     (-3)
#define TLS_SLOT_SANITIZER       (-2)
#define TLS_SLOT_ART_THREAD_SELF (-1)
#define MAX_TLS_SLOT             (-1)

#endif

#define BIONIC_TLS_SLOTS (MAX_TLS_SLOT - MIN_TLS_SLOT + 1)

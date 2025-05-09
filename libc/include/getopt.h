/*	$NetBSD: getopt.h,v 1.4 2000/07/07 10:43:54 ad Exp $	*/
/*	$FreeBSD$ */

/*-
 * Copyright (c) 2000 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Dieter Baron and Thomas Klausner.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

/**
 * @file getopt.h
 * @brief The getopt() and getopt_long() functions.
 */

#include <sys/cdefs.h>

#include <bits/getopt.h>

/** A `has_arg` value for `struct option`. */
#define no_argument 0
/** A `has_arg` value for `struct option`. */
#define required_argument 1
/** A `has_arg` value for `struct option`. */
#define optional_argument 2

struct option {
  /**
   * Name of long option. Options must have a non-NULL name.
   * A NULL name signals the end of the options array.
   */
  const char * _Nullable name;

  /**
   * One of `no_argument`, `required_argument`, or `optional_argument`.
   */
  int has_arg;

  /** If not NULL, set `*flag` to val when option found. */
  int* _Nullable flag;

  /** If `flag` not NULL, the value to assign to `*flag`; otherwise the return value. */
  int val;
};

__BEGIN_DECLS

/**
 * [getopt_long(3)](https://man7.org/linux/man-pages/man3/getopt_long.3.html) parses command-line options.
 */
int getopt_long(int __argc, char* _Nonnull const* _Nonnull __argv, const char* _Nonnull __options, const struct option* _Nonnull __long_options, int* _Nullable __long_index);

/**
 * [getopt_long_only(3)](https://man7.org/linux/man-pages/man3/getopt_long_only.3.html) parses command-line options.
 */
int getopt_long_only(int __argc, char* _Nonnull const* _Nonnull __argv, const char* _Nonnull __options, const struct option* _Nonnull __long_options, int* _Nullable __long_index);

#ifndef _OPTRESET_DECLARED
#define _OPTRESET_DECLARED
/**
 * Must be set to 1 to reset the `getopt` functions before scanning a new argument vector.
 */
extern int optreset;
#endif

__END_DECLS

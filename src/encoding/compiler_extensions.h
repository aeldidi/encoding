// SPDX-License-Identifier: 0BSD
// Copyright (C) 2022 Ayman El Didi
#ifndef COMPILER_EXTENSIONS_H
#define COMPILER_EXTENSIONS_H

// This file contains commonly implemented extensions which we make use of.

#if !defined(ENCODING_PUBLIC)
#define ENCODING_PUBLIC
#endif

// Finding the C version

#if !defined(ENCODING_C23)
#define ENCODING_C23 0
#endif

#if !defined(ENCODING_C11)
#define ENCODING_C11 0
#endif

#if defined(__STDC_VERSION__)
#if __STDC_VERSION__ >= 201112L
#undef ENCODING_C11
#define ENCODING_C11 1
#endif

#if __STDC_VERSION__ >= 202311L
#undef ENCODING_C23
#define ENCODING_C23 1
#endif
#endif // defined(__STDC_VERSION__)

#ifndef ENCODING_CPP17
#define ENCODING_CPP17 0
#endif

#if defined(__cplusplus)
#if __cplusplus >= 201703L
#undef ENCODING_CPP17
#define ENCODING_CPP17 1
#endif
#endif

// Finding supported attributes

#if !defined(UNLIKELY)
#define UNLIKELY(expr) (expr)
#if defined(__has_builtin)
#if __has_builtin(__builtin_expect)
#undef UNLIKELY
#define UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#endif
#endif // defined(__has_builtin)
#endif // !defined(UNLIKELY)

#endif // COMPILER_EXTENSIONS_H

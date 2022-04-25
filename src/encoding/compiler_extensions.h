// SPDX-License-Identifier: BSL-1.0
// Copyright (C) 2022 Ayman El Didi
#ifndef COMPILER_EXTENSIONS_H
#define COMPILER_EXTENSIONS_H

// This file contains commonly implemented extensions which we make use of.

#define unlikely(expr) (expr)

#if !defined(__has_builtin)
#else

#if __has_builtin(__builtin_expect)
#undef unlikely
#define unlikely(expr) __builtin_expect(!!(expr), 0)
#endif

#endif // !defined(__has_builtin)

#endif // COMPILER_EXTENSIONS_H
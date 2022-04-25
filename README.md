# `libencoding`

`libencoding` is a library for encoding and decoding binary data in various
formats. It's written in C but doesn't make use of the C runtime, meaning it
should be usable in any situation.

## Status

The following operations are supported:

- encoding, decoding, and validating UTF-8
- encoding, decoding, and validating UTF-16
- encoding, decoding, and validating base16 (encoding/hex), base32, and base64
- encoding and decoding binary data in big or little endian byte order

This library can be considered stable. Any changes will be backwards
compatible, and none are currently planned.

## Documentation

Each interface is documented in its respective header file. For example, the
documentation for `base64_encode` is in a comment above the function prototype
in `include/encoding/base64.h`.

## Installing `libencoding`

`libencoding` uses the [Meson build system](https://mesonbuild.com), but is
trivial to build by hand.

### Using `pkg-config`

If already installed, `libencoding` can be used the same way any library
supporting `pkg-config` is used. The module name is `encoding`.

### Using Meson

The following would go in your `meson.build` file:

```
encoding = subproject('encoding')
encoding_dep = encoding.get_variable('encoding_dep')
```

`libencoding` can then be used as a dependency by adding it to your
`executable()` or `library()` declaration by adding `encoding_dep` to the
`dependencies` section.

### Using Any Other Build System

Simply build and link with each file in `src/` and add `include/` to your
include directory.

All defined symbols are prefixed with the name of the encoding they operate on,
so no conflicts should occur by just doing the above steps. For example, the
function to decode UTF-8 is called `utf8_decode`.

## Security Model

The lengths of all arrays and strings are passed explicitly to avoid buffer
overruns.

When developing the library I considered embedding correctness checks in each
encoding and decoding function which would then stop the operation and zero out
any partially encoded/decoded memory. As a result of this performance suffered
significantly, making the library unnecessarily slower when processing trusted
input. As such, I refactored the library in the following way:

1. All input is assumed to be trusted, and encoding/decoding routines are
   optimized taking this into account.
2. Functions are provided to test the validity of any untrusted input. 

For example, any buffer which `utf8_valid` returns true for is safe to decode
using the `utf8_decode` function. This keeps the library fast when operating on
trusted input, while also causing untrusted input to fail early.

## License

`libencoding` is licensed under the Boost Software License, Version 1.0. See
the included `LICENSE` file or https://www.boost.org/LICENSE_1_0.txt for more
details.

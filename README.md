# `aeldidi/encoding`

`aeldidi/encoding` is a library for encoding and decoding binary data in various
formats. It's written in C but doesn't make use of the C runtime except for
`assert`, meaning it should be usable in any situation. It's also been tested
to build using C++ compilers.

## Status

The following operations are supported:

- encoding, decoding, and validating UTF-8
- encoding, decoding, and validating UTF-16
- encoding, decoding, and validating base16 (hex.c/hex.h), base32, and base64
- encoding and decoding binary data in big or little endian byte order

## Documentation

Each interface is documented in its respective header file. For example, the
documentation for `base64_encode` is in a comment above the function prototype
in `base64.h`.

## Using `aeldidi/encoding`

Go to the [releases](https://github.com/aeldidi/encoding/releases) page and
download the latest release, which contains self-contained `.c` and `.h` files
which you can compile and use.

Note that `binary.h` only contains `static` functions, and you include it
directly.

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

`aeldidi/encoding` is licensed under the 0BSD License, which allows you to do
whatever you want without restriction. See the included `LICENSE` file more
details.

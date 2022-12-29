valid = executable(
    'valid',
    ['valid.c', amalgamate_srcs],
    include_directories: incdirs,
    build_by_default: false,
)
test(
    'valid',
    valid,
    suite: 'utf16',
)

codepoint_encode = executable(
    'codepoint_encode',
    ['codepoint_encode.c', amalgamate_srcs],
    include_directories: incdirs,
    build_by_default: false,
)
test(
    'codepoint_encode',
    codepoint_encode,
    suite: 'utf16',
)

codepoint_decode = executable(
    'codepoint_decode',
    ['codepoint_decode.c', amalgamate_srcs],
    include_directories: incdirs,
    build_by_default: false,
)
test(
    'codepoint_decode',
    codepoint_decode,
    suite: 'utf16',
)

encoded_length = executable(
    'encoded_length',
    ['encoded_length.c', amalgamate_srcs],
    include_directories: incdirs,
    build_by_default: false,
)
test(
    'encoded_length',
    encoded_length,
    suite: 'utf16',
)

decoded_length = executable(
    'decoded_length',
    ['decoded_length.c', amalgamate_srcs],
    include_directories: incdirs,
    build_by_default: false,
)
test(
    'decoded_length',
    decoded_length,
    suite: 'utf16',
)

decode = executable(
    'decode',
    ['decode.c', amalgamate_srcs],
    include_directories: incdirs,
    build_by_default: false,
)
test(
    'decode',
    decode,
    suite: 'utf16',
)

encode = executable(
    'encode',
    ['encode.c', amalgamate_srcs],
    include_directories: incdirs,
    build_by_default: false,
)
test(
    'encode',
    encode,
    suite: 'utf16',
)

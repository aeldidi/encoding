valid = executable(
    'valid',
    ['valid.c', amalgamate_srcs],
    include_directories: incdirs,
    build_by_default: false,
)
test(
    'valid',
    valid,
    suite: 'utf8',
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
    suite: 'utf8',
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
    suite: 'utf8',
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
    suite: 'utf8',
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
    suite: 'utf8',
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
    suite: 'utf8',
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
    suite: 'utf8',
)

encode = executable(
    'encode',
    ['encode.c', amalgamate_srcs],
    include_directories: incdirs,
    build_by_default: false,
)
test(
    'encode',
    encode,
    suite: 'base64',
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
    suite: 'base64',
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
    suite: 'base64',
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
    suite: 'base64',
)

valid = executable(
    'valid',
    ['valid.c', amalgamate_srcs],
    include_directories: incdirs,
    build_by_default: false,
)
test(
    'valid',
    valid,
    suite: 'base64',
)

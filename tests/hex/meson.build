valid = executable(
    'valid',
    ['valid.c', amalgamate_srcs],
    include_directories: incdirs,
    build_by_default: false,
)
test(
    'valid',
    valid,
    suite: 'hex',
)

dump = executable(
    'dump',
    ['dump.c', amalgamate_srcs],
    include_directories: incdirs,
    build_by_default: false,
)
test(
    'dump',
    dump,
    suite: 'hex',
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
    suite: 'hex',
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
    suite: 'hex',
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
    suite: 'hex',
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
    suite: 'hex',
)

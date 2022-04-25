valid = executable(
	'valid',
	'valid.c',
	include_directories: test_incdirs,
	build_by_default: false,
	dependencies: encoding_dep,
)
test('valid',
	valid,
	suite: 'hex',
)

dump = executable(
	'dump',
	'dump.c',
	include_directories: test_incdirs,
	build_by_default: false,
	dependencies: encoding_dep,
)
test('dump',
	dump,
	suite: 'hex',
)

encode = executable(
	'encode',
	'encode.c',
	include_directories: test_incdirs,
	build_by_default: false,
	dependencies: encoding_dep,
)
test('encode',
	encode,
	suite: 'hex',
)

encoded_length = executable(
	'encoded_length',
	'encoded_length.c',
	include_directories: test_incdirs,
	build_by_default: false,
	dependencies: encoding_dep,
)
test('encoded_length',
	encoded_length,
	suite: 'hex',
)

decode = executable(
	'decode',
	'decode.c',
	include_directories: test_incdirs,
	build_by_default: false,
	dependencies: encoding_dep,
)
test('decode',
	decode,
	suite: 'hex',
)

decoded_length = executable(
	'decoded_length',
	'decoded_length.c',
	include_directories: test_incdirs,
	build_by_default: false,
	dependencies: encoding_dep,
)
test('decoded_length',
	decoded_length,
	suite: 'hex',
)

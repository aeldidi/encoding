encode = executable(
	'encode',
	'encode.c',
	include_directories: test_incdirs,
	dependencies:encoding_dep,
	build_by_default: false,
)
test('encode',
	encode,
	suite: 'base64',
)

encoded_length = executable(
	'encoded_length',
	'encoded_length.c',
	include_directories: test_incdirs,
	dependencies:encoding_dep,
	build_by_default: false,
)
test('encoded_length',
	encoded_length,
	suite: 'base64',
)

decoded_length = executable(
	'decoded_length',
	'decoded_length.c',
	include_directories: test_incdirs,
	dependencies:encoding_dep,
	build_by_default: false,
)
test('decoded_length',
	decoded_length,
	suite: 'base64',
)

decode = executable(
	'decode',
	'decode.c',
	include_directories: test_incdirs,
	dependencies:encoding_dep,
	build_by_default: false,
)
test('decode',
	decode,
	suite: 'base64',
)

valid = executable(
	'valid',
	'valid.c',
	include_directories: test_incdirs,
	dependencies:encoding_dep,
	build_by_default: false,
)
test('valid',
	valid,
	suite: 'base64',
)

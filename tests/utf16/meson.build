valid = executable(
	'valid',
	'valid.c',
	include_directories: test_incdirs,
	build_by_default: false,
	dependencies: encoding_dep,
)
test('valid',
	valid,
	suite: 'utf16',
)

codepoint_encode = executable(
	'codepoint_encode',
	'codepoint_encode.c',
	include_directories: test_incdirs,
	build_by_default: false,
	dependencies: encoding_dep,
)
test('codepoint_encode',
	codepoint_encode,
	suite: 'utf16',
)

codepoint_decode = executable(
	'codepoint_decode',
	'codepoint_decode.c',
	include_directories: test_incdirs,
	build_by_default: false,
	dependencies: encoding_dep,
)
test('codepoint_decode',
	codepoint_decode,
	suite: 'utf16',
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
	suite: 'utf16',
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
	suite: 'utf16',
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
	suite: 'utf16',
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
	suite: 'utf16',
)

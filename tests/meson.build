test_incdirs = [incdirs, include_directories('.')]

strerror = executable(
	'strerror',
	'strerror.c',
	include_directories: test_incdirs,
	build_by_default: false,
	dependencies: encoding_dep,
)
test('strerror',
	strerror,
)

subdir('base32')
subdir('base64')
subdir('binary')
subdir('utf8')
subdir('utf16')
subdir('hex')

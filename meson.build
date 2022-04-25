project(
	'encoding',
	'c',
	license: 'BSL-1.0',
	version: '1.0.0',
	default_options: [
		'c_std=c11',
		'warning_level=3',
		'default_library=static',
	],
)

if host_machine.system() == 'windows'
	add_project_arguments(
		[
			'-DENCODING_PUBLIC=__declspec(dllexport)',
		],
		language: 'c',
	)

else
	add_project_arguments(
		[
			'-DENCODING_PUBLIC=',
		],
		language: 'c',
	)
endif

srcs = files(
	'src/encoding/common.c',
	'src/encoding/utf8.c',
	'src/encoding/utf16.c',
	'src/encoding/binary.c',
	'src/encoding/base32.c',
	'src/encoding/base64.c',
	'src/encoding/hex.c',
)

incdirs = include_directories(
	'include/',
)

install_headers(
	'include/encoding/common.h',
	'include/encoding/utf8.h',
	'include/encoding/utf16.h',
	'include/encoding/binary.h',
	'include/encoding/base64.h',
	'include/encoding/hex.h',
	subdir: 'encoding'
)

encoding_lib = library(
	'encoding',
	srcs,
	include_directories: incdirs,
)

encoding_dep = declare_dependency(
	include_directories: incdirs,
	link_with: encoding_lib,
)

subdir('tests')

pkg = import('pkgconfig')
pkg.generate(
	encoding_lib,
	description: 'A library for encoding and decoding binary data in '+
	             'various formats.',
)

project(
    'encoding',
    'c',
    license: '0BSD',
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
    'src/encoding/base32.c',
    'src/encoding/base64.c',
    'src/encoding/hex.c',
    'src/encoding/utf16.c',
    'src/encoding/utf8.c',
)

incdirs = include_directories(
    'include/',
)

install_headers(
    'include/encoding/utf8.h',
    'include/encoding/utf16.h',
    'include/encoding/binary.c',
    'include/encoding/base64.h',
    'include/encoding/hex.h',
    subdir: 'encoding',
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

# Generate the amalgamations

amalgamate_exe = executable(
    'amalgamate_exe',
    files('tools/amalgamate.c'),
    build_by_default: false,
)
amalgamate_tgts = []

foreach file : ['base32', 'base64', 'hex', 'utf8', 'utf16']
    amalgamate_tgts += run_target(
        'amalgamate-' + file,
        command: [
            amalgamate_exe,
            meson.source_root() / 'src' / 'encoding',
            meson.source_root() / 'include' / 'encoding',
            file,
        ],
    )
endforeach

alias_target('amalgamate', amalgamate_tgts)

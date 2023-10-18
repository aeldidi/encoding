project(
    'encoding',
    'c',
    license: '0BSD',
    version: '1.0.0',
    default_options: [
        'c_std=c11',
        'warning_level=3',
    ],
)

srcs = files(
    'base32/base32.c',
    'base32/base32.h',
    'base64/base64.c',
    'base64/base64.h',
    'hex/hex.c',
    'hex/hex.h',
    'utf16/utf16.c',
    'utf16/utf16.h',
    'utf8/utf8.c',
    'utf8/utf8.h',
)

# Generate the amalgamations

amalgamate_exe = executable(
    'amalgamate_exe',
    files('tools/amalgamate.c'),
)
amalgamate_srcs = []

foreach file : ['base32', 'base64', 'hex', 'utf8', 'utf16']
    amalgamate_srcs += custom_target(
        file + '.c',
        input: srcs,
        output: file + '.c',
        command: [
            amalgamate_exe,
            meson.source_root() / file,
            file,
        ],
    )
endforeach

subdir('tests')

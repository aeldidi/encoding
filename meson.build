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
    'src/base32.c',
    'src/base32.h',
    'src/base64.c',
    'src/base64.h',
    'src/hex.c',
    'src/hex.h',
    'src/utf16.c',
    'src/utf16.h',
    'src/utf8.c',
    'src/utf8.h',
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
            meson.source_root() / 'src',
            file,
        ],
    )
endforeach

subdir('tests')

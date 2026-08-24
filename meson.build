project(
    'sdl-demo',
    'cpp',

    version: '1.0.0',
    meson_version: '1.12.0',
    license: 'MIT',
    license_files: 'LICENSE',

    default_options: [
        'cpp_std=c++20',
        'cpp_rtti=false',
        'cpp_eh=none',

        'warning_level=3',
        'werror=true'
    ]
)

# Common dirs

proj_root = meson.current_source_dir() + '/'
jni_root = 'app/src/jni/'
imgui_root = jni_root + 'third_party/lib/imgui/'
sdl_root = jni_root + 'third_party/lib/SDL3/'
system_incl = include_directories(
    jni_root + 'third_party/include',
    is_system: true
)

if host_machine.cpu() == 'armv8-a'
    lib_dir = 'arm64-v8a/'
elif host_machine.cpu() == 'armv7-a'
    lib_dir = 'armeabi-v7a/'
else
    error('Unknown cpu, available: armv8-a, armv7-a')
endif

# Prebuild commands

message('(Prebuild) Creating directories')
run_command(
    'mkdir',
    '-p',
    '.build/bin',
    '.build/lib/' + lib_dir,
    '.build/obj',
    '.build/res',
    check: true
)

message('(Prebuild) Extracting libraries')
run_command(
    'tar',
    '-xvf',
    sdl_root + 'libSDL3.tar.xz',
    '-C',
    sdl_root,
    check: true
)

message('(Prebuild) Executing other commands')
run_command('bash', 'scripts/prebuild.bash', check: true)

strip = find_program('strip', required: true)

# Libraries

cc = meson.get_compiler('cpp')

cpp_static = cc.find_library('c++_static', required: true)
android = cc.find_library('android', required: true)
log = cc.find_library('log', required: true)
egl = cc.find_library('EGL', required: true)
gl = cc.find_library('GLESv3', required: true)
sdl = cc.find_library(
    'SDL3',
    dirs: proj_root + sdl_root + lib_dir,
    required: true
)

# Build

sources = files(
    jni_root + 'main.cc',
    jni_root + 'core/display.cc',
    jni_root + 'core/shader.cc',
    jni_root + 'core/shader.cc',
    jni_root + 'core/gui.cc',
    jni_root + 'core/buffers.cc',
    imgui_root + 'imgui.cpp',
    imgui_root + 'imgui_draw.cpp',
    imgui_root + 'imgui_impl_opengl3.cpp',
    imgui_root + 'imgui_impl_sdl3.cpp',
    imgui_root + 'imgui_tables.cpp',
    imgui_root + 'imgui_widgets.cpp'
)

main = executable(
    'main',
    sources,

    android_exe_type: 'application',
    install: false,
    include_directories: system_incl,
    link_args: [
        '-static-libstdc++'
    ],
    dependencies: [
        cpp_static,
        android,
        log,
        egl,
        gl,
        sdl
    ]
)

if get_option('buildtype') == 'release'
    strip = custom_target(
        'strip',
        depends: main,
        input: main,
        output: 'blank',
        build_by_default: true,
        command: [
            strip,
            '--strip-unneeded',
            '@INPUT@'
        ]
    )
else
    strip = custom_target(
        'strip',
        depends: main,
        input: main,
        output: 'blank',
        build_by_default: true,
        command: [
            'test',
            '1 == 1'
        ]
    )
endif

copy = custom_target(
    'copy',
    depends: strip,
    input: main,
    output: 'none',
    build_by_default: true,
    command: [
        'cp',
        '@INPUT@',
        '../lib/' + lib_dir
    ]
)

custom_target(
    'postbuild',
    depends: copy,
    input: main,
    output: 'empty',
    build_by_default: true,
    command: [
        'bash',
        '../../scripts/postbuild.bash'
    ]
)

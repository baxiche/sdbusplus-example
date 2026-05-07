project(
    'sdbus-example',
    'cpp',
    default_options: [
        'cpp_std=c++23',
        'warning_level=2',
    ],
    meson_version: '>=1.1.1',
)

libsystemd_dep = dependency('libsystemd')
nlohmann_json_dep = dependency(
    'nlohmann_json',
    fallback: ['nlohmann_json', 'nlohmann_json_dep'],
)

sdbusplus_inc = include_directories('sdbusplus/include')
generated_inc = include_directories('generated')

sdbusplus_lib = static_library(
    'sdbusplus-local',
    files(
        'sdbusplus/src/bus.cpp',
        'sdbusplus/src/bus/match.cpp',
        'sdbusplus/src/event.cpp',
        'sdbusplus/src/exception.cpp',
        'sdbusplus/src/message/native_types.cpp',
        'sdbusplus/src/sdbus.cpp',
        'sdbusplus/src/server/interface.cpp',
        'sdbusplus/src/server/transaction.cpp',
    ),
    include_directories: sdbusplus_inc,
    dependencies: [libsystemd_dep, nlohmann_json_dep],
)

sdbusplus_dep = declare_dependency(
    include_directories: sdbusplus_inc,
    link_with: sdbusplus_lib,
    dependencies: [libsystemd_dep, nlohmann_json_dep],
)

executable(
    'hello-service',
    files(
        'src/hello-service.cpp',
        'generated/xyz/openbmc_project/Example/Simple/Hello/server.cpp',
    ),
    include_directories: generated_inc,
    dependencies: sdbusplus_dep,
    install: true,
)

install_data(
    'dbus-1/system-services/xyz.openbmc_project.Example.Simple.service',
    install_dir: get_option('datadir') / 'dbus-1/system-services',
)

install_data(
    'systemd/xyz.openbmc_project.Example.Simple.service',
    install_dir: get_option('prefix') / 'lib/systemd/system',
)

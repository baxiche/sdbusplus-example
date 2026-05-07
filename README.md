# Minimal sdbusplus Service Built With Meson

This repository is a small `sdbusplus` and `busctl` example. It builds a D-Bus
service named:

```text
xyz.openbmc_project.Example.Simple
```

The service exports this object:

```text
/xyz/openbmc_project/example/simple/hello
```

and implements this interface:

```text
xyz.openbmc_project.Example.Simple.Hello
```

The `Ping` method returns the input string with a `pong: ` prefix.

## Project Layout

```text
.
├── meson.build
├── src/hello-service.cpp
├── generated/xyz/openbmc_project/Example/Simple/Hello/
│   ├── common.hpp
│   ├── server.hpp
│   └── server.cpp
├── yaml/xyz/openbmc_project/Example/Simple/Hello.interface.yaml
├── sdbusplus/
├── dbus-1/system-services/xyz.openbmc_project.Example.Simple.service
├── systemd/xyz.openbmc_project.Example.Simple.service
└── subprojects/nlohmann_json.wrap
```

The top-level `meson.build` builds:

```text
build/libsdbusplus-local.a
build/hello-service
```

`libsdbusplus-local.a` is a small local static library built from the vendored
`sdbusplus` sources needed by this example. The final executable links that
library plus `libsystemd`.

## Prerequisites

Install the build tools and systemd development headers. On Ubuntu/Debian:

```bash
sudo apt install meson ninja-build g++ pkg-config libsystemd-dev
```

The project also needs `nlohmann_json`. If it is not installed system-wide,
Meson uses `subprojects/nlohmann_json.wrap` to fetch it from GitHub.

## Build With Meson

From the repository root:

```bash
meson setup build
meson compile -C build
```

The service binary is created here:

```text
build/hello-service
```

If your environment forces `ccache` and the cache is not writable, disable it
for this build command:

```bash
CCACHE_DISABLE=1 meson compile -C build
```

To reconfigure after editing `meson.build`:

```bash
meson setup build --reconfigure
meson compile -C build
```

To start from a clean build directory:

```bash
rm -rf build
meson setup build
meson compile -C build
```

## Run And Test

Run the service inside a temporary session bus:

```bash
dbus-run-session -- sh -c '
  ./build/hello-service &
  service_pid=$!
  trap "kill $service_pid 2>/dev/null || true" EXIT
  sleep 1

  busctl --user call \
    xyz.openbmc_project.Example.Simple \
    /xyz/openbmc_project/example/simple/hello \
    xyz.openbmc_project.Example.Simple.Hello \
    Ping s "hello"
'
```

Expected output:

```text
s "pong: hello"
```

Read the `Enabled` property:

```bash
dbus-run-session -- sh -c '
  ./build/hello-service &
  service_pid=$!
  trap "kill $service_pid 2>/dev/null || true" EXIT
  sleep 1

  busctl --user get-property \
    xyz.openbmc_project.Example.Simple \
    /xyz/openbmc_project/example/simple/hello \
    xyz.openbmc_project.Example.Simple.Hello \
    Enabled
'
```

## Install

Install into the configured prefix:

```bash
meson install -C build
```

By default, Meson installs the executable under:

```text
/usr/local/bin/hello-service
```

The build also installs:

```text
/usr/local/share/dbus-1/system-services/xyz.openbmc_project.Example.Simple.service
/usr/local/lib/systemd/system/xyz.openbmc_project.Example.Simple.service
```

For a system-wide service using the conventional system directories, configure
the prefix before building:

```bash
meson setup build --prefix=/usr
meson compile -C build
sudo meson install -C build
sudo systemctl daemon-reload
```

Then start the service manually:

```bash
sudo systemctl start xyz.openbmc_project.Example.Simple.service
```

Or let D-Bus activate it when a client calls the service:

```bash
busctl call \
  xyz.openbmc_project.Example.Simple \
  /xyz/openbmc_project/example/simple/hello \
  xyz.openbmc_project.Example.Simple.Hello \
  Ping s "hello"
```

## Regenerate The Binding

The checked-in generated binding is already enough to build this example. To
regenerate it from the YAML interface:

```bash
./sdbusplus/tools/sdbus++-gen-meson \
  --command cpp \
  --output generated/xyz/openbmc_project/Example/Simple/Hello \
  --tool ./sdbusplus/tools/sdbus++ \
  --directory yaml \
  xyz/openbmc_project/Example/Simple/Hello
```

This regenerates:

```text
generated/xyz/openbmc_project/Example/Simple/Hello/common.hpp
generated/xyz/openbmc_project/Example/Simple/Hello/server.hpp
generated/xyz/openbmc_project/Example/Simple/Hello/server.cpp
generated/xyz/openbmc_project/Example/Simple/Hello/aserver.hpp
generated/xyz/openbmc_project/Example/Simple/Hello/client.hpp
```

After regenerating, rebuild:

```bash
meson compile -C build
```

## Interface Mapping

```text
YAML file:
  yaml/xyz/openbmc_project/Example/Simple/Hello.interface.yaml

Interface item:
  xyz/openbmc_project/Example/Simple/Hello

D-Bus interface name:
  xyz.openbmc_project.Example.Simple.Hello

Generated C++ class:
  sdbusplus::server::xyz::openbmc_project::example::simple::Hello
```

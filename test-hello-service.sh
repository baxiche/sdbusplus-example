#!/bin/sh
set -eu

/tmp/sdbus-example/hello-service &
service_pid=$!

trap 'kill "$service_pid" 2>/dev/null || true' EXIT
sleep 1

busctl --user call \
  xyz.openbmc_project.Example.Simple \
  /xyz/openbmc_project/example/simple/hello \
  xyz.openbmc_project.Example.Simple.Hello \
  Ping s "hello"

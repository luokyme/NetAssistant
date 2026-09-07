# NetAssistant

A network debugging assistant based on Qt 5 Widgets, with UDP, TCP client/server,
file transfer, group chat, and Chinese/English interfaces.

## Build on Linux

Ubuntu / Debian:

```sh
sudo apt update
sudo apt install build-essential qtbase5-dev qt5-qmake
git clone https://github.com/luokyme/NetAssistant.git
cd NetAssistant
mkdir build
cd build
qmake ../NetAssistant.pro
make -j$(nproc)
./NetAssistant
```

Use Qt **5**, not Qt 4 or Qt 6. If multiple Qt versions are installed, select
`qmake -qt=qt5` or the absolute path to Qt 5's `qmake`. Qt 4 builds were reported
to display Chinese text incorrectly (#4). Modern Ubuntu no longer provides
`qt5-default`; the packages above provide the required Qt 5 development tools.

On Windows, open `NetAssistant.pro` in Qt Creator with a Qt 5 desktop kit.
Modern MSVC builds use UTF-8 source and execution character sets.

## Addresses and ports

- Enter or paste addresses normally, e.g. `192.168.1.10`; no fixed-width IP mask
  is required (#8, #9).
- TCP client mode accepts server hostnames such as `localhost` or `example.com`,
  as well as IP literals (#6). DNS resolution is handled by Qt.
- UDP and TCP server modes require a local IP literal. `0.0.0.0` binds all IPv4
  interfaces; a specific address binds only that interface (PR #10).
- Local port `0` lets the OS choose a free port. Destination ports must be
  between `1` and `65535`. UDP destinations currently accept IP literals only.
- Disconnect before changing the network mode or local/server endpoint. UDP
  destinations can be edited while connected and are validated before sending.

## Regression tests

With Qt 5 development tools installed:

```sh
mkdir build-tests
cd build-tests
qmake ../tests/network.pro
make -j$(nproc)
QT_QPA_PLATFORM=offscreen ./network-tests
```

The tests use local sockets and do not require external servers.

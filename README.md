# NetAssistant

**English** | [简体中文](README.zh-CN.md)

A desktop network debugging assistant built with Qt 5 Widgets. Send and inspect
UDP or TCP traffic, transfer files, and test client/server communication.

[Download releases](https://github.com/luokyme/NetAssistant/releases) ·
[Report an issue](https://github.com/luokyme/NetAssistant/issues) ·
[Ubuntu build workflow](https://github.com/luokyme/NetAssistant/actions/workflows/release-ubuntu.yml)

## Features

- UDP, TCP client, and TCP server modes.
- TCP connections using an IP address or server hostname.
- Text and hexadecimal data display, timed sending, and send/receive counters.
- File sending and saving received data to a file.
- TCP server group chat and echo options.
- Chinese and English interfaces, selectable from the **Language** menu.

## Download and install

### Ubuntu 24.04 · amd64

Download both files from [v1.1.0](https://github.com/luokyme/NetAssistant/releases/tag/v1.1.0)
to the same directory:

- [Debian package](https://github.com/luokyme/NetAssistant/releases/download/v1.1.0/netassistant_1.1.0_ubuntu24.04_amd64.deb)
- [SHA256 checksum](https://github.com/luokyme/NetAssistant/releases/download/v1.1.0/netassistant_1.1.0_ubuntu24.04_amd64.deb.sha256)

Open a terminal in that directory:

```sh
sha256sum -c netassistant_1.1.0_ubuntu24.04_amd64.deb.sha256
sudo apt install ./netassistant_1.1.0_ubuntu24.04_amd64.deb
NetAssistant
```

You can also launch **NetAssistant** from the application menu. `apt` installs
the required Qt runtime libraries; development tools are not needed.
This package targets Ubuntu 24.04 on Intel/AMD 64-bit systems (amd64).

To uninstall:

```sh
sudo apt remove netassistant
```

### Windows

An older Windows x64 build is available in
[v1.0.0](https://github.com/luokyme/NetAssistant/releases/tag/v1.0.0).
It does not include the v1.1.0 fixes. To build the current source, see
[Build from source](#build-from-source).

## Quick start

1. Select **UDP**, **TCP client**, or **TCP server**.
2. Enter the address and port according to the selected mode.
3. Connect or start listening, enter text in the send area, and send it.
4. Inspect received data and counters. Disconnect before changing the network
   mode or local/server endpoint.

| Mode | Main address and port | Additional settings |
| --- | --- | --- |
| UDP | Local IP and port | Destination IP and port; editable while connected |
| TCP client | Server IP or hostname, and server port | Local endpoint is shown after connecting |
| TCP server | Local IP and listening port | Select a connected client or all clients for sending |

Addresses can be typed or pasted normally, for example `192.168.1.10`.
TCP client mode also accepts hostnames such as `localhost` or `example.com`.
UDP destinations and local bind addresses require IP literals.

For UDP and TCP server modes, `0.0.0.0` binds all IPv4 interfaces; a specific
local IP binds only that interface. Local port `0` lets the OS choose a free
port, which is displayed after binding. Destination ports must be `1–65535`.

For a local test, open two instances: start a TCP server at `127.0.0.1:56050`,
then connect a TCP client to `localhost:56050` and send a message.

## Build from source

### Linux

On Ubuntu / Debian, install the build tools and Qt 5 development packages:

```sh
sudo apt update
sudo apt install git build-essential qtbase5-dev qt5-qmake
git clone https://github.com/luokyme/NetAssistant.git
cd NetAssistant
mkdir -p build
cd build
qmake ../NetAssistant.pro CONFIG+=release CONFIG-=debug
make -j"$(nproc)"
./NetAssistant
```

Qt **5** is required; Qt 4 and Qt 6 are not supported by the current project.
If multiple versions are installed, use `qmake -qt=qt5` with Qt's version
selector, or the absolute path to Qt 5's `qmake`.
Modern Ubuntu uses the packages above instead of `qt5-default`.

### Windows

Open [NetAssistant.pro](NetAssistant.pro) in Qt Creator and select a Qt 5 desktop
kit. Modern MSVC builds use UTF-8 source and execution character sets.
The current release has been built and tested on Ubuntu 24.04; the current
Windows source build has not been verified.

## Development

### Regression tests

From the repository root, with Qt 5 development tools installed:

```sh
mkdir -p build-tests
cd build-tests
qmake ../tests/network.pro
make -j"$(nproc)"
QT_QPA_PLATFORM=offscreen ./network-tests
```

Tests use local sockets and cover address entry, UDP binding and transmission,
TCP hostname connections, disconnect cleanup, server restart, and connection
failure recovery. They do not require external servers.

### Package and publish

To package a release binary on Ubuntu 24.04 amd64, run from the repository root:

```sh
sudo apt install dpkg-dev
scripts/package-ubuntu.sh 1.1.0 build/NetAssistant dist
```

The output directory contains a `.deb` and its `.sha256` checksum.

The [release workflow](.github/workflows/release-ubuntu.yml) builds the selected
tag, runs tests, verifies package installation and startup, then publishes the
assets to GitHub Releases. Pushing a stable version tag such as `v1.1.0` triggers
this workflow automatically.

To publish an existing tag manually:

1. Open **Actions → Ubuntu 24.04 release → Run workflow**.
2. Leave the workflow branch as `master`.
3. Set **tag** to the existing release tag, for example `v1.1.0`.
4. Start the run and check that both **build** and **publish** succeed.

Use a new **Run workflow** after workflow fixes; re-running an old job retains
its old configuration. Re-publishing a tag replaces its existing package assets.

## Troubleshooting

- **Cannot connect:** confirm the mode, destination address and port, that the
  server is listening, and that the firewall permits the connection. Bind to an
  address assigned to the local machine, or use `0.0.0.0` for all IPv4 interfaces.
- **`qmake` is missing or cannot launch Qt 5:** install `qtbase5-dev` and
  `qt5-qmake`, then check `qmake -v` and select Qt 5 if needed.
- **Chinese text is unreadable:** use Qt 5 when building. If glyphs are missing
  on Ubuntu, install a Chinese font with `sudo apt install fonts-noto-cjk`.

For other problems, [open an issue](https://github.com/luokyme/NetAssistant/issues)
with your OS, application/Qt version, network mode, reproduction steps, and any
error message.

## License

See [LICENSE](LICENSE) for the GNU General Public License, version 3.

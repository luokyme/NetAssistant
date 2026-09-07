#!/usr/bin/env bash
set -euo pipefail

# Package a release binary that was built and tested on Ubuntu 24.04.
if [[ $# -ne 3 ]]; then
    echo "Usage: $0 VERSION BINARY OUTPUT_DIRECTORY" >&2
    exit 1
fi
version=${1#v}
[[ $version =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]] || { echo "Expected a stable semantic version" >&2; exit 1; }
source /etc/os-release
[[ $ID == ubuntu && $VERSION_ID == 24.04 ]] || { echo "Build packages on Ubuntu 24.04" >&2; exit 1; }
repo_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
binary=$(realpath -- "$2")
[[ -x $binary ]] || { echo "Binary is missing or not executable" >&2; exit 1; }
mkdir -p -- "$3"
output_dir=$(realpath -- "$3")
arch=$(dpkg --print-architecture)
[[ $arch == amd64 ]] || { echo "This release targets amd64" >&2; exit 1; }
staging=$(mktemp -d)
trap 'rm -rf -- "$staging"' EXIT
package_root="$staging/package"
install -Dm755 "$binary" "$package_root/usr/bin/NetAssistant"
strip --strip-unneeded "$package_root/usr/bin/NetAssistant"
install -Dm644 "$repo_dir/packaging/netassistant.desktop" "$package_root/usr/share/applications/netassistant.desktop"
install -Dm644 "$repo_dir/NetAssistant.png" "$package_root/usr/share/icons/hicolor/256x256/apps/netassistant.png"
install -Dm644 "$repo_dir/LICENSE" "$package_root/usr/share/doc/netassistant/copyright"
install -Dm644 "$repo_dir/README.md" "$package_root/usr/share/doc/netassistant/README.md"
mkdir -p "$staging/debian" "$package_root/DEBIAN"
cat > "$staging/debian/control" <<CONTROL
Source: netassistant
Section: net
Priority: optional
Maintainer: NetAssistant contributors <noreply@github.com>

Package: netassistant
Architecture: amd64
Description: Qt network debugging assistant
CONTROL
# Let Ubuntu determine the ABI dependencies, including its t64 Qt packages.
dependencies=$(cd "$staging" && dpkg-shlibdeps -O -e"$package_root/usr/bin/NetAssistant")
dependencies=${dependencies#shlibs:Depends=}
cat > "$package_root/DEBIAN/control" <<CONTROL
Package: netassistant
Version: $version
Section: net
Priority: optional
Architecture: $arch
Maintainer: NetAssistant contributors <noreply@github.com>
Depends: $dependencies
Recommends: fonts-noto-cjk
Installed-Size: $(du -sk "$package_root/usr" | cut -f1)
Homepage: https://github.com/luokyme/NetAssistant
Description: Qt network debugging assistant
 UDP and TCP client/server debugging, file transfer, group chat,
 and Chinese/English interfaces. Built for Ubuntu 24.04.
CONTROL
artifact="netassistant_${version}_ubuntu24.04_${arch}.deb"
dpkg-deb --root-owner-group --build "$package_root" "$output_dir/$artifact"
(cd "$output_dir" && sha256sum "$artifact" > "$artifact.sha256")

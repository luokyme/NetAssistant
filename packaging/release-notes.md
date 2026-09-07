Ubuntu 24.04 (amd64) release, built with Qt 5.

- Fix IP address entry and UDP binding to the selected local interface (#8, #9, PR #10).
- Support TCP server hostnames (#6).
- Validate addresses and ports, clean up failed connections, and stop periodic sending on disconnect.
- Include Chinese/English translations, desktop launcher, and network regression tests.

Install the downloaded package with:

```sh
sha256sum -c netassistant_1.1.0_ubuntu24.04_amd64.deb.sha256
sudo apt install ./netassistant_1.1.0_ubuntu24.04_amd64.deb
```

Start **NetAssistant** from the application menu or run `NetAssistant` in a terminal.
Qt runtime dependencies are installed by apt. This package targets Ubuntu 24.04 amd64;
other distributions and architectures have not been validated.

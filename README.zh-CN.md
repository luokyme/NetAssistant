# NetAssistant

[English](README.md) | **简体中文**

基于 Qt 5 Widgets 的桌面网络调试助手，用于收发和查看 UDP、TCP 数据，
传输文件，以及测试客户端与服务端通信。

[下载发布版本](https://github.com/luokyme/NetAssistant/releases) ·
[反馈问题](https://github.com/luokyme/NetAssistant/issues) ·
[Ubuntu 构建工作流](https://github.com/luokyme/NetAssistant/actions/workflows/release-ubuntu.yml)

## 功能

- 支持 UDP、TCP 客户端和 TCP 服务端模式。
- TCP 客户端支持使用 IP 地址或服务器域名连接。
- 支持文本与十六进制数据显示、定时发送及收发计数。
- 支持发送文件、将接收数据保存到文件。
- TCP 服务端提供群聊转发和回显选项。
- 提供中文和英文界面，可通过 **Language** 菜单切换。

## 下载与安装

### Ubuntu 24.04 · amd64

从 [v1.1.0 发布页](https://github.com/luokyme/NetAssistant/releases/tag/v1.1.0)
下载以下两个文件，放在同一目录：

- [Debian 安装包](https://github.com/luokyme/NetAssistant/releases/download/v1.1.0/netassistant_1.1.0_ubuntu24.04_amd64.deb)
- [SHA256 校验文件](https://github.com/luokyme/NetAssistant/releases/download/v1.1.0/netassistant_1.1.0_ubuntu24.04_amd64.deb.sha256)

在该目录打开终端，执行：

```sh
sha256sum -c netassistant_1.1.0_ubuntu24.04_amd64.deb.sha256
sudo apt install ./netassistant_1.1.0_ubuntu24.04_amd64.deb
NetAssistant
```

也可以从应用菜单启动 **NetAssistant**。`apt` 会安装所需的 Qt 运行库，
无需安装开发工具。此安装包适用于 Intel/AMD 64 位架构（amd64）的 Ubuntu 24.04。

卸载：

```sh
sudo apt remove netassistant
```

### Windows

[v1.0.0 发布页](https://github.com/luokyme/NetAssistant/releases/tag/v1.0.0)
提供旧版 Windows x64 程序，不包含 v1.1.0 的修复。
如需使用当前源码，请参阅[源码构建](#源码构建)。

## 快速上手

1. 选择 **UDP**、**TCP 客户端**或 **TCP 服务端**模式。
2. 根据模式填写地址和端口。
3. 连接网络或开始监听，在发送区输入文本并发送。
4. 查看接收数据和收发计数。修改网络模式、本地端点或服务器端点前，请先断开连接。

| 模式 | 主地址与端口 | 其他设置 |
| --- | --- | --- |
| UDP | 本地 IP 和端口 | 目标 IP 和端口，连接期间可修改 |
| TCP 客户端 | 服务器 IP 或域名，以及服务器端口 | 连接成功后显示本地端点 |
| TCP 服务端 | 本地 IP 和监听端口 | 可选择向指定客户端或全部客户端发送 |

地址支持正常输入和粘贴，例如 `192.168.1.10`。
TCP 客户端还支持 `localhost`、`example.com` 等域名。
UDP 目标地址和本地绑定地址需要填写 IP 地址。

在 UDP 和 TCP 服务端模式下，`0.0.0.0` 表示绑定所有 IPv4 网络接口，
填写具体的本地 IP 则仅绑定对应接口。本地端口填写 `0` 时，由系统分配空闲端口，
绑定成功后会显示实际端口。目标端口范围为 `1–65535`。

本机测试示例：打开两个程序实例，在一个实例中启动 `127.0.0.1:56050` 的 TCP 服务端，
另一个实例使用 TCP 客户端连接 `localhost:56050`，然后发送消息。

## 源码构建

### Linux

在 Ubuntu / Debian 上安装构建工具和 Qt 5 开发依赖：

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

当前项目要求使用 Qt **5**，不支持 Qt 4 或 Qt 6。
如果安装了多个 Qt 版本，可通过 Qt 版本选择器使用 `qmake -qt=qt5`，
或直接使用 Qt 5 的 `qmake` 绝对路径。
较新的 Ubuntu 使用上述依赖包，无需安装 `qt5-default`。

### Windows

在 Qt Creator 中打开 [NetAssistant.pro](NetAssistant.pro)，选择 Qt 5 桌面构建套件。
较新的 MSVC 构建使用 UTF-8 源码及执行字符集。
当前发布版本已在 Ubuntu 24.04 上完成构建和测试；当前源码的 Windows 构建尚未验证。

## 开发与维护

### 回归测试

安装 Qt 5 开发工具后，在仓库根目录执行：

```sh
mkdir -p build-tests
cd build-tests
qmake ../tests/network.pro
make -j"$(nproc)"
QT_QPA_PLATFORM=offscreen ./network-tests
```

测试使用本地 socket，覆盖地址输入、UDP 绑定与发送、TCP 域名连接、断开清理、
服务端重启及连接失败后的恢复，不依赖外部服务器。

### 打包与发布

在 Ubuntu 24.04 amd64 上构建 release 程序后，从仓库根目录执行：

```sh
sudo apt install dpkg-dev
scripts/package-ubuntu.sh 1.1.0 build/NetAssistant dist
```

输出目录包含 `.deb` 安装包及对应的 `.sha256` 校验文件。

[发布工作流](.github/workflows/release-ubuntu.yml)会检出指定标签、构建程序、运行测试，
并验证安装包安装和启动，全部成功后将附件发布到 GitHub Releases。
推送 `v1.1.0` 这样的稳定版本标签会自动触发工作流。

手动发布已有标签：

1. 打开 **Actions → Ubuntu 24.04 release → Run workflow**。
2. 工作流分支保持为 `master`。
3. 在 **tag** 中填写已有的发布标签，例如 `v1.1.0`。
4. 启动任务，确认 **build** 和 **publish** 均成功。

工作流修复后，应新建一次 **Run workflow**；重新运行旧任务仍会使用旧配置。
重复发布同一标签会替换该版本已有的安装包附件。

## 常见问题

- **无法连接：** 检查网络模式、目标地址与端口、服务端是否正在监听，以及防火墙是否允许通信。
  本地绑定地址应为本机实际拥有的 IP，也可使用 `0.0.0.0` 绑定所有 IPv4 接口。
- **找不到 `qmake` 或无法启动 Qt 5：** 安装 `qtbase5-dev` 和 `qt5-qmake`，
  执行 `qmake -v` 检查版本，必要时指定 Qt 5。
- **中文显示异常：** 编译时请使用 Qt 5。如果 Ubuntu 上缺少中文字形，
  可执行 `sudo apt install fonts-noto-cjk` 安装中文字体。

其他问题请[提交 issue](https://github.com/luokyme/NetAssistant/issues)，
附上操作系统、程序及 Qt 版本、网络模式、复现步骤和错误信息。

## 许可证

项目采用 GNU 通用公共许可证第 3 版，详见 [LICENSE](LICENSE)。

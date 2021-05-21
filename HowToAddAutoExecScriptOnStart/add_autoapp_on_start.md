# 两种方式
## 方式1 命令

### 创建~/.config/autostart
mkdir -p ~/.config/autostart

### 增加run.desktop开机自动执行文件配置
cat >~/.config/autostart/run.desktop <<END
[Desktop Entry]
Type=Application
Name=StartupScript
Exec=你要执行的脚本的完整路径（如果要延迟，命令前加sleep 5 &&）
Path=如果你的脚本需要设置启动目录，在这里设置
Terminal=true
X-GNOME-Autostart-enabled=true
END

### 赋予可执行权限
chmod +x ~/.config/autostart/run.desktop

### 增加到开机信任
gio set ~/.config/autostart/run.desktop metadata::trusted yes

## 方式2 图形化操作
### 输入命令
$gnome-session-properties

### 添加

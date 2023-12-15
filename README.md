# gamepad手册



# 1.代码下载与编译

在main.cpp中根据使用的手柄修改程序，默认使用的是skydroid(云卓)手柄

```
git clone git@github.com:DeepRoboticsLab/gamepad.git
cd gamepad
mkdir build && cd build
cmake ..
make -j4
```



# 2.程序运行

1. 在手柄中安装提供的app，在app中按步骤2配置需要连接的主机的ip和端口号

   <img src="./doc/app_retroid.png" alt="a" style="zoom:60%;" />

   <center>app显示界面</center>

2. 获取需要通讯主机的ip地址，配置程序中设置的端口号，通过点击app右上角感叹号进入配置

   <img src="./doc/ip_config.png" alt="s" style="zoom:67%;" />

   <center>ip配置界面</center>

   

3. 编译好代码后，运行程序

   ```
   cd build
   ./gamepad_reciver
   ```

   

4. 摇动手柄按键，程序正常运行后终端结果如下

   <img src="./doc/terminal_show_info.png" alt="a" style="zoom:75%;" />

   <center>Skydroid手柄接受显示</center>

   

   <img src="./doc/terminal_show_info_retroid.png" alt="s" style="zoom:75%;" />

   <center>Retroid手柄接受显示</center>

   

5. 后续可以根据自己的需求开发，实现机器人的远程控制。






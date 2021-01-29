## 编译成APP
2. **源码根目录**下创建**build**目录`mkdir build`
3. 进行**build**目录,执行`cmake -DCMAKE_INSTALL_PREFIX=/usr ..`生成**Makefile**
4. 执行`make`进行编译

## 编译成LIB
2. **源码根目录**下创建**build**目录`mkdir build`
3. 进行**build**目录,执行`cmake -DCMAKE_INSTALL_PREFIX=/usr -DPLUGIN_DIR=/usr/share/kiran-control-center/plugins ..`生成**Makefile**，PLUGIN_DIR的路径，通过执行： kiran-control-center --print-plugins-path 输出.
4. 执行`make`进行编译

## 安装
1. 在**build**目录下执行`sudo make install`

## 卸载
1. 在**build**目录下执行`sudo make uninstall`

## app的使用
[root@localhost ~]# kiran-display-tools

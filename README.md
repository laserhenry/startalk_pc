**StarTalk PC 2.0 客户端**
=====
StarTalk PC 2.0 是基于StarTalk开源服务的Pc客户端, 支持Windows、Linux、Mac三个平台，除了部分平台性质的差别外，UI和功能都一致。

Release版本下载地址: [StarTalk官网](https://i.startalk.im/home/#/download)

### 开发环境搭建 
___
依赖的第三方库

名称  | 版本要求 | 下载地址 | 备注
 ---- | ---- | ------ | ---
 Qt  | 5.9.6及以上 | https://download.qt.io/archive/qt/ | 建议保持新版本 (ubuntu可以使用apt-get方式下载、MacOS可以使用brew方式下载)
 protobuf  | 2.6.1 | | 建议使用此版本
 zlib | 1211 | | 
 openssl | 1.1.1 | | 
 curl | 7.61.0 | | 
 qzxing |  | https://github.com/ftylitak/qzxing | 项目中包含源码
 quazip |  | https://github.com/stachenov/quazip | 项目中包含源码
 EventBus |  | https://github.com/greenrobot/EventBus | 项目中包含源码
 cJson | | https://github.com/DaveGamble/cJSON | 项目中包含源码 
 breakpad | | https://github.com/google/breakpad | 
 
1. Windows 操作系统   
    ① Cmake
      
2. Linux/MacOS 操作系统  
    ① Cmake  
    ② UUID  
 
Tips:  
    1> protobuf、zlib、openssl、curl几个库因为编译比较复杂，我们提供了已经编译成功的版本，位置 ${root_dir}/Depends/${name}，其他平台需要自己build  
    2> 安装Qt时请注意需要额外安装 "Qt WebEngine"、和 "Qt Network Authorization"模块。  
     QtMacExtras(MacOS)、QtX11Extras(Linux) 可能也需要额外安装，和安装的版本有关。
    3> Windows操作系统建议只安装一个版本的Qt

------
关于开发环境  
① Windows 建议使用 visual stdio 2017/2019  
② Linux Mac 使用Clion是个很好的选择  
③ 强烈推荐Visual Studio Code  

------

### 源码编译

```
#进入源文件根目录
$ cd ${root_dir}
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Debug/Release .. #根据自己需要修改build版本
$ make
```  

```
Windows参考  
$ cd ${root_dir}
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release -G "CodeBlocks - NMake Makefiles" .. 
$ cmake --build ${root_dir}/build --target all --
```

可执行文件会生成到 ${root_dir}/build/bin 目录下  
也可以参考 build_script 下的脚本

tips:  
    1> 需要修改跟目录下的CMakeList.txt 中第三方的路径 根据自己的安装位置修改  
    2> Windows操作系统下需要设置Qt、Cmake的全局系统变量  
    3> Windows操作系统下需要使用Visual Studio的工具编译( VS 2017/2019 的开发人员命令提示符 )  
    4> MacOs版本 cmake 增加参数 -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13  

### 关于更新

此版本不包含版本升级功能、我们会在功能完善之后放出。

### 联系我们
1> [issue](https://github.com/startalkIM/startalk/issues)  
2> 我们的[官网](https://i.startalk.im/home/#/) 留言  

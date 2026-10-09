# 四轮小车控制模拟程序

## 项目简介
本程序实现四轮小车模拟控制，支持 start、auto、manual、step、emergency、reset、status、quit 命令。

## 编译环境
- CMake 3.16+
- MinGW‑make

## 编译步骤
```bash
# 新建build文件夹
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
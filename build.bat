@echo off
echo === QDrive 编译脚本 ===
if not exist build mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
cd ..
echo 编译结束，exe 在 build 文件夹内
pause
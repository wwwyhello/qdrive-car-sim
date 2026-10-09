# 开发记录

1. CMake + MinGW 不能用中文文件夹路径，会出现 No rule to make target 编译错误，项目路径全部英文。
2. 移动项目之后，不能只清空build里面文件，需要直接删除整个build文件夹再重建，消除CMakeCache缓存。
3. status输出格式严格要求6行大写文本，不能夹杂额外printf提示文字，否则自动测试不通过。
4. auto、manual命令只修改**目标速度**，不会直接修改车轮实际速度；只有step才改变实际车轮速度。
5. emergency急停是直接把实际轮子速度置0，不需要调用step。
6. manual优先级高于auto；manual release只是关闭手动，自动的参数还保存在结构体，不会被清空。
7. 需要处理异常输入：参数缺失、速度越界、未知命令，不能让程序崩溃。
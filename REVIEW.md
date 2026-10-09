# REVIEW.md

## 问题1
程序主要分为头文件car.h、实现文件car.c、主程序main.c三部分。

car.h：定义枚举类型（系统状态、运动方向、控制来源）和Car结构体，声明全部功能函数，对外提供接口。
car.c：实现小车底层逻辑：初始化、四轮目标速度计算、step速度步进、急停、复位、status状态打印、获取当前生效目标。
main.c：负责读取控制台输入，解析每行命令字符串，调用car.c提供的接口完成各个命令业务，循环处理直到quit退出。

main的工作：循环读取标准输入的每一行命令；使用strtok分割命令字符串；匹配start/auto/manual/step/status/emergency/reset/quit等命令；做参数简单合法性校验；调用car模块函数修改小车状态；遇到非法命令直接忽略，保证程序不会崩溃。

## 问题2
结构体中同时保存auto与manual两套控制参数，使用`manual_valid`标记手动控制是否有效。
函数`get_effective_target`判断：如果`manual_valid=1`，优先选用手动的方向与速度；否则使用自动控制。以此实现手动优先级高于自动。

执行`manual release`时，代码把`car->manual_valid = 0`，清除手动有效标记。
之后调用`get_effective_target`会跳过手动分支，读取已经保存好的auto_dir、auto_speed，恢复自动控制。即使手动生效期间更新过auto参数，这些参数一直保存在结构体中不会丢失，释放后直接复用。

## 问题3
1. 普通运动命令`auto/manual`：只修改保存的控制参数，只影响目标速度，不修改车轮实际速度；实际速度变化必须依靠step。待机、急停状态会拒绝执行运动命令。
2. `step`：读取当前生效的目标，更新target_wheel数组；每个车轮实际速度向目标速度逼近，单次最多变化±20，不能跳变，在`car_step`函数内部处理。
3. `emergency`：直接把系统状态切为EMERGENCY，立刻将四个轮子实际速度直接置0，不需要经过step，在`car_emergency`实现。

## 问题4
新增运动方式需要修改位置：
1. 枚举`MoveDir`增加新的枚举常量。
2. `calc_target_wheel`增加case分支，填写该运动对应的四轮目标速度。
3. `str_to_mov`增加字符串匹配，解析输入命令文本到新枚举。
4. `mov_to_str`增加分支，status输出时打印对应大写字符串。

方便扩展的地方：运动逻辑集中在`calc_target_wheel`；命令解析与业务逻辑分离，main只负责解析，业务全部封装到car.c。
麻烦的地方：字符串与枚举互相转换有两处（str_to_mov、mov_to_str），增加新运动时两处都要修改，容易漏写。

后续改进：可以做一张映射表，把字符串、枚举、四轮速度模板写在一张数组表里，新增运动只需要在数组增加一行，不需要修改switch‑case多处分支；增加更多完整错误提示；增加边界测试。
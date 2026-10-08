# REVIEW.md
## 问题1：简要说明你的程序由哪些主要部分组成，各部分分别负责什么？main中主要完成了什么工作？
程序分为头文件、业务实现、主程序三部分：
1. include/car.h：预先定义好的结构体、枚举、函数声明（先决固定代码，禁止修改），描述小车状态和所有接口。
2. src/car.c：业务逻辑实现。实现小车初始化、各个控制命令、车轮速度计算、状态更新、状态打印。
3. src/main.c：程序入口。循环读取控制台输入，解析命令字符串，调用car.h声明的接口，分发控制指令。
main只负责读取输入、解析、调用接口，不写小车业务逻辑，实现输入和业务解耦。

## 问题2：自动控制和手动控制同时存在时，你的程序是如何保证手动控制优先的？执行 manual release后，又是如何恢复之前保存的自动控制要求的？
结构体中`manual_active`标记手动是否激活。
在`car_update_target`函数中优先判断：如果manual_active有效，就选用手动控制请求；否则选用自动请求。
manual release仅仅把`manual_active=0`，**不会清空保存的auto_req**，再次执行car_update_target就自动切回自动控制。

## 问题3：普通运动命令、step 和 emergency 对四轮实际速度的影响有什么区别？你的程序分别在哪里处理这些行为？
- auto/manual：只修改目标速度wheel_target，**不会改变实际车轮wheel_actual**，在car_cmd_auto/car_cmd_manual处理。
- step：每次执行，实际车轮向目标靠近，单轮单次最大变化±20，在car_cmd_step。
- emergency：直接强制把实际车轮清零，立刻进入急停状态，不需要step，car_cmd_emergency处理。

## 问题4：假设以后增加一种新的运动方式，你的程序需要修改哪些位置？哪些地方比较方便扩展；哪些地方修改起来比较麻烦；继续完善项目优先改进什么。
新增运动模式需要：
1. include/car.h 的MotionMode枚举新增成员
2. car.c中str2motion、motion2str增加分支
3. calc_wheel_target增加case，设置四个车轮速度
方便扩展：业务逻辑和输入分离，新增运动模式不改动main.c。
麻烦点：main用sscan简单解析命令，复杂参数不好扩展；status打印硬编码。
优先改进：写通用字符串分割函数，优化status输出，增加错误提示。
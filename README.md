高性能引脚读写

内置的引脚读写函数digitalRead和digitalWrite需要每次将引脚转换成寄存器地址再进行读写，而且还需要检查PWM计时器设定，虽然增加了可靠性，减少了内存占用，但是性能较低，无法满足超高频读写的需求。本库牺牲一定内存空间和与其它功能的兼容性和稳健性，换取对简单高频引脚读写操作的超高性能。

库中所有函数都和对应的内置函数大致同名（除了额外附赠一个DigitalToggle）。但是首字母大写，且所有参数都可以（但不必须）使用编译期确定的模板参数以提高性能。在所有参数均为模板的条件下，编译器可以将函数优化为内联的单条位运算指令，因而具有极高的性能。如有可能，应当尽量使用模板参数。

本库支持AVR和SAM架构。

本库为追求极致性能，跳过了一些内置函数会做的状态检查和设置，因此仅适用于简单无副作用的引脚高低平读写，可能会与PWM、中断等硬件特定的引脚额外功能冲突。一般应避免将本库的简单操作和其它复杂操作施加到同一个引脚上。如果一定需要切换使用，建议在切换时调用内置pinMode以将引脚重新设为正确的状态。

High performance pin read and write

The built-in pin read function digitalRead and digitalWrite need to convert the pin to the register address each time for reading and writing, and also need to check the PWM timer setting, although increased reliability, reduce memory usage, but the performance is low, can not meet the needs of UHF read and write. The library sacrifices a certain amount of memory space and compatibility and robustness with other functions in exchange for ultra-high performance for simple high-frequency pin read and write operations.

All functions in the library have roughly the same name as their built-in counterparts (except for an extra DigitalToggle), but with uppercase letters, and all parameters can (but are not required to) use compile-time template parameters to improve performance. Under the condition that all parameters are templates, the compiler can optimize the function to an inline single bit-operation instruction, which has extremely high performance. If possible, you should try to use template parameters.

This library supports AVR and SAM architectures.

In pursuit of extreme performance, the library skips the state checks and settings that some built-in functions would do, so it is only suitable for simple, side-effect free pin high and low level reading and writing, which may conflict with hardware specific pin additional features such as PWM and interrupt. You should generally avoid applying simple operations of this library and other complex operations to the same pin. If switching is necessary, it is recommended to call the built-in pinMode when switching to reset the pin to the correct state.

# API参考
```C++
#include <Low_level_quick_digital_IO.hpp>
using namespace Low_level_quick_digital_IO;
// 获知指定引脚配置为输入还是输出。
inline bool PinMode(uint8_t Pin);
// 获知指定引脚配置为输入还是输出。
template <uint8_t Pin>
inline bool PinMode();
// 将指定引脚配置为输入或输出。
inline void PinMode(uint8_t Pin, bool OutOrIn);
// 将指定引脚配置为输入或输出。
template <uint8_t Pin>
inline void PinMode(bool OutOrIn);
// 将指定引脚配置为输入或输出。
template <bool OutOrIn>
inline void PinMode(uint8_t Pin);
// 将指定引脚配置为输入或输出。
template <uint8_t Pin, bool OutOrIn>
inline void PinMode();
// 从指定的数字引脚读取值（HIGH或LOW）。额外指定读取输入值还是输出值。
inline bool DigitalRead(uint8_t Pin, bool OutOrIn);
// 从指定的数字引脚读取值（HIGH或LOW）。额外指定读取输入值还是输出值。
template <uint8_t Pin>
inline bool DigitalRead(bool OutOrIn);
// 从指定的数字引脚读取值（HIGH或LOW）。额外指定读取输入值还是输出值。
template <bool OutOrIn>
inline bool DigitalRead(uint8_t Pin);
// 从指定的数字引脚读取值（HIGH或LOW）。额外指定读取输入值还是输出值。
template <uint8_t Pin, bool OutOrIn>
inline bool DigitalRead();
// 从指定的数字引脚读取值（HIGH或LOW）。根据引脚当前配置为输入还是输出，决定读取输入值还是输出值。
inline bool DigitalRead(uint8_t Pin);
// 从指定的数字引脚读取值（HIGH或LOW）。根据引脚当前配置为输入还是输出，决定读取输入值还是输出值。
template <uint8_t Pin>
inline bool DigitalRead();
// 写一个HIGH或LOW值到数字引脚。
inline void DigitalWrite(uint8_t Pin, bool HighOrLow);
// 写一个HIGH或LOW值到数字引脚。
template <uint8_t Pin>
inline void DigitalWrite(bool HighOrLow);
// 写一个HIGH或LOW值到数字引脚。
template <bool HighOrLow>
inline void DigitalWrite(uint8_t Pin);
// 写一个HIGH或LOW值到数字引脚。
template <uint8_t Pin, bool HighOrLow>
inline void DigitalWrite();
// 反转数字引脚的输出状态：若为HIGH则变LOW，若为LOW则变HIGH
inline void DigitalToggle(uint8_t Pin);
// 反转数字引脚的输出状态：若为HIGH则变LOW，若为LOW则变HIGH
template <uint8_t Pin>
inline void DigitalToggle();
```
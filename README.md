高性能引脚操作

内置的引脚读写函数`digitalRead`和`digitalWrite`需要每次将引脚转换成寄存器地址再进行读写，而且还需要检查PWM计时器设定，虽然增加了可靠性，减少了内存占用，但是性能较低，无法满足超高频操作的需求。本库牺牲一定内存空间和与其它功能的兼容性和稳健性，追求使用尽可能少的指令周期完成引脚操作，并支持内置函数所欠缺的一些扩展功能。
# API参考
所有`OutOrIn`参数均可指定为`OUTPUT`或`INPUT`，所有`HighOrLow`参数均可指定为`HIGH`或`LOW`。许多方法都有多个重载，应尽可能使用模板参数来减少运行时开销。
```C++
namespace Low_level_quick_digital_IO
{
	/*
	引脚IO操作。主要是增强内置方法的功能和性能，额外提供便捷的引脚翻转功能。
	*/

	// 获知指定引脚配置为输入还是输出（OUTPUT或INPUT）。
	inline bool PinMode(uint8_t Pin);
	template <uint8_t Pin>
	inline bool PinMode();

	// 将指定引脚配置为输入或输出。
	inline void PinMode(uint8_t Pin, bool OutOrIn);
	template <uint8_t Pin>
	inline void PinMode(bool OutOrIn);
	template <bool OutOrIn>
	inline void PinMode(uint8_t Pin);
	template <uint8_t Pin, bool OutOrIn>
	inline void PinMode();

	// 从指定的数字引脚读取值（HIGH或LOW）。额外指定读取输入值还是输出值。
	inline bool DigitalRead(uint8_t Pin, bool OutOrIn);
	template <uint8_t Pin>
	inline bool DigitalRead(bool OutOrIn);
	template <bool OutOrIn>
	inline bool DigitalRead(uint8_t Pin);
	template <uint8_t Pin, bool OutOrIn>
	inline bool DigitalRead();

	// 从指定的数字引脚读取值（HIGH或LOW）。根据引脚当前配置为输入还是输出，决定读取输入值还是输出值。由于存在检查当前引脚配置的开销，建议尽可能选择指定读取输入值还是输出值的重载。
	inline bool DigitalRead(uint8_t Pin);
	template <uint8_t Pin>
	inline bool DigitalRead();
	
	// 写一个HIGH或LOW值到数字引脚。
	inline void DigitalWrite(uint8_t Pin, bool HighOrLow);
	template <uint8_t Pin>
	inline void DigitalWrite(bool HighOrLow);
	template <bool HighOrLow>
	inline void DigitalWrite(uint8_t Pin);
	template <uint8_t Pin, bool HighOrLow>
	inline void DigitalWrite();

	// 反转数字引脚的输出状态：若为HIGH则变LOW，若为LOW则变HIGH
	inline void DigitalToggle(uint8_t Pin);
	template <uint8_t Pin>
	inline void DigitalToggle();

	/*
	引脚中断操作。注意并非所有引脚都支持中断，使用PinInterruptable可以检查指定引脚是否支持中断。本库提供的中断附加和移除操作可以按需与内置方法混用，不要求附加和移除方法配对使用，不会产生异常。中断不会排队，如果在处理前发生多次中断，也只会处理一次。还可以按需手动取消中断事件。
	*/

	// 检查指定引脚是否支持中断功能。始终在使用中断前检查引脚支持性。出于性能考虑，本库中其它中断操作均不会检查引脚支持性，尝试对不支持的引脚附加中断是未定义行为。
	inline constexpr bool PinInterruptable(uint8_t Pin);

	// 检查全局设置中断是否启用。如未启用，所有中断均不生效。使用内置interrupts()和noInterrupts()来启用和禁用全局中断。
	inline bool GlobalInterruptEnabled();

	// 将任意可调用对象作为指定引脚的中断处理方法。此方法仅用于支持复杂的可调用对象，实际性能低于内置attachInterrupt，无论是在附加时还是在中断处理时都会有额外开销。如果你只需要附加一个简单的函数指针，应使用内置方法。对象会在下次调用AttachInterrupt（非内置）时被析构，在那之前其所拥有的资源将不会被释放。
	inline void AttachInterrupt(uint8_t Pin, std::move_only_function<void() const> &&ISR, int Mode);
	template <uint8_t Pin>
	inline void AttachInterrupt(std::move_only_function<void() const> &&ISR, int Mode);
	template <int Mode>
	inline void AttachInterrupt(uint8_t Pin, std::move_only_function<void() const> &&ISR);
	template <uint8_t Pin, int Mode>
	inline void AttachInterrupt(std::move_only_function<void() const> &&ISR);

	// 检查指定引脚是否已附加中断
	template <uint8_t Pin>
	inline bool InterruptEnabled();
	inline bool InterruptEnabled(uint8_t Pin);

	// 停止处理指定引脚的中断。如果引脚未附加中断，不会产生异常。此方法不会析构AttachInterrupt传入的可调用对象。
	template <uint8_t Pin>
	inline void DetachInterrupt();
	inline void DetachInterrupt(uint8_t Pin);

#ifdef ARDUINO_ARCH_AVR
	// AVR架构专有方法

	// 检查指定引脚是否有中断事件待处理
	template<uint8_t Pin>
	inline bool InterruptPending();
	inline bool InterruptPending(uint8_t Pin);

	// 取消指定引脚的待处理中断事件。如果引脚未竖起中断旗帜，不会产生异常。
	template <uint8_t Pin>
	inline void ClearInterrupt();
	inline void ClearInterrupt(uint8_t Pin);
#endif
#ifdef ARDUINO_ARCH_SAM
	// SAM架构专有方法

	// 检查指定引脚是否有中断事件待处理。由于SAM架构设计使然，此操作存在副作用，将会清除包括指定引脚在内的多个引脚的中断旗帜，请谨慎使用。
	template<uint8_t Pin>
	inline bool ClearInterruptPending();
	inline bool ClearInterruptPending(uint8_t Pin);
#endif
}
```
#pragma once
#ifdef ARDUINO_ARCH_AVR
#include "LLQDIO/AVR.hpp"
#endif
#ifdef ARDUINO_ARCH_SAM
#include "LLQDIO/SAM.hpp"
#endif
//所有引脚操作前必须确认引脚是否有效，确认方法是检查引脚号是否小于NUM_DIGITAL_PINS。所有中断操作前必须确认中断是否有效，确认方法是检查digitalPinToInterrupt(Pin)是否不等于NOT_AN_INTERRUPT
namespace Low_level_quick_digital_IO
{
	// 获知指定引脚配置为输入还是输出。
	inline bool PinMode(uint8_t Pin) _LLQDIO_PinMode_Get(Dynamic);
	// 获知指定引脚配置为输入还是输出。
	template <uint8_t Pin>
	inline bool PinMode() _LLQDIO_PinMode_Get(Static);
	// 将指定引脚配置为输入或输出。
	inline void PinMode(uint8_t Pin, bool OutOrIn) _LLQDIO_PinMode_Set(Dynamic);
	// 将指定引脚配置为输入或输出。
	template <uint8_t Pin>
	inline void PinMode(bool OutOrIn) _LLQDIO_PinMode_Set(Static);
	// 将指定引脚配置为输入或输出。
	template <bool OutOrIn>
	inline void PinMode(uint8_t Pin) _LLQDIO_PinMode_Set(Dynamic);
	// 将指定引脚配置为输入或输出。
	template <uint8_t Pin, bool OutOrIn>
	inline void PinMode() _LLQDIO_PinMode_Set(Static);
	// 从指定的数字引脚读取值（HIGH或LOW）。额外指定读取输入值还是输出值。
	inline bool DigitalRead(uint8_t Pin, bool OutOrIn) _LLQDIO_DigitalRead(Dynamic);
	// 从指定的数字引脚读取值（HIGH或LOW）。额外指定读取输入值还是输出值。
	template <uint8_t Pin>
	inline bool DigitalRead(bool OutOrIn) _LLQDIO_DigitalRead(Static);
	// 从指定的数字引脚读取值（HIGH或LOW）。额外指定读取输入值还是输出值。
	template <bool OutOrIn>
	inline bool DigitalRead(uint8_t Pin) _LLQDIO_DigitalRead(Dynamic);
	// 从指定的数字引脚读取值（HIGH或LOW）。额外指定读取输入值还是输出值。
	template <uint8_t Pin, bool OutOrIn>
	inline bool DigitalRead() _LLQDIO_DigitalRead(Static);
	// 从指定的数字引脚读取值（HIGH或LOW）。根据引脚当前配置为输入还是输出，决定读取输入值还是输出值。
	inline bool DigitalRead(uint8_t Pin)
	{
		return DigitalRead(Pin, PinMode(Pin));
	}
	// 从指定的数字引脚读取值（HIGH或LOW）。根据引脚当前配置为输入还是输出，决定读取输入值还是输出值。
	template <uint8_t Pin>
	inline bool DigitalRead()
	{
		return DigitalRead<Pin>(PinMode<Pin>());
	}
	// 写一个HIGH或LOW值到数字引脚。
	inline void DigitalWrite(uint8_t Pin, bool HighOrLow) _LLQDIO_DigitalWrite(Dynamic);
	// 写一个HIGH或LOW值到数字引脚。
	template <uint8_t Pin>
	inline void DigitalWrite(bool HighOrLow) _LLQDIO_DigitalWrite(Static);
	// 写一个HIGH或LOW值到数字引脚。
	template <bool HighOrLow>
	inline void DigitalWrite(uint8_t Pin) _LLQDIO_DigitalWrite(Dynamic);
	// 写一个HIGH或LOW值到数字引脚。
	template <uint8_t Pin, bool HighOrLow>
	inline void DigitalWrite() _LLQDIO_DigitalWrite(Static);
	// 反转数字引脚的输出状态：若为HIGH则变LOW，若为LOW则变HIGH
	inline void DigitalToggle(uint8_t Pin) _LLQDIO_DigitalToggle(Dynamic);
	// 反转数字引脚的输出状态：若为HIGH则变LOW，若为LOW则变HIGH
	template <uint8_t Pin>
	inline void DigitalToggle() _LLQDIO_DigitalToggle(Static);
	// 将任意可调用对象作为指定引脚的中断处理方法。输入不支持中断的引脚是未定义行为。
	inline void AttachInterrupt(uint8_t Pin, std::move_only_function<void() const> &&ISR, int Mode)
	{
		const _PinCommonIsr &PCI = _GetPinCommonIsr(Pin);
		PCI.PinIsr = std::move(ISR);
		//AVR无法优化attachInterrupt，因为牵扯到static变量，外部无法访问
		attachInterrupt(digitalPinToInterrupt(Pin), PCI.CommonIsr, Mode);
	}
	// 将任意可调用对象作为指定引脚的中断处理方法
	template <uint8_t Pin>
	inline void AttachInterrupt(std::move_only_function<void() const> &&ISR, int Mode)
	{
		_PinIsr<Pin> = std::move(ISR);
		attachInterrupt(digitalPinToInterrupt(Pin), _CommonIsr<Pin>, Mode);
	}
	// 将任意可调用对象作为指定引脚的中断处理方法
	template <int Mode>
	inline void AttachInterrupt(uint8_t Pin, std::move_only_function<void() const> &&ISR)
	{
		const _PinCommonIsr &PCI = _GetPinCommonIsr(Pin);
		PCI.PinIsr = std::move(ISR);
		attachInterrupt(digitalPinToInterrupt(Pin), PCI.CommonIsr, Mode);
	}
	// 将任意可调用对象作为指定引脚的中断处理方法
	template <uint8_t Pin, int Mode>
	inline void AttachInterrupt(std::move_only_function<void() const> &&ISR)
	{
		_PinIsr<Pin> = std::move(ISR);
		attachInterrupt(digitalPinToInterrupt(Pin), _CommonIsr<Pin>, Mode);
	}
}
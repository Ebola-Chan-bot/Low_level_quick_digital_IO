#pragma once
#ifdef ARDUINO_ARCH_AVR
#include "_QDII/AVR.hpp"
#endif
#ifdef ARDUINO_ARCH_SAM
#include "_QDII/SAM.hpp"
#endif
//所有引脚操作前必须确认引脚是否有效，确认方法是检查引脚号是否小于NUM_DIGITAL_PINS。所有中断操作前必须确认中断是否有效，确认方法是检查digitalPinToInterrupt(Pin)是否不等于NOT_AN_INTERRUPT
namespace Quick_digital_IO_interrupt {
	// // 获知引脚工作于OUTPUT还是INPUT模式。
	inline bool PinMode(uint8_t Pin) _QDII_PinMode_Get(Dynamic);
	// // 获知引脚工作于OUTPUT还是INPUT模式。
	template <uint8_t Pin>
	inline bool PinMode() _QDII_PinMode_Get(Static);

	// 切换引脚工作模式到OUTPUT或INPUT。
	inline void PinMode(uint8_t Pin, bool OutOrIn) _QDII_PinMode_Set(Dynamic);
	// 切换引脚工作模式到OUTPUT或INPUT。
	template <uint8_t Pin>
	inline void PinMode(bool OutOrIn) _QDII_PinMode_Set(Static);
	// 切换引脚工作模式到OUTPUT或INPUT。
	template <bool OutOrIn>
	inline void PinMode(uint8_t Pin) _QDII_PinMode_Set(Dynamic);
	// 切换引脚工作模式到OUTPUT或INPUT。
	template <uint8_t Pin, bool OutOrIn>
	inline void PinMode() _QDII_PinMode_Set(Static);

	// 从引脚读取状态（HIGH或LOW）。额外指定读取OUTPUT还是INPUT模式的状态。
	inline bool DigitalRead(uint8_t Pin, bool OutOrIn) _QDII_DigitalRead(Dynamic);
	// 从引脚读取状态（HIGH或LOW）。额外指定读取OUTPUT还是INPUT模式的状态。
	template <uint8_t Pin>
	inline bool DigitalRead(bool OutOrIn) _QDII_DigitalRead(Static);
	// 从引脚读取状态（HIGH或LOW）。额外指定读取OUTPUT还是INPUT模式的状态。
	template <bool OutOrIn>
	inline bool DigitalRead(uint8_t Pin) _QDII_DigitalRead(Dynamic);
	// 从引脚读取状态（HIGH或LOW）。额外指定读取OUTPUT还是INPUT模式的状态。
	template <uint8_t Pin, bool OutOrIn>
	inline bool DigitalRead() _QDII_DigitalRead(Static);

	// 从引脚读取状态（HIGH或LOW）。根据引脚当前工作模式，决定读取OUTPUT还是INPUT状态。由于存在检查工作模式的开销，建议尽可能选择指定读取OUTPUT还是INPUT状态的重载。
	inline bool DigitalRead(uint8_t Pin) {
		return DigitalRead(Pin, PinMode(Pin));
	}
	// 从引脚读取状态（HIGH或LOW）。根据引脚当前工作模式，决定读取OUTPUT还是INPUT状态。由于存在检查工作模式的开销，建议尽可能选择指定读取OUTPUT还是INPUT状态的重载。
	template <uint8_t Pin>
	inline bool DigitalRead() {
		return DigitalRead<Pin>(PinMode<Pin>());
	}

	// 将引脚OUTPUT状态设为HIGH或LOW。如果引脚当前工作于INPUT模式，此操作不会立即改变引脚的实际输出，只有切换到OUTPUT模式后才会生效。
	inline void DigitalWrite(uint8_t Pin, bool HighOrLow) _QDII_DigitalWrite(Dynamic);
	// 将引脚OUTPUT状态设为HIGH或LOW。如果引脚当前工作于INPUT模式，此操作不会立即改变引脚的实际输出，只有切换到OUTPUT模式后才会生效。
	template <uint8_t Pin>
	inline void DigitalWrite(bool HighOrLow) _QDII_DigitalWrite(Static);
	// 将引脚OUTPUT状态设为HIGH或LOW。如果引脚当前工作于INPUT模式，此操作不会立即改变引脚的实际输出，只有切换到OUTPUT模式后才会生效。
	template <bool HighOrLow>
	inline void DigitalWrite(uint8_t Pin) _QDII_DigitalWrite(Dynamic);
	// 将引脚OUTPUT状态设为HIGH或LOW。如果引脚当前工作于INPUT模式，此操作不会立即改变引脚的实际输出，只有切换到OUTPUT模式后才会生效。
	template <uint8_t Pin, bool HighOrLow>
	inline void DigitalWrite() _QDII_DigitalWrite(Static);

	// 反转引脚的OUTPUT状态：若为HIGH则变LOW，若为LOW则变HIGH
	inline void DigitalToggle(uint8_t Pin) _QDII_DigitalToggle(Dynamic);
	// 反转引脚的OUTPUT状态：若为HIGH则变LOW，若为LOW则变HIGH
	template <uint8_t Pin>
	inline void DigitalToggle() _QDII_DigitalToggle(Static);

	// 检查引脚是否支持中断功能。出于性能考虑，本库中其它中断操作均不会检查引脚支持性，尝试对不支持的引脚附加中断是未定义行为。
	inline constexpr bool PinInterruptable(uint8_t Pin) {
		return digitalPinToInterrupt(Pin) != NOT_AN_INTERRUPT;
	}

	// 将任意可调用对象作为引脚的中断处理方法，并设置中断条件。此方法仅用于支持复杂的可调用对象，实际性能低于内置attachInterrupt，无论是在附加时还是在中断处理时都会有额外开销。如果你只需要附加一个简单的函数指针，应使用内置方法。对象会在下次调用AttachInterrupt（非内置）时被析构，在那之前其所拥有的资源将不会被释放。
	inline void AttachInterrupt(uint8_t Pin, std::move_only_function<void() const>&& ISR, int Mode) {
		const _PinCommonIsr& PCI = _GetPinCommonIsr(Pin);
		PCI.PinIsr = std::move(ISR);
		//无法优化attachInterrupt，因为牵扯到static变量，外部无法访问
		attachInterrupt(digitalPinToInterrupt(Pin), PCI.CommonIsr, Mode);
	}
	// 将任意可调用对象作为引脚的中断处理方法，并设置中断条件。此方法仅用于支持复杂的可调用对象，实际性能低于内置attachInterrupt，无论是在附加时还是在中断处理时都会有额外开销。如果你只需要附加一个简单的函数指针，应使用内置方法。对象会在下次调用AttachInterrupt（非内置）时被析构，在那之前其所拥有的资源将不会被释放。
	template <uint8_t Pin>
	inline void AttachInterrupt(std::move_only_function<void() const>&& ISR, int Mode) {
		_CSL_Struct14Value(_PinIsr, Pin) = std::move(ISR);
		attachInterrupt(digitalPinToInterrupt(Pin), _CommonIsr<Pin>, Mode);
	}
	// 将任意可调用对象作为引脚的中断处理方法，并设置中断条件。此方法仅用于支持复杂的可调用对象，实际性能低于内置attachInterrupt，无论是在附加时还是在中断处理时都会有额外开销。如果你只需要附加一个简单的函数指针，应使用内置方法。对象会在下次调用AttachInterrupt（非内置）时被析构，在那之前其所拥有的资源将不会被释放。
	template <int Mode>
	inline void AttachInterrupt(uint8_t Pin, std::move_only_function<void() const>&& ISR) {
		const _PinCommonIsr& PCI = _GetPinCommonIsr(Pin);
		PCI.PinIsr = std::move(ISR);
		attachInterrupt(digitalPinToInterrupt(Pin), PCI.CommonIsr, Mode);
	}
	// 将任意可调用对象作为引脚的中断处理方法，并设置中断条件。此方法仅用于支持复杂的可调用对象，实际性能低于内置attachInterrupt，无论是在附加时还是在中断处理时都会有额外开销。如果你只需要附加一个简单的函数指针，应使用内置方法。对象会在下次调用AttachInterrupt（非内置）时被析构，在那之前其所拥有的资源将不会被释放。
	template <uint8_t Pin, int Mode>
	inline void AttachInterrupt(std::move_only_function<void() const>&& ISR) {
		_CSL_Struct14Value(_PinIsr, Pin) = std::move(ISR);
		attachInterrupt(digitalPinToInterrupt(Pin), _CommonIsr<Pin>, Mode);
	}
}
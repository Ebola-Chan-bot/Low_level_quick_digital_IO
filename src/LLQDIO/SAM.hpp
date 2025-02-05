#pragma once
#include "CommonTypes.hpp"
namespace Low_level_quick_digital_IO {
	// 用于初始化引脚参数的内部功能，一般不应直接调用
	namespace Internal {
#pragma push_macro("__cplusplus")
#undef __cplusplus
#define PinDescription constexpr PinDescription
#include <variant.cpp>
#undef PinDescription
#pragma pop_macro("__cplusplus")
#define _LLQDIO_Dynamic
#define _LLQDIO_Static Internal::
#define _LLQDIO_PinMode_Get(DS)                                                                                \
	{                                                                                                          \
		return _LLQDIO_##DS g_APinDescription[Pin].pPort->PIO_OSR & _LLQDIO_##DS g_APinDescription[Pin].ulPin; \
	}
#define _LLQDIO_PinMode_Set(DS)                                                                                                                                          \
	{                                                                                                                                                                    \
		(OutOrIn ? _LLQDIO_##DS g_APinDescription[Pin].pPort->PIO_OER : _LLQDIO_##DS g_APinDescription[Pin].pPort->PIO_ODR) = _LLQDIO_##DS g_APinDescription[Pin].ulPin; \
	}
#define _LLQDIO_DigitalRead(DS)                                                                                                                                                   \
	{                                                                                                                                                                             \
		return (OutOrIn ? _LLQDIO_##DS g_APinDescription[Pin].pPort->PIO_ODSR : _LLQDIO_##DS g_APinDescription[Pin].pPort->PIO_PDSR) & _LLQDIO_##DS g_APinDescription[Pin].ulPin; \
	}
#define _LLQDIO_DigitalWrite(DS)                                                                                                                                             \
	{                                                                                                                                                                        \
		(HighOrLow ? _LLQDIO_##DS g_APinDescription[Pin].pPort->PIO_SODR : _LLQDIO_##DS g_APinDescription[Pin].pPort->PIO_CODR) = _LLQDIO_##DS g_APinDescription[Pin].ulPin; \
	}
#define _LLQDIO_DigitalToggle(DS)                                                             \
	{                                                                                         \
		const PinDescription &PD = _LLQDIO_##DS g_APinDescription[Pin];                       \
		(PD.pPort->PIO_ODSR & PD.ulPin ? PD.pPort->PIO_CODR : PD.pPort->PIO_SODR) = PD.ulPin; \
	}
	}
	// 检查指定引脚是否已附加中断。如果引脚未附加中断，不会产生异常。
	inline bool InterruptEnabled(uint8_t Pin) {
		return g_APinDescription[Pin].ulPin & g_APinDescription[Pin].pPort->PIO_IMR;
	}
	// 检查指定引脚是否已附加中断。如果引脚未附加中断，不会产生异常。
	template <uint8_t Pin>
	inline bool InterruptEnabled() {
		return Internal::g_APinDescription[Pin].ulPin & Internal::g_APinDescription[Pin].pPort->PIO_IMR;
	}
	template <uint8_t... Pin>
	struct _PinIsrMap<std::integer_sequence<uint8_t, Pin...>> {
		static constexpr _PinCommonIsr value[NUM_DIGITAL_PINS] = { {_CSL_Struct14Value(_PinIsr, Pin), _CommonIsr<Pin>}... };
	};
	// 停止处理指定引脚的中断。如果引脚未附加中断，不会产生异常。此方法不会析构AttachInterrupt传入的可调用对象。
	inline void DetachInterrupt(uint8_t Pin) {
		g_APinDescription[Pin].pPort->PIO_IDR = g_APinDescription[Pin].ulPin;
	}
	// 停止处理指定引脚的中断。如果引脚未附加中断，不会产生异常。此方法不会析构AttachInterrupt传入的可调用对象。
	template <uint8_t Pin>
	inline void DetachInterrupt() {
		Internal::g_APinDescription[Pin].pPort->PIO_IDR = Internal::g_APinDescription[Pin].ulPin;
	}
	constexpr const _PinCommonIsr& _GetPinCommonIsr(uint8_t Pin) {
		return _PinIsrMap<std::make_integer_sequence<uint8_t, NUM_DIGITAL_PINS>>::value[Pin];
	}
	// 检查指定引脚是否有中断事件待处理。由于SAM架构设计使然，此操作存在副作用，将会清除包括指定引脚在内的多个引脚的中断旗帜，请谨慎使用。
	template<uint8_t Pin>
	inline bool ClearInterruptPending() {
		return Internal::g_APinDescription[Pin].pPort->PIO_ISR & Internal::g_APinDescription[Pin].ulPin;
	}
	// 检查指定引脚是否有中断事件待处理。由于SAM架构设计使然，此操作存在副作用，将会清除包括指定引脚在内的多个引脚的中断旗帜，请谨慎使用。
	inline bool ClearInterruptPending(uint8_t Pin) {
		return g_APinDescription[Pin].pPort->PIO_ISR & g_APinDescription[Pin].ulPin;
	}
	// 检查全局设置中断是否启用。如未启用，所有中断均不生效。使用内置interrupts()和noInterrupts()函数来启用和禁用全局中断。
	inline bool GlobalInterruptEnabled() {
		return !__get_PRIMASK();
	}
}
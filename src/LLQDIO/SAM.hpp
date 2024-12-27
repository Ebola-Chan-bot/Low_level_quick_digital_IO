#pragma once
#include "CommonTypes.hpp"
namespace Low_level_quick_digital_IO
{
	// 用于初始化引脚参数的内部功能，一般不应直接调用
	namespace Internal
	{
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
	// 检查指定引脚是否启用了中断
	inline bool InterruptEnabled(uint8_t Pin)
	{
		return g_APinDescription[Pin].ulPin & g_APinDescription[Pin].pPort->PIO_IMR;
	}
	// 检查指定引脚是否启用了中断
	template <uint8_t Pin>
	inline bool InterruptEnabled()
	{
		return Internal::g_APinDescription[Pin].ulPin & Internal::g_APinDescription[Pin].pPort->PIO_IMR;
	}
	template <uint8_t... Pin>
	struct _PinIsrMap<std::integer_sequence<uint8_t, Pin...>>
	{
		static constexpr _PinCommonIsr value[NUM_DIGITAL_PINS] = {{_CSL_Struct14Value(_PinIsr, Pin), _CommonIsr<Pin>}...};
	};
	// 停止处理指定引脚的中断
	inline void DetachInterrupt(uint8_t Pin)
	{
		g_APinDescription[Pin].pPort->PIO_IDR = g_APinDescription[Pin].ulPin;
	}
	// 停止处理指定引脚的中断
	template <uint8_t Pin>
	inline void DetachInterrupt()
	{
		Internal::g_APinDescription[Pin].pPort->PIO_IDR = Internal::g_APinDescription[Pin].ulPin;
	}
	constexpr const _PinCommonIsr &_GetPinCommonIsr(uint8_t Pin)
	{
		return _PinIsrMap<std::make_integer_sequence<uint8_t, NUM_DIGITAL_PINS>>::value[Pin];
	}
}
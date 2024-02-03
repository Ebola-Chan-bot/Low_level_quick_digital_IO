#pragma once
#include <Arduino.h>
namespace Low_level_quick_digital_IO
{
#ifdef ARDUINO_ARCH_AVR
#if __cplusplus < 201703L
#error 对AVR架构，必须使用C++17以上的标准编译
#endif
	// 用于初始化引脚参数的内部功能，一般不应直接调用
	namespace Internal
	{
		static volatile uint8_t *GetOutputRegister(uint8_t Pin)
		{
			return portOutputRegister(digitalPinToPort(Pin));
		}
		static uint8_t GetBitMask(uint8_t Pin)
		{
			return digitalPinToBitMask(Pin);
		}
		template <uint8_t Pin>
		static volatile uint8_t *const OutputRegister = GetOutputRegister(Pin);
		template <uint8_t Pin>
		static const uint8_t BitMask = GetBitMask(Pin);
	}
	// 引脚读取
	template <uint8_t Pin>
	inline bool DigitalRead()
	{
		static volatile uint8_t *const InputRegister = portInputRegister(digitalPinToPort(Pin));
		return *InputRegister<Pin> & Internal::BitMask<Pin>;
	}
	// 引脚写入
	template <uint8_t Pin, bool HighOrLow>
	inline void DigitalWrite()
	{
		// 对模板参数进行判断在编译期进行，自动剪除未运行的支路，运行时无需重复判断
		if (HighOrLow)
			*Internal::OutputRegister<Pin> |= Internal::BitMask<Pin>;
		else
			*Internal::OutputRegister<Pin> &= ~Internal::BitMask<Pin>;
	}
	// 引脚反转
	template <uint8_t Pin>
	inline void DigitalToggle()
	{
		*Internal::OutputRegister<Pin> ^= Internal::BitMask<Pin>;
	}
#endif
#ifdef ARDUINO_ARCH_SAM
	template <uint8_t Pin>
	struct Internal
	{
		static const uint32_t PinMask;
		static Pio *const Port;
	};
	template <uint8_t Pin>
	const uint32_t Internal<Pin>::PinMask = g_APinDescription[Pin].ulPin;
	template <uint8_t Pin>
	Pio *const Internal<Pin>::Port = g_APinDescription[Pin].pPort;
	template <uint8_t Pin>
	inline bool DigitalRead()
	{
		return Internal<Pin>::Port->PIO_PDSR & Internal<Pin>::PinMask;
	}
	template <uint8_t Pin, bool HighOrLow>
	inline void DigitalWrite()
	{
		(HighOrLow ? Internal<Pin>::Port->PIO_SODR : Internal<Pin>::Port->PIO_CODR) = Internal<Pin>::PinMask;
	}
	template <uint8_t Pin>
	inline void DigitalToggle()
	{
		(Internal<Pin>::Port->PIO_OSR ? Internal<Pin>::Port->PIO_SODR : Internal<Pin>::Port->PIO_CODR) = Internal<Pin>::PinMask;
	}
#endif
}
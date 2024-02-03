#pragma once
#include <Arduino.h>
namespace Low_level_quick_digital_IO
{
	// 用于初始化引脚参数的内部功能，一般不应直接调用
	namespace Internal
	{
		static inline volatile uint8_t *GetInputRegister(uint8_t PinCode)
		{
			return portInputRegister(digitalPinToPort(PinCode));
		}
		static inline volatile uint8_t *GetOutputRegister(uint8_t PinCode)
		{
			return portOutputRegister(digitalPinToPort(PinCode));
		}
		static inline uint8_t GetMask(uint8_t PinCode)
		{
			return digitalPinToBitMask(PinCode);
		}
#ifndef _HAS_CXX17
#define _HAS_CXX17 __cplusplus > 201402L
#endif
#if _HAS_CXX17
		// 仅第一次调用时需要计算端口和掩码，后续直接使用，实现加速
		template <uint8_t PinCode>
		static volatile uint8_t *const InputRegister = GetInputRegister(PinCode);
		template <uint8_t PinCode>
		static volatile uint8_t *const OutputRegister = GetOutputRegister(PinCode);
		template <uint8_t PinCode>
		static const uint8_t BitMask = GetMask(PinCode);
#else
		template <uint8_t PinCode>
		struct PinConstants
		{
			static volatile uint8_t *const InputRegister = GetInputRegister(PinCode);
			static volatile uint8_t *const OutputRegister = GetOutputRegister(PinCode);
			static const uint8_t BitMask = GetMask(PinCode);
		};
#endif
	}
	using namespace Internal;
	// 引脚读取
	template <uint8_t PinCode>
	inline bool DigitalRead()
	{
		return *InputRegister<PinCode> & BitMask<PinCode>;
	}
	// 引脚写入
	template <uint8_t PinCode, bool HighOrLow>
	inline void DigitalWrite()
	{
		// 对模板参数进行判断在编译期进行，自动剪除未运行的支路，运行时无需重复判断
		if (HighOrLow)
			*OutputRegister<PinCode> |= BitMask<PinCode>;
		else
			*OutputRegister<PinCode> &= ~BitMask<PinCode>;
	}
	// 引脚反转
	template <uint8_t PinCode>
	inline void DigitalToggle()
	{
		*OutputRegister<PinCode> ^= BitMask<PinCode>;
	}
}
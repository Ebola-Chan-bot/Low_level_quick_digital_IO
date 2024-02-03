#pragma once
#include <Arduino.h>
namespace Low_level_quick_digital_IO {
#ifdef ARDUINO_ARCH_AVR
#if __cplusplus<201703L
#error("对于AVR架构，编译器必须支持C++17以上")
#endif
	// 用于初始化引脚参数的内部功能，一般不应直接调用
	namespace Internal {
		static inline volatile uint8_t* GetOutputRegister(uint8_t PinCode) {
			return portOutputRegister(digitalPinToPort(PinCode));
		}
		static inline uint8_t GetMask(uint8_t PinCode) {
			return digitalPinToBitMask(PinCode);
		}
		// 仅第一次调用时需要计算端口和掩码，后续直接使用，实现加速
		template <uint8_t PinCode>
		static volatile uint8_t* const OutputRegister = GetOutputRegister(PinCode);
		template <uint8_t PinCode>
		static const uint8_t BitMask = GetMask(PinCode);
	}
	using namespace Internal;
	// 引脚读取
	template <uint8_t PinCode>
	inline bool DigitalRead() {
		static volatile uint8_t* const InputRegister = portInputRegister(digitalPinToPort(PinCode));
		return *InputRegister<PinCode> &BitMask<PinCode>;
	}
	// 引脚写入
	template <uint8_t PinCode, bool HighOrLow>
	inline void DigitalWrite() {
		// 对模板参数进行判断在编译期进行，自动剪除未运行的支路，运行时无需重复判断
		if (HighOrLow)
			*OutputRegister<PinCode> |= BitMask<PinCode>;
		else
			*OutputRegister<PinCode> &= ~BitMask<PinCode>;
	}
	// 引脚反转
	template <uint8_t PinCode>
	inline void DigitalToggle() {
		*OutputRegister<PinCode> ^= BitMask<PinCode>;
	}
#endif
#ifdef ARDUINO_ARCH_SAM
	template <uint8_t Pin>
	inline bool DigitalRead() {
		static const RoReg& PIO_PDSR = g_APinDescription[Pin].pPort->PIO_PDSR;
		static const uint32_t PinMask = g_APinDescription[Pin].ulPin;
		return PIO_PDSR & PinMask;
	}
	template<uint8_t Pin, bool HighOrLow>
	inline void DigitalWrite() {
		g_pinStatus[ulPin] = (g_pinStatus[ulPin] & 0x0F) | (ulVal << 4);
		if (PIO_GetOutputDataStatus(g_APinDescription[ulPin].pPort, g_APinDescription[ulPin].ulPin) == 0) {
			PIO_PullUp(g_APinDescription[ulPin].pPort, g_APinDescription[ulPin].ulPin, ulVal);
		}
		else {
			PIO_SetOutput(g_APinDescription[ulPin].pPort, g_APinDescription[ulPin].ulPin, ulVal, 0, PIO_PULLUP);
		}
	}
#endif
}
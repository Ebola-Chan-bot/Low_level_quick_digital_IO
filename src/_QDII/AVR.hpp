#pragma once
#include "CommonTypes.hpp"
#include <wiring_private.h>
namespace Quick_digital_IO_interrupt {
	// 用于初始化引脚参数的内部功能，一般不应直接调用
	namespace Internal {
#undef Pins_Arduino_h
#define ARDUINO_MAIN
#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6
#define PG 7
#define PH 8
#define PJ 10
#define PK 11
#define PL 12
#define PROGMEM constexpr
#include <pins_arduino.h>
#define PROGMEM __ATTR_PROGMEM__
#undef ARDUINO_MAIN
#undef PA 1
#undef PB 2
#undef PC 3
#undef PD 4
#undef PE 5
#undef PF 6
#undef PG 7
#undef PH 8
#undef PJ 10
#undef PK 11
#undef PL 12
#define _QDII_Dynamic_PTM *(volatile uint8_t *)portModeRegister(digitalPinToPort(Pin))
#define _QDII_Static_PTM *(volatile uint8_t *)Internal::port_to_mode_PGM[Internal::digital_pin_to_port_PGM[Pin]]
#define _QDII_Dynamic_PTB digitalPinToBitMask(Pin)
#define _QDII_Static_PTB Internal::digital_pin_to_bit_mask_PGM[Pin]
#define _QDII_Dynamic_PTI *(volatile uint8_t *)portInputRegister(digitalPinToPort(Pin))
#define _QDII_Static_PTI *(volatile uint8_t *)Internal::port_to_input_PGM[Internal::digital_pin_to_port_PGM[Pin]]
#define _QDII_Dynamic_PTO *(volatile uint8_t *)portOutputRegister(digitalPinToPort(Pin))
#define _QDII_Static_PTO *(volatile uint8_t *)Internal::port_to_output_PGM[Internal::digital_pin_to_port_PGM[Pin]]
#define _QDII_PinMode_Get(DS)    \
	{                              \
		return _QDII_##DS##_PTM; \
	}
#define _QDII_PinMode_Set(DS)                        \
	{                                                  \
		if (OutOrIn)                                   \
			_QDII_##DS##_PTM |= _QDII_##DS##_PTB;  \
		else                                           \
			_QDII_##DS##_PTM &= ~_QDII_##DS##_PTB; \
	}
#define _QDII_DigitalRead(DS)                                   \
	{                                                             \
		return OutOrIn ? _QDII_##DS##_PTO : _QDII_##DS##_PTI; \
	}
#define _QDII_DigitalWrite(DS)                       \
	{                                                  \
		if (HighOrLow)                                 \
			_QDII_##DS##_PTO |= _QDII_##DS##_PTB;  \
		else                                           \
			_QDII_##DS##_PTO &= ~_QDII_##DS##_PTB; \
	}
#define _QDII_DigitalToggle(DS)                 \
	{                                             \
		_QDII_##DS##_PTO ^= _QDII_##DS##_PTB; \
	}
	}
	constexpr volatile uint8_t& _InterruptRegister =
#ifdef EIMSK
		EIMSK
#elif defined(GICR)
		GICR
#elif defined(GIMSK)
		GIMSK
#else
#error detachInterrupt not finished for this cpu
#endif
		;
	template <uint8_t... Offset>
	struct _InterruptMask {
		static constexpr std::remove_cvref_t<decltype(_InterruptRegister)> INT[] = { 1 << Offset... };
	};
	using _InterruptMask_t = _InterruptMask<
#if defined(__AVR_ATmega32U4__)
		INT0, INT1, INT2, INT3, INT6
#elif defined(__AVR_AT90USB82__) || defined(__AVR_AT90USB162__) || defined(__AVR_ATmega32U2__) || defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega8U2__)
		INT0, INT1, INT2, INT3, INT4, INT5, INT6, INT7
#elif defined(EICRA) && defined(EICRB) && defined(EIMSK)
		INT4, INT5, INT0, INT1, INT2, INT3, INT6, INT7
#else
		INT0, INT1, INT2
#endif
	>;
	template <uint8_t... Pin>
	struct _PinIsrMap<std::integer_sequence<uint8_t, Pin...>> {
		static constexpr _PinCommonIsr value[EXTERNAL_NUM_INTERRUPTS] = { {_CSL_Struct14Value(_PinIsr, Pin), _CommonIsr<Pin>}... };
	};
	constexpr const _PinCommonIsr& _GetPinCommonIsr(uint8_t Pin) {
		return _PinIsrMap<std::make_integer_sequence<uint8_t, EXTERNAL_NUM_INTERRUPTS>>::value[digitalPinToInterrupt(Pin)];
	}

	// 检查指定引脚是否已附加中断，即处于监听或挂起状态。
	inline bool InterruptEnabled(uint8_t Pin) {
		return _InterruptRegister & _InterruptMask_t::INT[digitalPinToInterrupt(Pin)];
	}
	// 检查指定引脚是否已附加中断，即处于监听或挂起状态。
	template <uint8_t Pin>
	inline bool InterruptEnabled() {
		return _InterruptRegister & _InterruptMask_t::INT[digitalPinToInterrupt(Pin)];
	}
	
	// 停止处理指定引脚的中断。如果引脚空闲或空闲但挂起，也不会产生异常。此方法不会析构AttachInterrupt传入的可调用对象。此方法是AttachInterrupt的逆操作。
	inline void DetachInterrupt(uint8_t Pin) {
		_InterruptRegister &= ~_InterruptMask_t::INT[digitalPinToInterrupt(Pin)];
	}
	// 停止处理指定引脚的中断。如果引脚空闲或空闲但挂起，也不会产生异常。此方法不会析构AttachInterrupt传入的可调用对象。此方法是AttachInterrupt的逆操作。
	template <uint8_t Pin>
	inline void DetachInterrupt() {
		_InterruptRegister &= ~_InterruptMask_t::INT[digitalPinToInterrupt(Pin)];
	}

	// 检查指定引脚是否有中断事件挂起。
	template<uint8_t Pin>
	inline bool InterruptPending() {
		return EIFR & _InterruptMask_t::INT[digitalPinToInterrupt(Pin)];
	}
	// 检查指定引脚是否有中断事件挂起。
	inline bool InterruptPending(uint8_t Pin) {
		return EIFR & _InterruptMask_t::INT[digitalPinToInterrupt(Pin)];
	}

	// 取消引脚挂起的中断事件。如果引脚未挂起，也不会产生异常。
	template <uint8_t Pin>
	inline void ClearInterrupt() {
		EIFR = _InterruptMask_t::INT[digitalPinToInterrupt(Pin)];
	}
	// 取消引脚挂起的中断事件。如果引脚未挂起，也不会产生异常。
	inline void ClearInterrupt(uint8_t Pin) {
		EIFR = _InterruptMask_t::INT[digitalPinToInterrupt(Pin)];
	}
	
	// 检查全局设置中断是否启用。如禁用，所有引脚的中断均不工作。使用内置interrupts()和noInterrupts()来启用和禁用全局中断。
	inline bool GlobalInterruptEnabled() {
		return SREG & (1 << SREG_I);
	}
}
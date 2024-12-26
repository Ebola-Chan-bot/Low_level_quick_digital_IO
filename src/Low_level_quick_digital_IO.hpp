#pragma once
#include <Arduino.h>
#include <functional>
#include <unordered_map>
namespace Low_level_quick_digital_IO
{
	// 用于初始化引脚参数的内部功能，一般不应直接调用
	namespace Internal
	{
#ifdef ARDUINO_ARCH_AVR
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
#define _LLQDIO_Dynamic_PTM *(volatile uint8_t *)portModeRegister(digitalPinToPort(Pin))
#define _LLQDIO_Static_PTM *(volatile uint8_t *)Internal::port_to_mode_PGM[Internal::digital_pin_to_port_PGM[Pin]]
#define _LLQDIO_Dynamic_PTB digitalPinToBitMask(Pin)
#define _LLQDIO_Static_PTB Internal::digital_pin_to_bit_mask_PGM[Pin]
#define _LLQDIO_Dynamic_PTI *(volatile uint8_t *)portInputRegister(digitalPinToPort(Pin))
#define _LLQDIO_Static_PTI *(volatile uint8_t *)Internal::port_to_input_PGM[Internal::digital_pin_to_port_PGM[Pin]]
#define _LLQDIO_Dynamic_PTO *(volatile uint8_t *)portOutputRegister(digitalPinToPort(Pin))
#define _LLQDIO_Static_PTO *(volatile uint8_t *)Internal::port_to_output_PGM[Internal::digital_pin_to_port_PGM[Pin]]
#define _LLQDIO_PinMode_Get(DS)    \
	{                              \
		return _LLQDIO_##DS##_PTM; \
	}
#define _LLQDIO_PinMode_Set(DS)                        \
	{                                                  \
		if (OutOrIn)                                   \
			_LLQDIO_##DS##_PTM |= _LLQDIO_##DS##_PTB;  \
		else                                           \
			_LLQDIO_##DS##_PTM &= ~_LLQDIO_##DS##_PTB; \
	}
#define _LLQDIO_DigitalRead(DS)                                   \
	{                                                             \
		return OutOrIn ? _LLQDIO_##DS##_PTO : _LLQDIO_##DS##_PTI; \
	}
#define _LLQDIO_DigitalWrite(DS)                       \
	{                                                  \
		if (HighOrLow)                                 \
			_LLQDIO_##DS##_PTO |= _LLQDIO_##DS##_PTB;  \
		else                                           \
			_LLQDIO_##DS##_PTO &= ~_LLQDIO_##DS##_PTB; \
	}
#define _LLQDIO_DigitalToggle(DS)                 \
	{                                             \
		_LLQDIO_##DS##_PTO ^= _LLQDIO_##DS##_PTB; \
	}
#endif
#ifdef ARDUINO_ARCH_SAM
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
#endif
	}
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
#ifdef ARDUINO_ARCH_AVR
	constexpr volatile auto& _InterruptRegister =
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
	template<uint8_t...Offset>
	struct _InterruptMask
	{
		static constexpr uint8_t value[] = { 1 << Offset... };
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
#endif
	// 检查指定引脚是否支持中断
	constexpr bool SupportInterrupt(uint8_t Pin)
	{
		return digitalPinToInterrupt(Pin) != NOT_AN_INTERRUPT;
	}
	// 检查指定引脚是否启用了中断
	inline bool InterruptEnabled(uint8_t Pin)
	{
		return
#ifdef ARDUINO_ARCH_AVR
			_InterruptRegister & _InterruptMask_t::value[digitalPinToInterrupt(Pin)]
#endif
#ifdef ARDUINO_ARCH_SAM
			g_APinDescription[Pin].ulPin & g_APinDescription[Pin].pPort->PIO_IMR
#endif
			;
	}
	// 检查指定引脚是否启用了中断
	template <uint8_t Pin>
	inline bool InterruptEnabled()
	{
		return
#ifdef ARDUINO_ARCH_AVR
			_InterruptRegister & _InterruptMask_t::value[digitalPinToInterrupt(Pin)]
#endif
#ifdef ARDUINO_ARCH_SAM
			Internal::g_APinDescription[Pin].ulPin & Internal::g_APinDescription[Pin].pPort->PIO_IMR
#endif
			;
	}
	template <uint8_t Pin>
#ifdef __cpp_variable_templates
	std::move_only_function<void() const> _PinIsr;
#else
	struct _PinIsr
	{
		static std::move_only_function<void() const> value;
	};
#endif
	template <uint8_t Pin>
	void _CommonIsr()
	{
		_CSL_Struct14Value(_PinIsr, Pin)();
	}
	struct _PinCommonIsr
	{
		std::move_only_function<void()const>& PinIsr;
		void(*CommonIsr)();
	};
#ifdef ARDUINO_ARCH_AVR
	template<uint8_t New, typename T>
	struct _Prepend;
	template<uint8_t New, uint8_t...Old>
	struct _Prepend<New, std::integer_sequence<uint8_t, Old...>>
	{
		using type = std::integer_sequence<uint8_t, New, Old...>;
	};
	template<typename T>
	struct _Interruptable
	{
		using type = std::integer_sequence<uint8_t>;
	};
	template<uint8_t First, uint8_t...Left>
	struct _Interruptable<std::integer_sequence<uint8_t, First, Left...>>
	{
		using type = std::conditional_t<SupportInterrupt(First), typename _Prepend<First, typename _Interruptable<std::integer_sequence<uint8_t, Left...>>::type>::type, typename _Interruptable<std::integer_sequence<uint8_t, Left...>>::type>;
	};
	template<typename T>
	struct _PinIsrMap;
	template<uint8_t...Pin>
	struct _PinIsrMap<std::integer_sequence<uint8_t, Pin...>>
	{
		static const std::unordered_map<uint8_t, _PinCommonIsr> value;
	};
	template<uint8_t...Pin>
	const std::unordered_map<uint8_t, _PinCommonIsr> _PinIsrMap<std::integer_sequence<uint8_t, Pin...>>::value{ {Pin,{_CSL_Struct14Value(_PinIsr, Pin), _CommonIsr<Pin>}}... };
#endif
#ifdef ARDUINO_ARCH_SAM

#endif
	// 将任意可调用对象作为指定引脚的中断处理方法。输入不支持中断的引脚是未定义行为。
	inline void AttachInterrupt(uint8_t Pin, std::move_only_function<void() const>&& ISR, int Mode)
	{
		const _PinCommonIsr& PCI = _PinIsrMap<typename _Interruptable<std::make_integer_sequence<uint8_t, UINT8_MAX>>::type>::value.at(Pin);
		PCI.PinIsr = std::move(ISR);
		attachInterrupt(digitalPinToInterrupt(Pin), PCI.CommonIsr, Mode);
	}
	// 将任意可调用对象作为指定引脚的中断处理方法
	template <uint8_t Pin>
	inline void AttachInterrupt(std::move_only_function<void() const>&& ISR, int Mode)
	{
		_PinIsr<Pin> = std::move(ISR);
		attachInterrupt(digitalPinToInterrupt(Pin), _CommonIsr<Pin>, Mode);
	}
	// 将任意可调用对象作为指定引脚的中断处理方法
	template <int Mode>
	inline void AttachInterrupt(uint8_t Pin, std::move_only_function<void() const>&& ISR)
	{
		const _PinCommonIsr& PCI = _PinIsrMap<typename _Interruptable<std::make_integer_sequence<uint8_t, UINT8_MAX>>::type>::value.at(Pin);
		PCI.PinIsr = std::move(ISR);
		attachInterrupt(digitalPinToInterrupt(Pin), PCI.CommonIsr, Mode);
	}
	// 将任意可调用对象作为指定引脚的中断处理方法
	template <uint8_t Pin, int Mode>
	inline void AttachInterrupt(std::move_only_function<void() const>&& ISR)
	{
		_PinIsr<Pin> = std::move(ISR);
		attachInterrupt(digitalPinToInterrupt(Pin), _CommonIsr<Pin>, Mode);
	}
	//停止处理指定引脚的中断
	inline void DetachInterrupt(uint8_t Pin)
	{
#ifdef ARDUINO_ARCH_AVR
		_InterruptRegister &= ~_InterruptMask_t::value[digitalPinToInterrupt(Pin)];
#endif
#ifdef ARDUINO_ARCH_SAM
		g_APinDescription[Pin].pPort->PIO_IDR = g_APinDescription[Pin].ulPin;
#endif
	}
	//停止处理指定引脚的中断
	template<uint8_t Pin>
	inline void DetachInterrupt()
	{
#ifdef ARDUINO_ARCH_AVR
		_InterruptRegister &= ~_InterruptMask_t::value[digitalPinToInterrupt(Pin)];
#endif
#ifdef ARDUINO_ARCH_SAM
		Internal::g_APinDescription[Pin].pPort->PIO_IDR = Internal::g_APinDescription[Pin].ulPin;
#endif
	}
}
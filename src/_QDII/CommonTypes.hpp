#pragma once
#include <Cpp_Standard_Library.h>
#include <functional>
#include <Arduino.h>
namespace Quick_digital_IO_interrupt
{
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
		std::move_only_function<void() const> &PinIsr;
		void (*CommonIsr)();
	};
	template <typename T>
	struct _PinIsrMap;
}
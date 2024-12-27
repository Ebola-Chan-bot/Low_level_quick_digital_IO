#pragma once
#include <Arduino.h>
#include <functional>
namespace Low_level_quick_digital_IO
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
#include <Low_level_quick_digital_IO.hpp>
//本示例比较了使用内置函数和本库函数的相同功能。从肉眼上看不出区别，证明本库能够实现和内置函数相同的基本功能。但是在高频读写情况下，使用模板参数执行编译期计算，本库将更胜一筹。

//可以改为任何你连接了可用于报告引脚状态的设备的引脚号
constexpr uint8_t BuiltinPin = 2;
constexpr uint8_t EfficientPin = 3;
using namespace Low_level_quick_digital_IO;
void setup() {
  //可以获知引脚当前是OUTPUT还是INPUT。内置函数无此功能。
  bool IsOutput = PinMode<EfficientPin>();

  //可以按需指定要读取的是输出电平还是输入电平，无论当前状态为何；也可以根据当前状态自动返回对应的电平。内置函数无此功能。
  bool IsHighOut = DigitalRead<EfficientPin, OUTPUT>();
  bool IsHighIn = DigitalRead<EfficientPin, INPUT>();
  bool IsHigh = DigitalRead<EfficientPin>();

  //使用前必须先设置pinMode
  PinMode<BuiltinPin, OUTPUT>();
  pinMode(EfficientPin, OUTPUT);
  //在setup阶段，BuiltinPin和EfficientPin的行为完全相同
  digitalWrite(BuiltinPin, HIGH);
  DigitalWrite<EfficientPin, HIGH>();
  delay(1000);
  digitalWrite(BuiltinPin, LOW);
  DigitalWrite<EfficientPin, LOW>();
}
void loop() {
  //在loop阶段，EfficientPin持续闪烁
  delay(1000);
  //直接反转引脚电平，无需获知当前是高是低。内置函数无此功能。
  DigitalToggle<EfficientPin>();
}
//本库所有参数也支持非模板的运行时参数。性能会稍低，但仍可保证高于内置。
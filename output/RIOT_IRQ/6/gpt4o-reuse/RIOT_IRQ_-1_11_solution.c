
#include "xtimer.h"
#include "thread.h"
#include "irq.h"
#include "periph/timer.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/cpuid.h"
#include "periph/pm.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/wdt.h"
#include "periph/gpio.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/uart.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/flashpage.h"
#include "periph/eeprom.h"
#include "periph/c
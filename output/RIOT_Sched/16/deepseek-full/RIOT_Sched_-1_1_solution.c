
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.h"
#include "periph/i2c.h"
#include "periph/adc.h"
#include "periph/pwm.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtt.h"
#include "periph/watchdog.h"
#include "periph/random.h"
#include "periph/gpio.h"
#include "periph/uart.h"
#include "periph/timer.h"
#include "periph/rtc.h"
#include "periph/spi.
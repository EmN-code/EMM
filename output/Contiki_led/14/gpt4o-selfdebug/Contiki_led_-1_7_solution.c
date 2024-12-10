#include "sys/rtimer.h" 
#include "net/netstack.h" 
#include "contiki-default-conf.h" 
#include "contiki-conf.h"

#include <stdio.h>

// Declare functions
int read_dht11_temperature(); 
void set_rgb_color(int color);

// Define RGB colors
#define RGB_LED_RED   0x1
#define RGB_LED_GREEN 0x2
#define RGB_LED_BLUE  0x4
#define RGB_LED_WHITE (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

// Dummy implementation of reading temperature from DHT11
int read_dht11_temperature() {
    // In real implementation, this would interact with specific registers/hardware
    return 25; // returns a dummy temperature
}

// Dummy implementation of setting an RGB color
void set_rgb_color(int color) {
    // In a real scenario, this function would interact with hardware to change the LED color
    printf("RGB LED set to color code: %d\n", color);
}

int main() {
    while (1) {
      int temperature = read_dht11_temperature();
      printf("Current temperature: %d°C\n", temperature);
      
      if (temperature > 24) { 
          set_rgb_color(RGB_LED_RED); // Example for turning on red light
      } else {
          set_rgb_color(RGB_LED_BLUE); // Example for turning on blue light
      }
      
      rtimer_arch_schedule(rtimer_arch_now() + RTIMER_SECOND * 5); // Wait for 5 seconds
    }
    return 0;
}
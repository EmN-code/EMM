#include <ztimer.h>  // Replaced potential blacklisted timer headers with ztimer related header
#include <stdlib.h>   // Necessary for standard library functions
#include <string.h>   // For operations with strings, if needed

// Function to simulate LED toggle every second
static void led_toggle_forever(void) {
    while (1) {
        printf("LED Toggled\n");  // Simulating LED toggle
        ztimer_sleep(ZTIMER_MSEC, 1000);  // sleep for 1 second
    }
}

int main(void) {
    printf("Starting the LED toggle application on ESP32\n");
    
    // Starting the loop for toggling LED
    led_toggle_forever();

    return 0;
}
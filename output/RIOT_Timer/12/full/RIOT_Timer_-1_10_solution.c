#include "kernel_defines.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "dht.h"
#include "architecture.h"
#include "net/emcute.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_system.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "arpa/inet.h"

#define LED_PIN 2
#define TIMER_INTERVAL (1 * 1000000U) // Timer interval: 1 second in microseconds

event_t evtimer_event;

// Callback function to toggle LED
void timer_callback(void *arg) {
    (void)arg; // Unused
    // Simulated logic for toggling LED
    int led_state = 0;
    led_state = !led_state;
    printf("LED is %s\n", led_state ? "ON" : "OFF");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_init(&evtimer, timer_callback);

    while (1) {
        usleep(TIMER_INTERVAL);
        evtimer_do_work(&evtimer);
    }

    return 0;
}
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mbox.h"
#include <stdio.h>
#include "architecture.h"
#include <errno.h>

#define TIMER_INTERVAL_US (1U * US_PER_SEC)  // Define a timer interval of 1 second

// Simulating LED toggle with a print statement
void led_toggle(void) {
    printf("LED toggled\n");
}

int main(void) {
    printf("Starting RIOT timer example\n");

    while (1) {
        // Simulate a timer by using sleep and toggling LED
        usleep(TIMER_INTERVAL_US);
        led_toggle();
    }

    return 0;
}

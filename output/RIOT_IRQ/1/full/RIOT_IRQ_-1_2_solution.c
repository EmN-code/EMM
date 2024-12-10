#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <string.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <shell.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <nimble_scanner.h>
#include <msg.h>
#include <mutex.h>
#include <esp_wifi.h>
#include <irq.h>
#include <arpa/inet.h>

#define ENABLE 1
#define DISABLE 0

atomic_int irq_flag = 0;

void handle_irq(void) {
    if (irq_flag == DISABLE) {
        irq_flag = ENABLE;
    }
    else {
        irq_flag = DISABLE;
    }
}

int main(void) {
    unsigned state;
    state = irq_disable();
    printf("IRQ Disabled\n");

    // Simulate some atomic operation while IRQs are disabled
    handle_irq();
    if (irq_flag == ENABLE) {
        printf("IRQ handled: Enabled\n");
    }
    else {
        printf("IRQ handled: Disabled\n");
    }

    irq_restore(state);
    printf("IRQ Restored\n");

    return 0;
}
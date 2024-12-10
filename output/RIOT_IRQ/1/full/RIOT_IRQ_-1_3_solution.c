#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <shell.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <msg.h>
#include <mqttclient.h>
#include <mutex.h>
#include <dht_params.h>
#include <esp_wifi.h>
#include <arpa/inet.h>
#include <assert.h>
#include <mma8x5x_regs.h>
#include <nimble_scanner.h>
#include <ztimer.h>
#include <time_units.h>
#include <irq.h>

atomic_bool irq_handled = ATOMIC_VAR_INIT(false);

void irq_handler(void) {
    atomic_store(&irq_handled, true);
}

void disable_interrupts() {
    unsigned state = irq_disable();
    // Perform operations with interrupts disabled.
    if(!atomic_load(&irq_handled)) {
        printf("Interrupt is disabled successfully.\n");
    }
    irq_restore(state);
    if(atomic_load(&irq_handled)) {
        printf("Interrupt was handled while disabled.\n");
    }
    else {
        printf("Interrupt handling failed.\n");
    }
}

int main(void) {
    printf("Program Started\n");
    disable_interrupts();
    printf("Program Finished\n");
    return 0;
}
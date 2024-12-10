#include "thread.h"
#include "stdio.h"
#include "stdbool.h"
#include "paho_mqtt.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "irq_arch.h"
#include "irq.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "stdlib.h"
#include "string.h"

// Function to simulate interrupt handling on an ESP32
void handle_interrupt(void) {
    unsigned state;
    // Disable the interrupts
    state = irq_disable();
    // Simulate some critical section operations
    printf("Handling interrupt, critical operations in progress...\n");
    // Restore the interrupts
    irq_restore(state);
    printf("Interrupts restored.\n");
}

int main(void) {
    printf("Starting the ESP32 Interrupt Handling Example\n");
    handle_interrupt();
    return 0; // Program successfully executed
}

#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <xtimer.h>
#include <thread.h>
#include <mutex.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <mma8x5x_regs.h>
#include <msg.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <net/sock/udp.h>
#include <esp_system.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <ztimer.h>
#include <dht.h>
#include <esp_wifi.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stddef.h>
#include <mbox.h>
#include <net/gcoap.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void my_interrupt_handler(void)
{
    atomic_fetch_add(&counter, 1);
}

int main(void)
{
    unsigned state;

    // Disable interrupts
    state = irq_disable();

    // Set the interrupt handler
    irq_set_exclusive_handler(ESP32_IRQ_NUM, my_interrupt_handler);

    // Enable the interrupt
    irq_enable(ESP32_IRQ_NUM);

    // Restore interrupts
    irq_restore(state);

    // Simulate some work
    xtimer_sleep(5);

    // Check the counter
    printf("Counter value: %d\n", atomic_load(&counter));

    return 0;
}

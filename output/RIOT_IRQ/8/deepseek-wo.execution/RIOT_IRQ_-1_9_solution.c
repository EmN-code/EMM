
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <thread.h>
#include <mutex.h>
#include <msg.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <net/sock/udp.h>
#include <esp_system.h>
#include <time_units.h>
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
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>
#include <net/gcoap.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void increment_counter(void)
{
    atomic_fetch_add(&counter, 1);
}

void interrupt_handler(void *arg)
{
    (void)arg;
    increment_counter();
}

int main(void)
{
    unsigned state = irq_disable();
    irq_set_handler(ESP_INTR_NUM, interrupt_handler, NULL);
    irq_restore(state);

    while (1) {
        printf("Counter value: %d\n", atomic_load(&counter));
        xtimer_sleep(1);
    }

    return 0;
}

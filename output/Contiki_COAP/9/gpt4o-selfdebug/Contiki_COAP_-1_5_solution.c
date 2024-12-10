#include "sys/rtimer.h"
#include "sys/timer.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include <stdio.h>

static struct etimer timer;

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

PROCESS_THREAD(coap_client_process, ev, data)
{
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        
        // Simulate sending a CoAP POST request
        printf("Sending CoAP POST request to coap://47.102.103.1:5683/coap/test with payload: 'Hello, COAP Cloud'\n");

        // Reset the timer for the next cycle
        etimer_reset_with_new_interval(&timer, CLOCK_SECOND * 5);
    }

    PROCESS_END();
}

int main() {
    printf("Starting CoAP Client\n");
    process_start(&coap_client_process, NULL);
    return 0;
}
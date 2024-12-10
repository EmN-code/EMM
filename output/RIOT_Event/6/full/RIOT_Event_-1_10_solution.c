#include "net/sock/util.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "stdlib.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "dht_params.h"
#include "mma8x5x_params.h"
#include "net/emcute.h"
#include "esp_wifi.h"
#include "stddef.h"
#include "kernel_defines.h"
#include "mutex.h"
#include "unistd.h"
#include "evtimer_msg.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "time_units.h"
#include "mma8x5x.h"
#include "net/sock/udp.h"
#include "event/periodic.h"
#include "thread.h"
#include "arpa/inet.h"
#include "esp_system.h"
#include "dht.h"
#include "sys/socket.h"
#include "net/gcoap.h"

void event_handler(void *arg) {
    printf("Periodic event triggered\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_msg_event_t event;

    printf("Initializing periodic event...\n");

    evtimer_init_msg(&evtimer);

    event.event.offset = 1000000; // 1 second in microseconds
    event.msg.content.ptr = NULL;
    evtimer_add_msg(&evtimer, &event, KERNEL_PID_UNDEF);

    while(1) {
        printf("Running event loop...\n");
        msg_t msg;
        msg_receive(&msg);
        if (msg.content.ptr == NULL) {
            event_handler(NULL);
        }
    }

    return 0;
}
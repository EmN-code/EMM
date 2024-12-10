#include <paho_mqtt.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <event/thread.h>
#include <architecture.h>
#include <arpa/inet.h>

#define MQTT_BROKER_IP "192.168.1.100"
#define MQTT_BROKER_PORT 1883

void handle_error(int rc) {
    if (rc != 0) {
        printf("Error: %d\n", rc);
    }
}

int main(void) {
    mbox_t my_mbox;
    msg_t mbox_queue[8];
    evtimer_t my_evtimer;
    evtimer_mbox_event_t my_event;
    Network network;
    int rc;

    mbox_init(&my_mbox, mbox_queue, 8);
    evtimer_init_mbox(&my_evtimer);
    evtimer_add_mbox(&my_evtimer, &my_event, &my_mbox);

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);

        if (rc != 0) {
            printf("Failed to connect to the network. Retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Network connected\n");
            break;
        }
    }

    while (1) {
        msg_t msg;
        if (_mbox_get(&my_mbox, &msg, 1)) {
            printf("Event received\n");
        }
        xtimer_sleep(2);
    }

    return 0;
}

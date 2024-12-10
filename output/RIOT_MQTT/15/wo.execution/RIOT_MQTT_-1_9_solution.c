#include "arpa/inet.h"
#include "dht.h"
#include "dht_params.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "mutex.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"

#include "MQTTClient.h"

#include "ztimer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define STATE_TOPIC "esp32/state"
#define CLIENT_ID "esp32-client"
#define WORK_STATE "work"
#define NOT_WORK_STATE "not work"

static sock_udp_ep_t broker_ep = {
    .family = AF_INET,
    .port = MQTT_BROKER_PORT
};

int main(void) {
    printf("Connecting to MQTT Broker\n");
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_ep.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &broker_ep, NULL);

    // Simulate a simple toggle state and publish every 5 seconds
    char *state_msg = WORK_STATE;
    while (1) {
        printf("Publishing state: %s\n", state_msg);

        // Send the state message via MQTT (hypothetical function)
        if (sock_udp_send(&sock, state_msg, strlen(state_msg), &broker_ep) < 0) {
            printf("Failed to send message\n");
        }

        // Toggle state for next publish
        state_msg = (strcmp(state_msg, WORK_STATE) == 0) ? NOT_WORK_STATE : WORK_STATE;

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    sock_udp_close(&sock);

    return 0;
}

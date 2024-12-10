#include <stdio.h>  // For standard input and output
#include <string.h> // For string operations
#include "net/af.h" // For address family
#include "net/ipv6/addr.h" // For IPv6 address manipulation
#include "net/sock/udp.h" // For UDP socket communication
#include "net/emcute.h" // Provides MQTT capabilities for RIOT

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MSG_LEN 64
#define STATE_INTERVAL_MS (5 * 1000U) // Interval of 5 seconds

typedef struct {
    unsigned long timestamp;
    char state[16];
} device_state_t;

static int _publish_state(sock_udp_ep_t *remote, const char *state) {
    device_state_t msg;
    char buffer[MSG_LEN];

    msg.timestamp = xtimer_now_usec64(); // Usec timestamp
    snprintf(msg.state, sizeof(msg.state), "%s", state);

    /* Convert device_state_t to a buffer for sending */
    snprintf(buffer, MSG_LEN, "%lu:%s", msg.timestamp, msg.state);

    int res = sock_udp_send(NULL, buffer, strlen(buffer), remote);
    return res;
}

int main(void) {
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    printf("MQTT-SN publisher initialized\n");
    sock_udp_ep_t remote = { .family = AF_INET6, .port = BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, BROKER_IP);

    while (1) {
        /* Publish the work status every five seconds */
        if (_publish_state(&remote, "work") < 0) {
            puts("Error publishing work state\n");
        }

        xtimer_sleep(STATE_INTERVAL_MS / 1000);

        if (_publish_state(&remote, "not work") < 0) {
            puts("Error publishing not work state\n");
        }

        xtimer_sleep(STATE_INTERVAL_MS / 1000);
    }
    return 0;
}
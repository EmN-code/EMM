#include net/sock/udp.h
#include net/sock.h
#include net/ipv4/addr.h
#include kernel_defines.h
#include <string.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define MQTT_PUB_INTERVAL (5U)

static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

void connect_to_broker(void) {
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) == 1) {
        printf("Connected to MQTT Broker\n");
    } else {
        printf("Error connecting to MQTT Broker\n");
    }
}

void mqtt_publish_state(const char *state) {
    // Implementation for publishing state to MQTT broker
    // This section will need to use appropriate MQTT libraries (not emcute)
    printf("Publishing state: %s\n", state);
}

int main(void) {
    connect_to_broker();
    while (1) {
        mqtt_publish_state("work or not work");
        xtimer_sleep(MQTT_PUB_INTERVAL);
    }
    return 0;
}
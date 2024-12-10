#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_QOS 0
#define STATE_WORK "work"
#define STATE_NOT_WORK "not work"
#define INTERVAL 5

static emcute_con_t broker_connection;

static void *publish_state(void *arg) {
    (void)arg;
    char *message;
    int counter = 0;

    while (1) {
        message = counter % 2 == 0 ? STATE_WORK : STATE_NOT_WORK;
        emcute_pub_t pub;
        memset(&pub, 0, sizeof(pub));
        pub.topic.name = MQTT_TOPIC;
        pub.data = message;
        pub.data_len = strlen(message);
        pub.qos = MQTT_QOS;

        if (emcute_pub(&pub) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        } else {
            printf("Published: %s\n", message);
        }

        counter++;
        sleep(INTERVAL);
    }
    return NULL;
}

static int mqtt_connect(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP);
    return emcute_con(&broker_connection, &gw, true, "esp32");
}

int main(void) {
    if (mqtt_connect() != EMCUTE_OK) {
        printf("error: unable to connect to the broker\n");
        return -1;
    }

    puts("Connected to broker, starting publish loop.");
    publish_state(NULL);

    return 0;
}
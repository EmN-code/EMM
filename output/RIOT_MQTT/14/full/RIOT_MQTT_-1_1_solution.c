#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqtt.h>
#include <net/emcute.h>  // we are not using emcute but including for network management if necessary.
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>

#define MQTT_TOPIC "esp32/status"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // 5 seconds interval

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void on_pub(struct mqtt_client *client, void **arg, unsigned int msg_id) {
    (void)client;
    (void)arg;
    printf("Published msg id: %d\n", msg_id);
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    Network network;
    struct mqtt_client mqtt_client;

    unsigned char sendbuf[512], readbuf[512];
    mqtt_client_init(&mqtt_client, &network, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    printf("Connecting to WiFi\n");

    // Network Setup
    while (1) {
        int res = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (res != 0) {
            printf("Error connecting to network. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Connected to network\n");
        break;
    }

    // Connect to the MQTT broker
    printf("Connecting to MQTT broker\n");
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "riot_esp32";

    while (1) {
        int rc = MQTTConnect(&mqtt_client, &connectData);
        if (rc != 0) {
            printf("Error connecting to MQTT broker. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Connected to MQTT broker\n");
        break;
    }

    char *payload = "work";
    while (1) {
        mqtt_message_t message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)payload;
        message.payloadlen = strlen(payload);

        printf("Publishing to MQTT broker\n");
        while (MQTTPublish(&mqtt_client, MQTT_TOPIC, &message) != 0) {
            printf("Error publishing. Retrying...\n");
            xtimer_sleep(1);
        }
        printf("Published: %s\n", payload);

        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}
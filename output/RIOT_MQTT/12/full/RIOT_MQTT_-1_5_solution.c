#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <architecture.h>
#include <kernel_defines.h>
#include <thread.h>
#include <event/thread.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <timex.h>
#include <mbox.h>
#include <mutex.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <net/mqttsn.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/af.h>
#include <net/gcoap.h>
#include <net/ipv6/addr.h>
#include <sys/socket.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"

void publish_state(MQTTClient *client) {
    MQTTMessage message;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);
    message.qos = 1;
    message.retained = 0;

    int rc;
    // Attempt to publish until successful
    while (1) {
        rc = MQTTPublish(client, MQTT_TOPIC, &message);
        if (rc == 0) {
            printf("MQTT message published successfully\n");
            break;
        } else {
            printf("MQTT publish failed, retrying...\n");
            xtimer_sleep(1); // Sleep for 1 second before retrying
        }
    }
}

int main(void) {
    Network network;
    NetworkInit(&network);

    int rc;
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            xtimer_sleep(1); // Sleep for 1 second before retrying
            continue;
        } else {
            printf("Network connected successfully\n");
            break;
        }
    }

    unsigned char sendbuffer[80], readbuffer[80];
    MQTTClient client;
    MQTTClientInit(&client, &network, 1000, sendbuffer, sizeof(sendbuffer), readbuffer, sizeof(readbuffer));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;

    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed, retrying...\n");
            xtimer_sleep(1); // Sleep for 1 second before retrying
            continue;
        } else {
            printf("MQTT connected successfully\n");
            break;
        }
    }

    while (1) {
        publish_state(&client);
        xtimer_sleep(5); // Sleep for 5 seconds before publishing again
    }

    return 0;
}
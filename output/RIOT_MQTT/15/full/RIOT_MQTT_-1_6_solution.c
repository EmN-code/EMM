#include <xtimer.h>
#include <stddef.h>
#include <event/thread.h>
#include <assert.h>
#include <freertos/FreeRTOS.h>
#include <kernel_defines.h>
#include <msg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <architecture.h>
#include <mutex.h>
#include <net/emcute.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_INTERVAL (5U * 1000000U)  // 5 seconds

static char sendbuf[128];
static char readbuf[128];
static MQTTClient client;
static Network network;

void publish_state(void) {
    MQTTMessage message;
    char *state = "work or not work";
    int rc;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*) state;
    message.payloadlen = strlen(state);

    while (1) {
        rc = MQTTPublish(&client, "esp32/status", &message);
        if (rc != 0) {
            printf("Failed to publish message, reconnecting...\n");
            while (MQTTConnect(&client, NULL) != 0) {
                xtimer_sleep(1);
            }
            continue;
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    int rc;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    // Initialize network
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        break;
    }

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        break;
    }

    // Start MQTT task handler
    MQTTStartTask(&client);

    // Start publishing state
    publish_state();

    return 0;
}
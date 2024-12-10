#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <kernel_defines.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <sys/socket.h>

#define EMCUTE_PORT        (1883U)
#define EMCUTE_ID_MAX_LEN  (32U)
#define TOPIC_MAX_LEN      (64U)
#define MQTT_BROKER_IP     "47.102.103.1"
#define SLEEP_DURATION     (5U)

static char mqtt_client_id[EMCUTE_ID_MAX_LEN] = "riot-esp32";

int main(void) {
    emcute_topic_t topic;
    topic.name = "esp32/state";

    sock_udp_ep_t gw = { .port = EMCUTE_PORT };

    // Convert the IP string to a network IP representation
    if (!inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4)) {
        printf("Error: Invalid MQTT broker IP\n");
        return -1;
    }

    // Initialize the emcute client here and add logic for connection retries
    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res != EMCUTE_OK) {
            printf("Error: Unable to connect to the MQTT-SN broker. Retrying...\n");
            continue;
        } else {
            printf("Connected to MQTT-SN broker successfully\n");
            break;
        }
    }

    // Register the topic
    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: Unable to register the topic\n");
        return -1;
    }

    while (1) {
        char message[] = "Work"; // Toggle logic can be added here to publish different states
        res = emcute_pub(&topic, message, sizeof(message), EMCUTE_QOS_0);

        if (res == EMCUTE_OK) {
            printf("Published data: %s\n", message);
        } else {
            printf("Error: Failed to publish data\n");
        }

        // Wait for 5 seconds
        sleep(SLEEP_DURATION);
    }

    return 0;
}
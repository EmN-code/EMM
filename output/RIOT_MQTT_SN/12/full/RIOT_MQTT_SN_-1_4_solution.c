#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "dht_params.h"
#include "thread.h"

#define EMCUTE_PORT     (1883U)
#define MQTT_BROKER_IP  "47.102.103.1"
#define PUB_TOPIC_NAME  "device/state"
#define PUB_INTERVAL    (5U)  // Publish interval in seconds

static emcute_topic_t topic;
static sock_udp_ep_t remote;

void publish_state(void) {
    static const char *messages[] = {"work", "not work"};
    static int msg_index = 0;
    int res;

    while (1) {
        res = emcute_reg(&topic);
        if (res != EMCUTE_OK) {
            printf("Error registering topic [Error Code: %d]\n", res);
            continue;
        }

        const char *pub_data = messages[msg_index];
        res = emcute_pub(&topic, pub_data, strlen(pub_data), EMCUTE_QOS_0);

        if (res != EMCUTE_OK) {
            printf("Error publishing data [Error Code: %d]\n", res);
        } else {
            printf("Published: %s\n", pub_data);
        }

        msg_index = (msg_index + 1) % 2;
        sleep(PUB_INTERVAL);
    }
}

int main(void) {
    // Initializing the remote parameters
    remote.family = AF_INET;
    remote.port = EMCUTE_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    int res;

    while (1) {
        res = emcute_con(&remote, true, NULL, NULL, 0, 0);
        if (res != EMCUTE_OK) {
            printf("Error connecting to MQTT-SN broker [Error Code: %d]\n", res);
            sleep(1);  // Wait before retrying
            continue;
        }
        printf("Connected to broker\n");
        break;
    }

    // Setup the topic for publishing
    topic.name = PUB_TOPIC_NAME;

    publish_state();  // Start publishing the state

    return 0;
}
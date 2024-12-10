#include "net/asymcute.h"
#include "net/emcute.h"
#include "timex.h"
#include "thread.h"

#define CLIENT_ID "mqttsn_client"
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC "device/state"
#define PUB_INTERVAL (5U)

static emcute_topic_t topic;

// Callback for published messages
void on_pub(const emcute_topic_t *topic, void *data, size_t len){
    (void)topic; // Unused
    printf("Published: %.*s\n", len, (char *)data);
}

int main(void) {
    // Initialize and configure the MQTT-SN client
    emcute_conf_t emcute_conf = {
        .id = CLIENT_ID,
        .try_reconnect = false,
    };
    emcute_init(&emcute_conf);

    // Define the broker address and port
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = BROKER_PORT,
    };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    // Try to connect to the broker
    if (emcute_con(&gw, true) != EMCUTE_OK) {
        printf("Error: unable to connect to broker\n");
        return 1;
    }
    printf("Connected to broker %s\n", BROKER_IP);

    // Initialize topic
    topic.name = PUB_TOPIC;

    // Main loop
    while (1) {
        char *message;
        if (/* condition to check if working */) {
            message = "work";
        } else {
            message = "not work";
        }
        emcute_publish(&topic, message, strlen(message), EMCUTE_QOS_0);
        on_pub(&topic, message, strlen(message));
        thread_sleep(PUB_INTERVAL * US_PER_SEC);
    }

    return 0;
}
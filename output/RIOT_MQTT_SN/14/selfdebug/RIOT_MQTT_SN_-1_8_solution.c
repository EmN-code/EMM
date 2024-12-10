#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <emcute.h>
#include <ztimer.h>

#define SERVER_ADDR "47.102.103.1"  // MQTT-SN broker address
#define SERVER_PORT 1883            // MQTT-SN broker port
#define PUB_TOPIC_NAME "device/state"

// Define MQTT-SN connection structures and variables
time_t last_pub_time = 0;
emcute_sub_t subscription;

void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;
    printf("Received Publication from topic '%s': %s\n", topic->name, in);
}

int main(void)
{
    // Create endpoint for the MQTT-SN broker
    sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };

    // Convert the server address from text to binary
    if (inet_pton(AF_INET6, SERVER_ADDR, &gw.addr) != 1) {
        printf("Error: invalid address\n");
        return 1;
    }

    // Connect to the broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", SERVER_ADDR, SERVER_PORT);
        return 1;
    }

    // Continuously publish the state every five seconds
    while (1) {
        char *pub_data = "work";

        // Prepare the topic to publish
        emcute_topic_t t;
        t.name = PUB_TOPIC_NAME;

        // Register the topic
        if (emcute_reg(&t) != EMCUTE_OK) {
            printf("Error: unable to register topic\n");
            emcute_discon();
            return 1;
        }

        // Publish the data
        if (emcute_pub(&t, pub_data, strlen(pub_data), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
            emcute_discon();
            return 1;
        }

        printf("Published '%s' to topic '%s'\n", pub_data, PUB_TOPIC_NAME);

        // Delay for 5 seconds
        ztimer_sleep(ztimer_now() + ZTIMER_SEC(5));
    }

    // Disconnect before exiting
    emcute_discon();
    return 0;
}
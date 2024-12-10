#include <sys/socket.h>   // Required for socket operations
#include <netinet/in.h>    // Standard Internet protocols
#include <net/sock/udp.h>  // UDP network operations
#include <net/sock/util.h> // Utility functions for sockets
#include <net/gcoap.h>     // CoAP support
#include <net/emcute.h>    // For emcute functions
#include <mma8x5x_regs.h>  // Sensor registry operations
#include <event/thread.h>  // Thread events
#include <thread.h>        // For threading support

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC_NAME "state"
#define PUB_MSG "working"

static emcute_topic_t topic;

static int _on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic; (void)data; (void)len;
    return EMCUTE_OK;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_BROKER_PORT };

    while (1) {
        // Connect to MQTT-SN gateway
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if(res != EMCUTE_OK) {
            printf("Failed to connect to gateway. Retrying in 1 second...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        printf("Connected to gateway\n");
        break;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    while (1) {
        if(emcute_reg(&topic) != EMCUTE_OK) {
            printf("Failed to register topic. Retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        printf("Registered topic: %s\n", topic.name);
        break;
    }

    char message[] = PUB_MSG;

    while (1) {
        // Publish the message
        int res = emcute_pub(&topic, message, sizeof(message), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Failed to publish message. Retrying in 1 second...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        } else {
            printf("Published message: %s\n", message);
            ztimer_sleep(ZTIMER_SEC, 5); // Delay for 5 seconds
        }
    }
    
    // Run emcute (blocking call)
    emcute_run(MQTT_BROKER_PORT, "mqtt_client");

    return 0;
}
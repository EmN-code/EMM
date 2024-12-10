#include "net/sock/util.h"
#include "thread.h"
#include "netinet/in.h"
#include "mma8x5x.h"
#include "net/emcute.h"
#include "mma8x5x_params.h"
#include "event/thread.h"
#include "net/sock/udp.h"
#include "mma8x5x_regs.h"
#include "arpa/inet.h"
#include "net/gcoap.h"

#define EMCUTE_PORT     1883
#define MQTT_TOPIC      "state"
#define MQTT_MESSAGE_ON "work"
#define MQTT_MESSAGE_OFF "not work"
#define BROKER_IP       "47.102.103.1"
#define STACK_SIZE      THREAD_STACKSIZE_MAIN
#define SLEEP_INTERVAL  (5U * 1000000U) // Microseconds

static char stack[STACK_SIZE];
static msg_t queue[8];

static void *mqtt_thread(void *arg) {
    (void)arg;
    emcute_topic_t topic;

    // Configure the broker endpoint
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    // Connect to the broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        // Connection failed
        return NULL;
    }

    // Register the topic
    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        // Topic registration failed
        return NULL;
    }

    while (1) {
        // Publish "work" state
        if (emcute_pub(&topic, MQTT_MESSAGE_ON, strlen(MQTT_MESSAGE_ON), 0) != EMCUTE_OK) {
            // Publish failed
        }
        
        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return NULL;
}

int main(void) {
    // Start the MQTT-SN thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");
    
    // Run the main loop
    while (1) {
        // Perform any other main operations here
        xtimer_sleep(1);
    }

    return 0;
}

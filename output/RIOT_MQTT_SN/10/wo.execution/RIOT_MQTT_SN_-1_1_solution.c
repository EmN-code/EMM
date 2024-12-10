#include "thread.h"
#include "periph/gpio.h"
#include "emcute.h"
#include "xtimer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC) // 5 seconds
#define MQTT_TOPIC "state"

typedef struct {
    const char *name;           
    uint16_t id;               
} emcute_topic_t;

static emcute_topic_t topic = {
    .name = MQTT_TOPIC,
    .id = 0
};

static void *publisher(void *arg) {
    (void)arg;
    char *message = "work";
    while (1) {
        int res = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        } else {
            printf("Published: %s\n", message);
        }
        // Wait for the next publication interval
        xtimer_sleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    emcute_endpoint_t - ep;

    printf("Initializing MQTT-SN Client\n");

    // Connect to the broker
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);
    if (emcute_con(&gw) != EMCUTE_OK) {
        puts("Error: unable to connect to the broker");
        return 1;
    }
    
    puts("Connected to the broker\n");

    // Create publisher thread
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher, NULL, "publisher");

    // Enter idle loop
    while (1) {
        thread_sleep();
    }
    return 0;
}
#include <stdio.h>
#include <stddef.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <event/thread.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <dht_params.h>

#define EMCUTE_PORT         1883
#define EMCUTE_ID           "gertrud"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_BUFSIZE      128
#define TOPIC_MAXLEN        64

static char stack[THREAD_STACKSIZE_DEFAULT];
static char mqtt_client_stack[THREAD_STACKSIZE_DEFAULT];
static char recv_buf[EMCUTE_BUFSIZE];
static emcute_topic_t topics[TOPIC_MAXLEN];
static sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT };

// Mutex to protect shared resources
static mutex_t lock = MUTEX_INIT;

void *mqtt_thread(void *arg) {
    (void)arg;

    /* initialize the topic we want to subscribe/publish to */
    topics[0].name = "state";

    /* setup the remote address */
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    // Connect to the broker
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to the broker\n");
        return NULL;
    }

    // Register the topic
    if (emcute_reg(&topics[0]) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
    }

    // Main loop to publish messages
    while (1) {
        // Locking shared resource
        mutex_lock(&lock);

        // Publish a message
        const char *data = "work";
        if (emcute_pub(&topics[0], data, strlen(data), 0) != EMCUTE_OK) {
            puts("error: unable to publish data\n");
        }

        // Unlocking shared resource
        mutex_unlock(&lock);

        // Wait for 5 seconds using ztimer_sleep
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    // Disconnect
    emcute_discon();
    return NULL;
}

int main(void) {
    // Create a thread to handle MQTT connection and messaging
    thread_create(mqtt_client_stack, sizeof(mqtt_client_stack), 
                  EMCUTE_PRIO, THREAD_CREATE_STACKTEST, 
                  mqtt_thread, NULL, "mqtt_client");

    return 0;
}
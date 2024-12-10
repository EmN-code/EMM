#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "esp_system.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "net/sock/util.h"
#include "paho_mqtt.h"
#include "mutex.h"
#include "net/sock/udp.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "dht_params.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "sys/socket.h"
#include "thread.h"

#define EMCUTE_PRIO          (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS            (16U)
#define TOPIC_MAXLEN         (64U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];
static size_t num_subscriptions = 0;

// Function to connect to the MQTT-SN broker
static int connect_mqtt_sn(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };

    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) != 1) {
        puts("error: unable to parse IPv4 address");
        return 1;
    }

    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res != EMCUTE_OK) {
          printf("Error: unable to connect to gateway [%d]. Retrying...\n", res);
          sleep(1);
          continue;
        }
        printf("Successfully connected to gateway at %s\n", "47.102.103.1");
        break;
    }

    return 0;
}

// Function to publish to the topic
static int pub_state(const char* state) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = "state";

    while (1) {
        if (emcute_reg(&t) != EMCUTE_OK) {
          puts("Error: unable to obtain a topic ID");
          sleep(1);
          continue;
        }
        break;
    }

    while (1) {
        if (emcute_pub(&t, state, strlen(state), flags) != EMCUTE_OK) {
          printf("Error: unable to publish data\n");
          sleep(1);
          continue;
        }
        printf("Published %s on topic %s\n", state, t.name);
        break;
    }

    return 0;
}

// Main application thread
static void* mqtt_thread(void* arg) {
    (void)arg;

    // Start the emCute thread
    emcute_run(MQTT_PORT, "riot-client");

    return NULL;
}

// Main function
int main(void) {
    // Initialize MQTT
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  mqtt_thread, NULL, "mqtt_thread");
    
    // Connect to MQTT-SN broker
    if (connect_mqtt_sn() != 0) {
        return 1;
    }

    const char* state_work = "work";
    const char* state_not_work = "not work";

    // Publish the state every 5 seconds
    while (1) {
        pub_state(state_work);
        sleep(5);
        pub_state(state_not_work);
        sleep(5);
    }

    return 0;
}
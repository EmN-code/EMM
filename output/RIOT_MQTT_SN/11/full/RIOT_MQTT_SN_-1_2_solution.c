#include "net/sock/udp.h"
#include "stdlib.h"
#include "net/emcute.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "thread.h"
#include "stdio.h"

define EMCUTE_PORT          (1883U)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)
#define MQTT_BROKER_IP      "47.102.103.1"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static MQTTClient client;
static Network network;
static unsigned char sendbuf[80], readbuf[80];

static char *topics[NUMOFSUBS];
static emcute_sub_t subscriptions[NUMOFSUBS];

static int publish_state(const char *state) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    
    /* Step 1: Prepare a topic */
    t.name = "device/state";
    /* Step 2: Publish the data to the server */
    if (emcute_pub(&t, state, strlen(state), flags) != EMCUTE_OK) {
        puts("error: unable to publish data");
        return 1;
    }

    puts("success: data published");
    return 0;
}

static void *mqtt_thread(void *arg) {
    (void)arg; /* unused */

    while (1) {
        /* Publish "work" state */
        if (publish_state("work") != 0) {
            break;
        }
        /* Sleep for 5 seconds */
        thread_sleep(5);

        /* Publish "not work" state */
        if (publish_state("not work") != 0) {
            break;
        }
        /* Sleep for 5 seconds */
        thread_sleep(5);
    }

    return NULL;
}

int main(void) {
    /* Step 1: Start the networking interface */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  emcute_thread, NULL, "emcute");

    /* Step 2: Connect to the MQTT-SN broker */
    while (1) {
        int rc = NetworkConnect(&network, MQTT_BROKER_IP, EMCUTE_PORT);
        if (rc != 0) {
            printf("connect failed with return code %d\n", rc);
            thread_sleep(1);
            continue;
        } else {
            printf("connected to MQTT broker\n");
            break;
        }
    }

    /* Step 3: Start the MQTT client */
    MQTTClientInit(&client, &network, 3000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    /* Step 4: Create a thread to publish the state to the MQTT-SN broker */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_thread, NULL, "mqtt");

    return 0;
}
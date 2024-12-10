#include <msg.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <stdio.h>
#include <string.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t t;

// Function to publish MQTT-SN messages
void *mqtt_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));

    // Define the broker and topic
    sock_udp_ep_t gw = {.family = AF_INET6, .port = 1883};
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "::ffff:47.102.103.1");

    char *topic_name = "device/state";
    t.name = topic_name;
    t.id = 0;

    // Loop to initialize connection to broker
    while (1) {
        int con = emcute_con(&gw, true,"");
        if (con != EMCUTE_OK) {
            printf("Connection to broker failed\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Connected to broker\n");
        break;
    }

    // Register the topic
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("Unable to register topic\n");
        return NULL;
    }

    // Publish state every 5 seconds
    while (1) {
        const char data[] = "work or not work";
        int pub = emcute_pub(&t, data, strlen(data), EMCUTE_QOS_0);
        if (pub != EMCUTE_OK) {
            printf("Publish failed\n");
        } else {
            printf("Publish success: %s\n", data);
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    puts("RIOT MQTT-SN example\n");
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, mqtt_thread, NULL, "mqtt-thread");
    return 0;
}
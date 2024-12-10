#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[4];
static emcute_sub_t subscriptions[NUMOFSUBS];

static void *_emcute_thread(void *arg) {
    (void)arg;
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, "RIOT_client");
    return NULL; /* should never be reached */
}

static int connect_to_broker(char *address, uint16_t port) {
    sock_udp_ep_t gw = { .port = port }; // Set the gateway port

    /* parse the broker address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, address) == NULL) {
        printf("Error: unable to parse broker address\n");
        return 1;
    }

    if (emcute_con(&gw, false, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%u\n", address, port);
        return 1;
    }

    printf("Connected to broker at [%s]:%u\n", address, port);
    return 0;
}

int main(void) {
    /* set a thread to run emcute in the background */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  _emcute_thread, NULL, "emcute");
    
    /* initialize our subscription buffers */
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));

    /* connect to the MQTT-SN broker */
    if (connect_to_broker("47.102.103.1", 1883) != 0) {
        printf("Failed to connect\n");
        return 1;
    }

    /* publish state every 5 seconds */
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    const char *topic_name = "state";
    const char *state_msg = "work";

    /* get topic ID by registering the topic name */
    t.name = topic_name;
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("Error: unable to obtain topic ID\n");
        return 1;
    }

    while (1) {
        if (emcute_pub(&t, state_msg, strlen(state_msg), flags) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        else {
            printf("Published: %s\n", state_msg);
        }
        
        /* wait for 5 seconds */
        xtimer_sleep(5);
    }

    return 0;
}
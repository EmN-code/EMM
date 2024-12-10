#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <stddef.h>
#include <stdio.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "work_status"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(SERVER_PORT, "mqtt_client_id");
    return NULL;
}

int main(void) {
    /* Initialize the EMCUTE stack */
    emcute_init();
    
    /* Set up thread for EMCUTE to run */
    kernel_pid_t emcute_thread_pid =
        thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                      emcute_thread, NULL, "emcute");

    /* Prepare the server endpoint */
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = SERVER_PORT
    };
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4);
    
    /* Connect to the MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to broker\n");
        return 1;
    }
    printf("Connected to broker\n");

    /* Register topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return 1;
    }
    printf("Topic registered\n");

    /* Publish loop */
    while (1) {
        const char *state = "work"; // your given state
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Failed to publish data\n");
        } else {
            printf("Published data: %s\n", state);
        }
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    /* Disconnect after use */
    emcute_discon();
    return 0;
}
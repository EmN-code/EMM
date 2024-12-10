
#include <unistd.h>  // For usleep
#include <net/emcute.h>
#include <msg.h>
#include <thread.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 8

static msg_t msg_queue[MSG_QUEUE_SIZE];

void publish_state(void) {
    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0;  // Will be populated by emcute_reg

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return;
    }

    const char *state = "work";  // or "not work" based on your logic
    res = emcute_pub(&topic, state, strlen(state), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing state: %d\n", res);
    }
}

void *emcute_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = { .port = EMQX_SERVER_PORT };
    int res = inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);
    if (res != 1) {
        printf("Error parsing IP address\n");
        return NULL;
    }

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return NULL;
    }

    while (1) {
        publish_state();
        usleep(5000000);  // Sleep for 5 seconds
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}

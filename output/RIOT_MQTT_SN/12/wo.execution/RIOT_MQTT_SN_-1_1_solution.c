#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "net/emcute.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define PUBLISH_INTERVAL (5UL * US_PER_SEC)

static emcute_socket_t sock;
static emcute_topic_t topic;

void *publish_thread(void *arg) {
    (void)arg;
    char *state = "work"; // Initial state
    
    while (1) {
        printf("Publishing state: %s\n", state);
        emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
        state = (strcmp(state, "work") == 0) ? "not work" : "work"; // Toggle state
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    size_t len;
    sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER_IP);

    /* initialize emcute socket */
    memset(&sock, 0, sizeof(sock));
    sock.data = NULL;
    if (emcute_sock(&sock, EMCUTE_PORT, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to initialize emcute socket");
        return 1;
    }

    /* configure the topic we want to publish to */
    memset(&topic, 0, sizeof(topic));
    topic.name = MQTT_TOPIC;
    topic.id = 0;

    /* start MQTT-SN connection */
    if (emcute_con(&gw, true, "pub-dev") != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    puts("Connected to MQTT-SN broker");
    /* create publishing thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, publish_thread, NULL, "publish thread");

    return 0;
}
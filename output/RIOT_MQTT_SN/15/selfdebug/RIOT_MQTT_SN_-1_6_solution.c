#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/inet.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "device/state"

static emcute_topic_t topic;
static sock_udp_ep_t server = {
    .family = AF_INET,
    .port = SERVER_PORT
};

void *mqtt_thread(void *arg) {
    (void)arg;
    char *data_working = "work";
    char *data_not_working = "not work";
    unsigned flags = EMCUTE_QOS_0;

    while (1) {
        int res = inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);
        if (res <= 0) {
            printf("Error in converting IP address\n");
            continue;
        }
        emcute_con(&server, true, NULL, NULL, 0, flags);
        emcute_reg(&topic);

        // Random condition to publish "work" or "not work"
        if (/* some condition */) {
            emcute_pub(&topic, data_working, strlen(data_working), flags);
        } else {
            emcute_pub(&topic, data_not_working, strlen(data_not_working), flags);
        }

        emcute_discon();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(_msg_queue, (sizeof(_msg_queue) / sizeof(_msg_queue[0])));

    topic.name = TOPIC_NAME;
    topic.id = 0;
    
    thread_create(_mqtt_thread_stack, sizeof(_mqtt_thread_stack), MQTT_THREAD_PRIORITY, 0, mqtt_thread, NULL, "mqtt");

    return 0;
}

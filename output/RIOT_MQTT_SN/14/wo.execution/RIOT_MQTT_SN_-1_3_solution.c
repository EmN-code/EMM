#include "thread.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "event/thread.h"
#include "net/gnrc.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC "device/state"
#define PUB_INTERVAL (5U)
#define MQTT_CLIENT_ID "esp32-client"

static char stack[THREAD_STACKSIZE_MAIN];

void* mqtt_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t broker = {
        .family = AF_INET6,
        .netif = SOCK_ADDR_ANY_NETIF,
        .port = BROKER_PORT
    };

    inet_pton(AF_INET6, BROKER_IP, &broker.addr.ipv6);
    emcute_con_t connection = {
        .state = EMCUTE_CON_STATE_DISCONNECTED
    };

    emcute_run(BROKER_PORT, MQTT_CLIENT_ID);
    
    while (1) {
        connection.state = EMCUTE_CON_STATE_CONNECTED;
        printf("MQTT-SN client connected\n");

        emcute_topic_t t;
        t.name = PUB_TOPIC;

        const char *data = "work";
        size_t len = strlen(data);

        printf("Publishing data: %s\n", data);
        int res = emcute_pub(&t, data, len, EMCT_PUBLISH_FLAG_NONE);

        if (res != EMCUTE_OK) {
            printf("Error publishing data\n");
        }

        thread_sleep(PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  mqtt_thread, NULL, "MQTT thread");
    while(1) {};
    return 0;
}
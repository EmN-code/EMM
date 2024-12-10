#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <string.h>
#include <xtimer.h>

#define SERVER_ADDR         "47.102.103.1"
#define SERVER_PORT         1883
#define PUB_INTERVAL        (5U * US_PER_SEC)
#define CLIENT_ID           "riot_client"
#define TOPIC_NAME          "state"
#define TOPIC_MSG           "work"
#define BUFFER_SIZE         (64U)

static emcute_topic_t topic;
static char stack_buffer[THREAD_STACKSIZE_MAIN];
static char pub_buffer[BUFFER_SIZE];

void on_publish(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;  
    (void)data;   
    (void)len;    
}

static int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4);

    return emcute_con(&gw, true, NULL, NULL, 0, 0);
}

int main(void) {
    emcute_topic_t t;
    strncpy(t.name, TOPIC_NAME, sizeof(t.name));
    t.id = 0;  // set to some value or use emcute_reg()

    while (1) {
        if (emcute_pub(&t, TOPIC_MSG, strlen(TOPIC_MSG), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Publish failed\n");
        }
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}
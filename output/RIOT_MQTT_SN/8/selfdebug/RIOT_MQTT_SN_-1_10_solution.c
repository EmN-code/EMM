#include <ztimer.h>
#include <net/mqttsn.h>
#include <net/af.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL (5U * 1000U)  // Interval in milliseconds

static int publish_state(emcute_topic_t *topic) {
    const char *state_msg = "work";
    unsigned flags = EMCUTE_QOS_0;
    
    return emcute_pub(topic, state_msg, strlen(state_msg), flags);
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    emcute_topic_t topic;
    
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) != 1) {
        printf("IP address conversion failed\n");
        return 1;
    }
    
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Unable to connect to gateway at %s\n", SERVER_IP);
        return 1;
    }
    
    topic.name = "esp32/status";
    
    printf("MQTT-SN client initialized and connected\n");
    
    while (1) {
        int res = publish_state(&topic);
        if (res != EMCUTE_OK) {
            printf("Publishing failed with error code %d\n", res);
        } else {
            printf("Message published\n");
        }
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);
    }
    return 0;
}
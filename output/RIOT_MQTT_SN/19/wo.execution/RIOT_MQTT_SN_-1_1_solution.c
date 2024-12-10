#include "xtimer.h"
#include "net/emcute.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "device/status"
#define PUB_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;
static char pub_data_is_working[] = "work";
static char pub_data_not_working[] = "not work";

int main(void) {
    // Initialize the MQTT-SN client
    emcute_topic_t topic;
    emcute_pkt_t pkt;
    emcute_sub_t sub;

    // Setup connection parameters
    sockaddr_in_t gw = { .sin_family = AF_INET,
                         .sin_port = htons(BROKER_PORT) };
    inet_pton(AF_INET, BROKER_IP, &gw.sin_addr);

    // Initialize the network configuration
    net_if_schedule(NO_NETID);
    emcute_init(NETIF, EMCUTE_DEFAULT_TTL, EMCUTE_DEFAULT_PORT);

    while (1) {
        // Publish "work" status
        pkt.data = pub_data_is_working;
        pkt.data_len = sizeof(pub_data_is_working);
        pkt.resp.wait_time = 0;
       
        // Use emcute-lib to publish
        int result = emcute_pub(&topic, &pub_data_is_working, strlen(pub_data_is_working), EMCUTE_QOS_1);

        if(result != EMCUTE_OK) {
            printf("Failed to publish data\n");
        }
        
        // Sleep for the interval
        xtimer_usleep(PUB_INTERVAL);

        // Publish "not work" status
        pkt.data = pub_data_not_working;
        pkt.data_len = sizeof(pub_data_not_working);

        // Use emcute-lib to publish
        result = emcute_pub(&topic, &pub_data_not_working, strlen(pub_data_not_working), EMCUTE_QOS_1);

        if(result != EMCUTE_OK) {
            printf("Failed to publish data\n");
        }

        // Sleep for the interval
        xtimer_usleep(PUB_INTERVAL);
    }

    return 0;
}


#include <net/ipv6/addr.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <xtimer.h>
#include <esp_system.h>
#include <mma8x5x_params.h>
#include <mutex.h>
#include <stdio.h>
#include <dht_params.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <dht.h>
#include <esp_wifi.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_CONTENT "work"
#define MSG_CONTENT_LEN 4

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = EMQX_SERVER_PORT;

    // Convert IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: invalid IP address");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN gateway");
        return 1;
    }

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;

    // Register the topic
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        return 1;
    }

    while (1) {
        // Publish the message
        if (emcute_pub(&topic, MSG_CONTENT, MSG_CONTENT_LEN, 0) != EMCUTE_OK) {
            puts("Error: could not publish message");
        } else {
            puts("Message published successfully");
        }

        // Wait for 5 seconds before publishing again
        xtimer_sleep(5);
    }

    return 0;
}

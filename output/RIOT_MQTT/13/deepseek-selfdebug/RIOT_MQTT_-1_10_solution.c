
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/af.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define PUBLISH_INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void *mqtt_thread(void *arg)
{
    (void)arg;

    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    char addr_str[IPV4_ADDR_MAX_STR_LEN];
    ipv4_addr_t broker_addr;

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;

    // Convert the broker IP address from string to binary form
    if (ipv4_addr_from_str(&broker_addr, MQTT_BROKER_IP) == NULL) {
        puts("Error: invalid broker IP address");
        return NULL;
    }
    remote.addr.ipv4 = broker_addr;

    // Create a UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: failed to create UDP socket");
        return NULL;
    }

    char message[64];
    while (1) {
        // Prepare the message to be sent
        snprintf(message, sizeof(message), "{\"state\": \"%s\"}", MQTT_MESSAGE);

        // Send the message to the MQTT broker
        if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
            puts("Error: failed to send message");
        } else {
            printf("Message sent: %s\n", message);
        }

        // Wait for 5 seconds before sending the next message
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Close the socket (this will never be reached in this example)
    sock_udp_close(&sock);

    return NULL;
}

int main(void)
{
    puts("MQTT Publisher Example");

    // Create and start the MQTT thread
    thread_create(NULL, MQTT_THREAD_STACKSIZE, MQTT_THREAD_PRIORITY,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}

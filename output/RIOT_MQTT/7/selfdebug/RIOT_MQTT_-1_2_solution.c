#include <string.h>
#include <stdio.h>
#include <thread.h>
#include <ztimer.h>
#include <net/sock/udp.h>
#include <net/mqttsn/mqttsn.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define SLEEP_INTERVAL (5U) // 5 seconds

static kernel_pid_t main_pid;

// Thread stack definition
#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
char thread_stack[THREAD_STACKSIZE];

// Thread function to publish the MQTT message
static void *mqtt_pub_thread(void *arg) {
    (void)arg;
    mqttsn_t client;
    mqttsn_pkt_id_t pkt_id;

    // Initialize MQTT-SN client.
    memset(&client, 0, sizeof(client));

    // Define the connection parameters
    sock_udp_ep_t gw_ep = {
        .family = AF_INET,
        .netif = SOCK_ADDR_ANY_NETIF,
        .port = SERVER_PORT};
    inet_pton(AF_INET, SERVER_ADDR, &gw_ep.addr.ipv6);

    // Connect to MQTT-SN Gateway
    mqttsn_connect(&client, &gw_ep, "esp32_client", false);

    // Publish loop
    char message[64];
    while (1) {
        snprintf(message, sizeof(message), "state: %s", "work");
        pkt_id = mqttsn_publish(&client, "status", (uint8_t*)message, strlen(message), 0);
        if (pkt_id < 0) {
            printf("Publish failed\n");
        } else {
            printf("Message: %s\n", message);
        }
        ztimer_sleep(ZTIMER_SEC, SLEEP_INTERVAL);
    }

    // Disconnect after the loop is done
    mqttsn_disconnect(&client);

    return NULL;
}

int main(void) {
    main_pid = thread_create(thread_stack, sizeof(thread_stack),
                             THREAD_PRIORITY_MAIN - 1,
                             THREAD_CREATE_STACKTEST,
                             mqtt_pub_thread, NULL, "mqtt_pub_thread");
    if (main_pid <= KERNEL_PID_UNDEF) {
        printf("Thread creation failed\n");
    }
    return 0;
}

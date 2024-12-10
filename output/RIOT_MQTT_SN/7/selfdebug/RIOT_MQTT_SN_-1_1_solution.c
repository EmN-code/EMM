#include <time.h>  // For using nanosleep
#include <stdio.h>   // For usage of printf
#include <string.h>  // For usage of strcpy
#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"

// Function to set up MQTT-SN client
void mqtt_sn_connect(emcute_sock_t *sock, const char *address_ip, int port_number) {
    // Define the network interface and address
    sock->remote_family = AF_INET;
    ipv6_addr_from_str((ipv6_addr_t *)&sock->remote_addr, address_ip);
    sock->port = port_number;
    
    // Establish connection
    if (emcute_con(sock, "device_id", true, 0, NULL) != EMCUTE_OK) {
        printf("MQTT-SN connection failed\n");
    } else {
        printf("MQTT-SN connected\n");
    }
}

// Main function
int main(void) {
    emcute_sock_t sock;
    const char* topic_name = "device/state";
    emcute_topic_t topic = {.name = topic_name, .id = 0};
    char *publish_data[2] = {"work", "not work"};

    // Establish MQTT-SN connection
    mqtt_sn_connect(&sock, "47.102.103.1", 1883);

    // Topic subscription
    unsigned flags = EMCUTE_QOS_0;
    if (emcute_sub(&topic, flags) != EMCUTE_OK) {
        printf("Error subscribing to topic\n");
        return 1;
    }

    // Infinite loop to publish data
    while (1) {
        for (int i = 0; i < 2; ++i) {
            emcute_pub(&topic, publish_data[i], strlen(publish_data[i]), false);
            printf("Published: %s\n", publish_data[i]);
            xtimer_sleep(5);  // Wait for 5 seconds
        }
    }

    return 0;
}
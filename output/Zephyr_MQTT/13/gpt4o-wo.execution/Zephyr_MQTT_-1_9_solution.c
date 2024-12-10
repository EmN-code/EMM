#include <zephyr/sys/mutex.h>
#include <zephyr/sys/math_extras.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/fdtable.h>
#include <zephyr/kernel.h>
#include <zephyr/net/wifi.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/websocket.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/net/offloaded_netdev.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/coap.h>
#include <zephyr/net/buf.h>
#include <zephyr/device.h>
#include <zephyr/syscalls/i2c.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/socket.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/tracing/tracing.h>
#include <zephyr/random/random.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/poll.h>
#include <zephyr/posix/netdb.h>
#include <zephyr/posix/arpa/inet.h>
#include <sys/time.h>
#include <time.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "state"
#define MESSAGE "work or not work"

K_THREAD_STACK_DEFINE(thread_stack, 1024);
struct k_thread thread_data;

// Function to connect to the MQTT broker
tatic void mqtt_connect_and_publish(void) {
    int sock;
    struct sockaddr_in broker;

    // Create socket
    sock = zsock_socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return;
    }

    // Configure broker address
    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.sin_addr);

    // Connect to broker
    if (zsock_connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        printf("Failed to connect to broker\n");
        zsock_close(sock);
        return;
    }

    while (1) {
        // Publish message to the topic
        if (zsock_send(sock, MESSAGE, strlen(MESSAGE), 0) < 0) {
            printf("Failed to send message\n");
        }
        // Wait for 5 seconds
        k_sleep(K_SECONDS(5));
    }

    zsock_close(sock);
}

// Entry point of the program
void main(void) {
    k_thread_create(&thread_data, thread_stack,
                    K_THREAD_STACK_SIZEOF(thread_stack),
                    (k_thread_entry_t)mqtt_connect_and_publish,
                    NULL, NULL, NULL, 7, 0, K_NO_WAIT);
}
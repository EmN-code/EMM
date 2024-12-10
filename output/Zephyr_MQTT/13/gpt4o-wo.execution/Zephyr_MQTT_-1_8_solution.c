#include zephyr/tracing/tracing_macros.h
#include zephyr/tracing/tracing.h
#include zephyr/syscalls/socket.h
#include zephyr/syscalls/random.h
#include zephyr/syscalls/kernel.h
#include zephyr/syscalls/i2c.h
#include zephyr/syscalls/flash.h
#include zephyr/sys/net_mgmt.h
#include zephyr/posix/sys/socket.h
#include zephyr/posix/unistd.h
#include zephyr/posix/poll.h
#include zephyr/posix/netdb.h
#include zephyr/posix/arpa/inet.h
#include zephyr/pm/state.h
#include zephyr/net/websocket.h
#include zephyr/net/wifi_mgmt.h
#include zephyr/net/wifi.h
#include zephyr/net/tls_credentials.h
#include zephyr/net/socket_select.h
#include zephyr/net/socket_poll.h
#include zephyr/net/socket.h
#include zephyr/net/offloaded_netdev.h
#include zephyr/net/net_mgmt.h
#include zephyr/net/net_ip.h
#include zephyr/net/mqtt_sn.h
#include zephyr/net/mqtt.h
#include zephyr/net/dns_resolve.h
#include zephyr/net/coap.h
#include zephyr/net/buf.h
#include zephyr/fs/nvs.h
#include zephyr/drivers/flash.h
#include zephyr/device.h
#include zephyr/devicetree.h
#include zephyr/kernel.h

// Other needed headers
#include <errno.h>

// Define constants
#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define MQTT_TOPIC "test/topic"
#define MQTT_STATE_WORK "work"
#define MQTT_STATE_NOT_WORK "not work"

struct sockaddr_in broker;
int sock;
bool toggle_work_state = true;

void main(void) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        // Handle error
        return;
    }

    // Configure broker's address and port
    memset(&broker, 0, sizeof(broker));
    broker.sin_family = AF_INET;
    broker.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &broker.sin_addr);

    // Connect to the MQTT broker
    if (connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
        // Handle error
        close(sock);
        return;
    }

    while (1) {
        // Alternate work state
        const char *state = toggle_work_state ? MQTT_STATE_WORK : MQTT_STATE_NOT_WORK;
        toggle_work_state = !toggle_work_state;

        // Send state to the MQTT broker
        if (send(sock, state, strlen(state), 0) < 0) {
            // Handle error
            break;
        }

        k_msleep(5000);
    }
    close(sock);
}

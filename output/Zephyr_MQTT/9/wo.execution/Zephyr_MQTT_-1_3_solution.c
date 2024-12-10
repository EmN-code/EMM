
#include <zephyr/kernel.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/socket_select.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/sys/printk.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0

static int mqtt_connect(int *sock)
{
    struct sockaddr_in broker_addr;

    *sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (*sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    if (connect(*sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        printk("Failed to connect to MQTT broker\n");
        close(*sock);
        return -1;
    }

    return 0;
}

static int mqtt_publish(int sock)
{
    uint8_t mqtt_packet[128];
    uint8_t *ptr = mqtt_packet;
    uint16_t len;

    // Fixed header
    *ptr++ = 0x30; // MQTT Control Packet Type (PUBLISH) and QoS level
    *ptr++ = 0x00; // Remaining length (to be filled later)

    // Variable header
    len = strlen(MQTT_TOPIC);
    *ptr++ = len >> 8;
    *ptr++ = len & 0xFF;
    memcpy(ptr, MQTT_TOPIC, len);
    ptr += len;

    // Payload
    len = strlen(MQTT_MESSAGE);
    memcpy(ptr, MQTT_MESSAGE, len);
    ptr += len;

    // Remaining length
    *mqtt_packet = (ptr - mqtt_packet - 1);

    if (send(sock, mqtt_packet, ptr - mqtt_packet, 0) < 0) {
        printk("Failed to send MQTT publish packet\n");
        return -1;
    }

    return 0;
}

int main(void)
{
    int sock;

    if (mqtt_connect(&sock) < 0) {
        return -1;
    }

    while (1) {
        if (mqtt_publish(sock) < 0) {
            break;
        }
        k_sleep(K_SECONDS(5));
    }

    close(sock);
    return 0;
}

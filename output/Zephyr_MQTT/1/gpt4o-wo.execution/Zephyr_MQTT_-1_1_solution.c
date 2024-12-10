// Necessary includes
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

// MQTT Broker Address and Port
#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "esp32/state"

// Function to initialize the socket and connect to the MQTT broker
int mqtt_connect(int *sock) {
    struct sockaddr_in broker_addr;
    int ret;

    *sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (*sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    // Initialize the broker address structure
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_addr.sin_addr);

    // Connect to the broker
    ret = connect(*sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr));
    if (ret < 0) {
        printk("Failed to connect to broker\n");
        close(*sock);
        return -1;
    }

    printk("Connected to broker\n");
    return 0;
}

// Function to publish data to the MQTT broker
void mqtt_publish(int sock, const char *message) {
    char buf[128];
    int len;

    // Construct MQTT publish packet (PUBLISH control packet format)
    // This is a simple format, a real implementation should handle MQTT control packets properly

    // Fixed header
    buf[0] = 0x30;  // PUBLISH command
    buf[1] = strlen(MQTT_TOPIC) + strlen(message) + 2;

    // Variable header
    memcpy(&buf[2], MQTT_TOPIC, strlen(MQTT_TOPIC));
    buf[2 + strlen(MQTT_TOPIC)] = 0;

    // Payload
    memcpy(&buf[2 + strlen(MQTT_TOPIC) + 1], message, strlen(message));

    // Send the publish packet
    len = send(sock, buf, 2 + strlen(MQTT_TOPIC) + 1 + strlen(message), 0);
    if (len < 0) {
        printk("Failed to send message\n");
    } else {
        printk("Message sent: %s\n", message);
    }
}

// Main function
int main(void) {
    int sock;
    const char *state_message = "work or not work";

    if (mqtt_connect(&sock) != 0) {
        return -1;
    }

    while (1) {
        mqtt_publish(sock, state_message);
        k_sleep(K_SECONDS(5));  // Sleep for 5 seconds
    }

    return 0;
}
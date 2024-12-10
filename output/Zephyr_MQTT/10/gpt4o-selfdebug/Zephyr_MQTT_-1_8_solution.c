#include <zephyr.h>   // Essential Zephyr kernel API
#include <net/socket.h>  // Socket APIs
#include <net/net_core.h>  // Network core functionalities

#define SERVER_ADDR "47.102.103.1"  // MQTT broker IP
#define SERVER_PORT 1883  // MQTT broker port

/* Publish state to MQTT broker every 5 seconds */
void main(void) {
    struct sockaddr_in broker;
    int sock;
    char message[] = "state: work";  // Example payload

    // Initialize broker address
    broker.sin_family = AF_INET;
    broker.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &broker.sin_addr);

    while (1) {
        // Create socket
        sock = socket(AF_INET, SOCK_STREAM, 0);

        if (sock < 0) {
            printk("Error creating socket\n");
            return;
        }

        // Connect to the broker
        if (connect(sock, (struct sockaddr *)&broker, sizeof(broker)) < 0) {
            printk("Connection failed\n");
            close(sock);
            return;
        }

        // Send a message to the broker
        send(sock, message, sizeof(message), 0);
        printk("Message sent: %s\n", message);

        // Close connection
        close(sock);

        // Wait before sending the next message
        k_sleep(K_SECONDS(5));
    }
}
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL 5

int main(void) {
    struct sockaddr_in broker_addr;
    int sockfd;
    char message[] = "state=work";
    
    // Create a socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    while (1) {
        // Send the state message to the MQTT broker
        if (sendto(sockfd, message, sizeof(message), 0, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
            perror("sendto");
            close(sockfd);
            return -1;
        }
        // Wait for the next interval
        sleep(PUBLISH_INTERVAL);
    }

    // Close the socket (unreachable code in this example)
    close(sockfd);

    return 0;
}
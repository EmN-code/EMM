#include <timex.h>
#include <time_units.h>
#include <thread_config.h>
#include <thread.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define COAP_PORT 5683
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_URI_PATH "/coap/test"

static const char *payload = "Hello, COAP Cloud";

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buf[256];

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COAP_PORT);
    inet_pton(AF_INET, COAP_SERVER_IP, &server_addr.sin_addr);

    // Construct CoAP POST request
    sprintf(buf, "POST %s CoAP/1.0\n\n%s", COAP_URI_PATH, payload);

    // Send request
    if (sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("sendto");
        close(sock);
        return -1;
    }

    printf("CoAP request sent\n");

    close(sock);
    return 0;
}
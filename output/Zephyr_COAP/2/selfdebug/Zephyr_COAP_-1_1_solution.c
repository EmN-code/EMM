#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>

#define COAP_MAX_PAYLOAD 32

static const char *uri_path = "coap/test";
static const char *payload = "Hello, COAP Cloud";
static const char *server_ip = "47.102.103.1";
static const int server_port = 5683;

int main(void) {
    int sock;
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t request_buf[128]; // Buffer to hold the CoAP request
    int ret;

    // Configure the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    // Create a socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return -1;
    }

    // Initialize CoAP packet
    ret = coap_packet_init(&request, request_buf, sizeof(request_buf),
                           COAP_VERSION_1, COAP_TYPE_CON,
                           0, NULL, 0, coap_next_id(),
                           COAP_METHOD_POST, uri_path);
    if (ret < 0) {
        printf("Failed to init CoAP packet\n");
        return -1;
    }

    // Set payload
    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        printf("Failed to set payload marker\n");
        return -1;
    }

    ret = coap_packet_append_payload(&request, (uint8_t *)payload, strlen(payload));
    if (ret < 0) {
        printf("Failed to append payload\n");
        return -1;
    }

    // Send CoAP request
    ret = sendto(sock, request.data, request.offset, 0,
                 (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printf("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printf("CoAP request sent successfully\n");

    // Close the socket
    close(sock);

    return 0;
}
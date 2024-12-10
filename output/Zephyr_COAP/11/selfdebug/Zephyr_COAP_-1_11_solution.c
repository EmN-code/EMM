
#include <zephyr.h>
#include <net/socket.h>
#include <net/coap.h>
#include <string.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

void send_coap_request(int sock, const struct sockaddr *server_addr) {
    struct coap_packet request;
    uint8_t buffer[128];
    int ret;

    ret = coap_packet_init(&request, buffer, sizeof(buffer), COAP_TYPE_CON, COAP_METHOD_POST, coap_next_id());
    if (ret < 0) {
        printf("Failed to initialize CoAP packet\n");
        return;
    }

    ret = coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));
    if (ret < 0) {
        printf("Failed to append URI path option\n");
        return;
    }

    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        printf("Failed to append payload marker\n");
        return;
    }

    ret = coap_packet_append_payload(&request, MESSAGE, strlen(MESSAGE));
    if (ret < 0) {
        printf("Failed to append payload\n");
        return;
    }

    ret = sendto(sock, request.data, request.offset, 0, server_addr, sizeof(struct sockaddr_in));
    if (ret < 0) {
        printf("Failed to send CoAP request\n");
        return;
    }

    printf("CoAP request sent successfully\n");
}

int main(void) {
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    send_coap_request(sock, (struct sockaddr *)&server_addr);

    close(sock);
    return 0;
}

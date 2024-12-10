#include <net/coap.h>
#include <net/socket.h>
#include <zephyr.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

void main(void) {
    struct sockaddr_in6 server_addr;
    int sock;
    struct coap_packet request;
    uint8_t token[8];
    uint8_t payload[] = MESSAGE;
    uint8_t buf[256];
    int result;

    printf("Sending CoAP POST request to server\n");

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET;
    server_addr.sin6_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin6_addr);

    result = coap_packet_init(&request, buf, sizeof(buf),
                              1, COAP_TYPE_CON, 8,
                              coap_next_token(), COAP_METHOD_POST,
                              coap_next_id());
    if (result < 0) {
        printf("Failed to initialize CoAP request\n");
        close(sock);
        return;
    }

    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, 
                              COAP_PATH, strlen(COAP_PATH));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, payload, sizeof(payload) - 1);

    if (sendto(sock, request.data, request.offset, 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Failed to send CoAP request\n");
    }
    printf("CoAP request sent successfully\n");
    close(sock);
}
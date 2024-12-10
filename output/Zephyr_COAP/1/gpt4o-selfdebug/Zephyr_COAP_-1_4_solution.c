#include <zephyr/types.h>
#include <zephyr.h>
#include <net/coap.h>
#include <net/socket.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683

#define POST_REQUEST_URI "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

void main(void) {
    struct sockaddr_in6 addr6;
    struct coap_packet request;
    int sock;
    int result;

    printf("Configuring network\n");

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create socket: %d\n", errno);
        return;
    }

    memset(&addr6, 0, sizeof(addr6));
    addr6.sin6_family = AF_INET;
    addr6.sin6_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &addr6.sin6_addr);

    result = connect(sock, (struct sockaddr *)&addr6, sizeof(addr6));
    if (result < 0) {
        printf("Failed to connect: %d\n", errno);
        close(sock);
        return;
    }

    printf("Setting up CoAP POST request\n");

    result = coap_packet_init(&request, NULL, 0, COAP_VERSION_1, COAP_TYPE_CON,
                              0, coap_next_id(), COAP_METHOD_POST,
                              (uint8_t *)POST_REQUEST_URI, strlen(POST_REQUEST_URI));
    if (result < 0) {
        printf("Failed to init CoAP packet: %d\n", result);
        close(sock);
        return;
    }

    result = coap_packet_append_payload_marker(&request);
    if (result < 0) {
        printf("Failed to append payload marker: %d\n", result);
        close(sock);
        return;
    }

    result = coap_packet_append_payload(&request, (uint8_t *) MESSAGE, strlen(MESSAGE));
    if (result < 0) {
        printf("Failed to append payload: %d\n", result);
        close(sock);
        return;
    }

    printf("Sending CoAP request\n");

    result = send(sock, request.data, request.offset, 0);
    if (result < 0) {
        printf("Failed to send data: %d\n", errno);
    } else {
        printf("CoAP POST request sent successfully\n");
    }

    close(sock);
}
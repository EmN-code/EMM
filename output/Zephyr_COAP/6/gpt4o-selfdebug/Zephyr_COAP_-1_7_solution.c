#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>

#define COAP_SERVER_HOST "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH     "/coap/test"
#define COAP_PAYLOAD      "Hello, COAP Cloud"

void main(void) {
    int sock;
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t request_data[128];

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET, COAP_SERVER_HOST, &server_addr.sin_addr);

    coap_packet_init(&request, request_data, sizeof(request_data),
                     COAP_VERSION_1, COAP_TYPE_CON, 0,
                     coap_next_id(), COAP_METHOD_POST, coap_next_token());

    coap_packet_append_option(&request, COAP_OPTION_URI_PATH,
                              (uint8_t *)COAP_URI_PATH, strlen(COAP_URI_PATH));

    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    sendto(sock, request_data, request.offset, 0,
           (struct sockaddr *)&server_addr, sizeof(server_addr));

    close(sock);
}
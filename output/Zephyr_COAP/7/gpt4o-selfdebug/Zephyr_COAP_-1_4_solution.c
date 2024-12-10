#include <zephyr/zephyr.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define MESSAGE_SIZE 64

void main(void) {
    struct sockaddr_in server_addr;
    int sock;
    struct coap_packet request;
    uint8_t token[8] = {0x00};  // Initial token
    uint8_t payload[] = "Hello, COAP Cloud";
    uint8_t msg_buf[MESSAGE_SIZE];

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Unable to create socket\n");
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    coap_packet_init(&request, msg_buf, sizeof(msg_buf), 
                     COAP_VERSION_1, COAP_TYPE_CON, 
                     sizeof(token), token, COAP_METHOD_POST, 
                     coap_next_id());

    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, "coap", 4);
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, "test", 4);

    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, payload, sizeof(payload) - 1);

    if (sendto(sock, request.data, request.offset, 0, 
               (struct sockaddr *)&server_addr, 
               sizeof(server_addr)) < 0) {
        printf("Error sending packet\n");
    }

    close(sock);
}
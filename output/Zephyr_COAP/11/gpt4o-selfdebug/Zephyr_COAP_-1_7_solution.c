#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define POST_DATA "Hello, COAP Cloud"

static int send_coap_post_request(void) {
    int sock;
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t request_data[128];
    const char *uri_path[] = { URI_PATH, NULL };

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printf("Failed to create UDP socket\n");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    coap_packet_init(&request, request_data, sizeof(request_data), 1, COAP_TYPE_CON, 8, coap_next_token(), COAP_METHOD_POST, coap_next_id());
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, URI_PATH, strlen(URI_PATH));
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)POST_DATA, strlen(POST_DATA));

    sendto(sock, request_data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    close(sock);
    return 0;
}

void main(void) {
    printf("Starting CoAP client example\n");

    while (1) {
        send_coap_post_request();
        k_sleep(K_SECONDS(10));
    }
}
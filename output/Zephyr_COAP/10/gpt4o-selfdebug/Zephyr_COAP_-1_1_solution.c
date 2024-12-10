#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define REQUEST_BUFFER_SIZE 256

void send_coap_request(void) {
    int sock;
    struct sockaddr_in server_addr;
    uint8_t request_buffer[REQUEST_BUFFER_SIZE];
    struct coap_packet request;
    int r;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    /* Initialize CoAP packet */
    coap_packet_init(&request, request_buffer, sizeof(request_buffer), 1, COAP_TYPE_CON, 8, coap_next_id(), COAP_METHOD_POST, coap_next_token());

    /* Set URI path */
    const char *uri_path = "/coap/test";
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH, uri_path, strlen(uri_path));

    /* Add payload */
    const char *payload = "Hello, COAP Cloud";
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)payload, strlen(payload));

    /* Send CoAP packet */
    r = sendto(sock, request.data, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (r < 0) {
        printk("Failed to send CoAP request\n");
    } else {
        printk("CoAP request sent successfully\n");
    }

    close(sock);
}

int main(void) {
    while (1) {
        send_coap_request();
        k_sleep(K_SECONDS(5));
    }
    return 0;
}

#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/posix/arpa/inet.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"
#define COAP_BUFFER_SIZE 256

void main(void) {
    int sock;
    struct sockaddr_in server_addr;
    uint8_t buffer[COAP_BUFFER_SIZE];
    struct coap_packet request;
    int r;

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket: %d\n", errno);
        return;
    }

    // Server address configuration
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    // Initialize CoAP request
    r = coap_packet_init(&request, buffer, sizeof(buffer),
                         COAP_VERSION_1,
                         COAP_TYPE_CON,
                         COAP_TOKEN_MAX_LEN,
                         0, COAP_METHOD_POST,
                         coap_next_id());
    if (r < 0) {
        printk("Failed to initialize CoAP request: %d\n", r);
        close(sock);
        return;
    }

    // Append URI_PATH option
    r = coap_packet_append_option(&request, COAP_OPTION_URI_PATH,
                                  COAP_PATH, strlen(COAP_PATH));
    if (r < 0) {
        printk("Unable to append option to request: %d\n", r);
        close(sock);
        return;
    }

    // Append payload
    r = coap_packet_append_payload_marker(&request);
    if (r < 0) {
        printk("Unable to append payload marker: %d\n", r);
        close(sock);
        return;
    }

    r = coap_packet_append_payload(&request, (uint8_t *)MESSAGE, strlen(MESSAGE));
    if (r < 0) {
        printk("Unable to append payload: %d\n", r);
        close(sock);
        return;
    }

    // Send CoAP request to server
    r = sendto(sock, buffer, coap_packet_get_len(&request), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (r < 0) {
        printk("Failed to send CoAP request: %d\n", errno);
    } else {
        printk("CoAP message sent\n");
    }

    close(sock);
}

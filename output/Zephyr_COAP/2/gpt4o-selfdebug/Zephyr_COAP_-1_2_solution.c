#include <zephyr/net/socket.h>
#include <zephyr/net/coap_client.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

void main(void) {
    int sock;
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t request_buffer[128];
    struct coap_option options[4];
    int ret;

    /* Creating a socket */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket. Error: %d\n", errno);
        return;
    }

    /* Defining server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET, COAP_SERVER_IP, &server_addr.sin_addr);

    /* Constructing CoAP request */
    ret = coap_packet_init(&request, request_buffer, sizeof(request_buffer),
                           COAP_TYPE_CON, COAP_METHOD_POST, coap_next_token(),
                           options, ARRAY_SIZE(options));
    if (ret < 0) {
        printk("Failed to initialize CoAP request. Error: %d\n", ret);
        close(sock);
        return;
    }

    ret = coap_packet_append_uri_path(&request, COAP_URI_PATH);
    if (ret < 0) {
        printk("Failed to append URI path. Error: %d\n", ret);
        close(sock);
        return;
    }

    ret = coap_packet_append_payload_marker(&request);
    if (ret < 0) {
        printk("Failed to append payload marker. Error: %d\n", ret);
        close(sock);
        return;
    }

    ret = coap_packet_append_payload(&request, (uint8_t *)COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    if (ret < 0) {
        printk("Failed to append payload. Error: %d\n", ret);
        close(sock);
        return;
    }

    /* Sending CoAP request */
    ret = sendto(sock, request_buffer, request.offset, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("Failed to send CoAP request. Error: %d\n", errno);
        close(sock);
        return;
    }

    printk("CoAP request sent successfully\n");

    close(sock);
}
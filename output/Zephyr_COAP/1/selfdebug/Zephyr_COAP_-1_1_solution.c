#include <zephyr/net/socket.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

int main() {
    struct sockaddr_in server_addr;
    struct coap_packet request;
    uint8_t request_buf[256];
    int sock;
    int ret;

    /* Create the socket for CoAP communication */
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    /* Configure server address struct */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET, COAP_SERVER_IP, &server_addr.sin_addr);

    /* Initialize CoAP packet */
    ret = coap_packet_init(&request, request_buf, sizeof(request_buf),
                           1, COAP_TYPE_CON, 8, coap_next_token(),
                           COAP_METHOD_POST, coap_next_id());
    if (ret < 0) {
        printk("Failed to initialize CoAP packet\n");
        close(sock);
        return -1;
    }

    /* Add URI_PATH option */
    coap_packet_append_option(&request, COAP_OPTION_URI_PATH,
                              COAP_URI_PATH, sizeof(COAP_URI_PATH) - 1);

    /* Set payload */
    coap_packet_append_payload_marker(&request);
    coap_packet_append_payload(&request, (uint8_t *)COAP_PAYLOAD,
                               sizeof(COAP_PAYLOAD) - 1);

    /* Send the CoAP packet */
    ret = sendto(sock, request.data, request.offset, 0,
                 (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printk("CoAP POST request sent successfully\n");

    /* Close the socket */
    close(sock);

    return 0;
}
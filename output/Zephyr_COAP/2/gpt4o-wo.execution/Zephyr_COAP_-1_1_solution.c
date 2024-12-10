#include <zephyr/net/coap.h>
#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#define COAP_BUF_SIZE 256

static const char *coap_server_ip = "47.102.103.1";
static const uint16_t coap_server_port = 5683;
static const char *coap_uri_path = "/coap/test";

static int send_coap_post_request(void) {
    struct sockaddr_in addr;
    struct coap_packet request;
    uint8_t request_buf[COAP_BUF_SIZE];
    uint8_t token[2] = {0x61, 0x62}; // Arbitrary token
    int sock;
    ssize_t sent;
    int r;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(coap_server_port);
    inet_pton(AF_INET, coap_server_ip, &addr.sin_addr);

    r = coap_packet_init(&request, request_buf, sizeof(request_buf),
                         COAP_VERSION_1, COAP_TYPE_CON, sizeof(token), token,
                         COAP_METHOD_POST, coap_next_id());
    if (r < 0) {
        printk("Failed to initialize CoAP request\n");
        close(sock);
        return -1;
    }

    r = coap_packet_append_option(&request, COAP_OPTION_URI_PATH,
                                  coap_uri_path, strlen(coap_uri_path));
    if (r < 0) {
        printk("Failed to append URI path\n");
        close(sock);
        return -1;
    }

    r = coap_packet_append_payload_marker(&request);
    if (r < 0) {
        printk("Failed to append payload marker\n");
        close(sock);
        return -1;
    }

    r = coap_packet_append_payload(&request, (uint8_t *)"Hello, COAP Cloud", 17);
    if (r < 0) {
        printk("Failed to append payload\n");
        close(sock);
        return -1;
    }

    sent = sendto(sock, request.data, request.offset, 0,
                  (struct sockaddr *)&addr, sizeof(addr));
    if (sent < 0) {
        printk("Failed to send CoAP request\n");
        close(sock);
        return -1;
    }

    printk("CoAP POST request sent successfully\n");
    close(sock);
    return 0;
}

void main(void) {
    while (1) {
        if (send_coap_post_request() < 0) {
            printk("Error sending CoAP request\n");
        }

        k_sleep(K_SECONDS(10));
    }
}
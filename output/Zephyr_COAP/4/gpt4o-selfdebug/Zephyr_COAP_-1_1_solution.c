#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/net/socket.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

#define STACK_SIZE 1024
#define PRIORITY 7

void coap_client_thread(void)
{
    int sock;
    struct sockaddr_in addr;
    struct coap_packet cpkt;
    uint8_t coap_buf[256];
    uint8_t coap_token[8] = "token1";
    int r;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(COAP_SERVER_PORT);
    inet_pton(AF_INET, COAP_SERVER_IP, &addr.sin_addr);

    r = coap_packet_init(&cpkt, coap_buf, sizeof(coap_buf),
                         COAP_VERSION_1,
                         COAP_TYPE_CON,
                         sizeof(coap_token), coap_token,
                         COAP_METHOD_POST, coap_next_id());
    if (r < 0) {
        printk("Failed to init CoAP message\n");
        close(sock);
        return;
    }

    r = coap_packet_append_option(&cpkt, COAP_OPTION_URI_PATH,
                                  COAP_URI_PATH,
                                  strlen(COAP_URI_PATH));
    if (r < 0) {
        printk("Failed to append option\n");
        close(sock);
        return;
    }

    r = coap_packet_append_payload_marker(&cpkt);
    if (r < 0) {
        printk("Failed to append payload marker\n");
        close(sock);
        return;
    }

    r = coap_packet_append_payload(&cpkt, (uint8_t *)COAP_PAYLOAD,
                                   strlen(COAP_PAYLOAD));
    if (r < 0) {
        printk("Failed to append payload\n");
        close(sock);
        return;
    }

    r = sendto(sock, cpkt.data, cpkt.offset, 0,
               (struct sockaddr *)&addr, sizeof(addr));
    if (r < 0) {
        printk("Failed to send CoAP request\n");
    } else {
        printk("CoAP request sent successfully\n");
    }

    close(sock);
}

void main(void)
{
    k_tid_t coap_tid;

    coap_tid = k_thread_create(
        (struct k_thread *)&coap_tid, K_THREAD_STACK_SIZEOF(coap_tid),
        coap_client_thread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
    k_thread_name_set(coap_tid, "coap_client_thread");
}
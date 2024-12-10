#include <zephyr/syscalls/i2c.h>
#include <zephyr/net/socket_poll.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/sys/iterable_sections.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/syscalls/flash.h>
#include <zephyr/syscalls/random.h>
#include <zephyr/tracing/tracing_macros.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683

static int sock;

void prepare_socket(void) {
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &addr.sin_addr);

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
}

void send_coap_post(void) {
    uint8_t buffer[128];
    struct coap_packet cpkt;

    coap_packet_init(&cpkt, buffer, sizeof(buffer), 1, COAP_TYPE_CON, 
                     COAP_TOKEN_LEN, COAP_CODE_POST, coap_next_id());

    coap_packet_append_uri_path(&cpkt, "coap/test");
    coap_packet_append_payload_marker(&cpkt);
    coap_packet_append_payload(&cpkt, (uint8_t *)"Hello, COAP Cloud", 
                               strlen("Hello, COAP Cloud"));

    send(sock, cpkt.data, cpkt.offset, 0);
}

int main(void) {
    prepare_socket();
    send_coap_post();
    close(sock);
    return 0;
}
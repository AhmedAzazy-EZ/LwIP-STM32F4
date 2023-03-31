/*
 * tcp_client.h
 *
 * Created: 31-March-23
 *  Author: Ahmed Azazy
 */
#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "lwip/tcp.h"
#include <stdint.h>

void tcp_setup(void);
void tcp_request(struct ip_addr * ip_addr , struct tcp_pcb * my_pcb);
void  tcp_error_callback(void *arg, err_t err);
err_t tcp_receive_callback(void *arg, struct tcp_pcb *tpcb,
                             struct pbuf *p, err_t err);
err_t tcp_sent_callback(void *arg, struct tcp_pcb *tpcb,
                              u16_t len);

err_t tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err);
uint32_t tcp_send_packet(struct tcp_pcb *tpcb);

#ifdef __cplusplus
}
#endif

#endif
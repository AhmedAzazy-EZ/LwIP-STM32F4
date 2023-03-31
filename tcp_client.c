/*
 * tcp_client.c
 *
 * Created: 31-March-23
 *  Author: Ahmed Azazy
 */

#include <stdint.h>
#include <string.h>
#include "tcp_client.h"

uint8_t Is_connected ;
char raw_buffer[2000];

void tcp_request(struct ip_addr * ip_addr , struct tcp_pcb * my_pcb)
{
	if(Is_connected == 0)
	{
		//create the protocol control block
		my_pcb = tcp_new(); 
		
		/* 
		 * passing the date to the tcp callback function
		 * but in this case I dont need to pass anything
		 * So pass NULL
		 */
		tcp_arg(my_pcb, NULL);

		/* 
		 * Register Call backs
		 */
    tcp_err(my_pcb, tcp_error_callback);
    tcp_recv(my_pcb, tcp_receive_callback);
    tcp_sent(my_pcb, tcp_sent_callback);

		tcp_connect(my_pcb, ip_addr, 80, tcp_connected_callback);
		Is_connected = 1;
	}
}


void  tcp_error_callback(void *arg, err_t err)
{
	
	//the err_t indicates the error reason e.g
	//ERR_ABRT: aborted through tcp_abort or by a TCP timer 
	//ERR_RST: the connection was reset by the remote host
	
}


err_t tcp_receive_callback(void *arg, struct tcp_pcb *tpcb,
                             struct pbuf *p, err_t err)
	{
		if(p == NULL)
		{
			//Error happened , return
		}
		else
		{
			strncpy(raw_buffer , (char *)p->payload , 2000);
		}
		
		if(tcp_close(tpcb) == ERR_OK)
			Is_connected = 0;
		
		return err;
	}
	


/**
 * This function is called when the sent
 * data have been acknowledged by the host
 */ 	
err_t tcp_sent_callback(void *arg, struct tcp_pcb *tpcb,
                              u16_t len)
{
	
}

/**
 * This function is called when a connection
 * has been established successfully
 */ 	
err_t tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err)
{

	tcp_send_packet(tpcb);
}

uint32_t tcp_send_packet(struct tcp_pcb *tpcb)
{
		err_t error;
    char *string = "HEAD / HTTP/1.0\r\nHost: 192.168.1.7\r\n\r\n ";
    uint32_t len = strlen(string);

    /* push to buffer */
        error = tcp_write(tpcb, string, strlen(string), TCP_WRITE_FLAG_COPY);

    if (error) {
        //Cant write
        return 1;
    }

    /* now send */
    error = tcp_output(tpcb);
    if (error) {
        //cant send
        return 1;
    }
    return 0;
}
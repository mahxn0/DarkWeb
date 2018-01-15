/*
 *  LibNoPoll: A websocket library
 *  Copyright (C) 2017 Advanced Software Production Line, S.L.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *  
 *  You may find a copy of the license under this software is released
 *  at COPYING file. This is LGPL software: you are welcome to develop
 *  proprietary applications using this library without any royalty or
 *  fee but returning back any change, improvement or addition in the
 *  form of source code, project image, documentation patches, etc.
 *
 *  For commercial support on build Websocket enabled solutions
 *  contact us:
 *          
 *      Postal address:
 *         Advanced Software Production Line, S.L.
 *         Av. Juan Carlos I, Nº13, 2ºC
 *         Alcalá de Henares 28806 Madrid
 *         Spain
 *
 *      Email address:
 *         info@aspl.es - http://www.aspl.es/nopoll
 */
#include <nopoll-regression-common.h>
#include <nopoll.h>

#include <signal.h>

#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/x509v3.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#include <sys/socket.h>  
#include <netinet/in.h>  



typedef struct Node  
{  
    int data;
	noPollConn * conn_cur;
    struct Node *pNext;
}NODE,*PNODE; 

PNODE pHead=NULL; 


#include "./xml_op.h"


int connection_close_count = 0;

noPollConn * conn_cur;
int flag = 0;

char buf[1024];



void __nopoll_regression_on_close (noPollCtx * ctx, noPollConn * conn, noPollPtr user_data)
{
	printf ("Reg test: called connection close (TLS: %d)..\n", nopoll_conn_is_tls_on (conn));
	connection_close_count++;
	conn_cur = NULL;
	flag = 1;

	PNODE p=pHead->pNext; 
	PNODE pre=pHead;
	PNODE q;
	int Val = nopoll_conn_get_id(conn);
	printf("conn id = %d\n", Val);
      
	while(p != NULL)
	{
		if(p->data == Val)
		{
			q = p;
			p = p->pNext;
			pre->pNext = p;
			free(q);
		}
		else
		{
			pre = p;
			p = p->pNext;
		}
	}
	printf("end here\n");
	
	return;
}

nopoll_bool on_connection_opened (noPollCtx * ctx, noPollConn * conn, noPollPtr user_data)
{
	/* set connection close */
	nopoll_conn_set_on_close (conn, __nopoll_regression_on_close, NULL);

	if (! nopoll_conn_set_sock_block (nopoll_conn_socket (conn), nopoll_false)) {
		printf ("ERROR: failed to configure non-blocking state to connection..\n");
		return nopoll_false;
	} /* end if */

	/* check to reject */
	if (nopoll_cmp (nopoll_conn_get_origin (conn), "http://deny.aspl.es"))  {
		printf ("INFO: rejected connection from %s, with Host: %s and Origin: %s\n",
			nopoll_conn_host (conn), nopoll_conn_get_host_header (conn), nopoll_conn_get_origin (conn));
		return nopoll_false;
	} /* end if */

	/* get protocol to reply an especific case. This is an example
	   on how to detect protocols requested by the client and how
	   to reply with a particular value at the server. */
	printf ("Requested protocol: %s\n", nopoll_conn_get_requested_protocol (conn));
	if (nopoll_cmp (nopoll_conn_get_requested_protocol (conn), "hello-protocol")) {
		/* set hello-protocol-response */
		nopoll_conn_set_accepted_protocol (conn, "hello-protocol-response");
	} /* end if */

	/* notify connection accepted */
	/* printf ("INFO: connection received from %s, with Host: %s and Origin: %s\n",
	   nopoll_conn_host (conn), nopoll_conn_get_host_header (conn), nopoll_conn_get_origin (conn)); */
	return nopoll_true;
}

nopoll_bool on_connection_opened_c (noPollCtx * ctx, noPollConn * conn, noPollPtr user_data)
{
	flag = 1;
	/* set connection close */
	nopoll_conn_set_on_close (conn, __nopoll_regression_on_close, NULL);

	if (! nopoll_conn_set_sock_block (nopoll_conn_socket (conn), nopoll_false)) {
		printf ("ERROR: failed to configure non-blocking state to connection..\n");
		return nopoll_false;
	} /* end if */

	printf("enter open \n");

	/* notify connection accepted */

	printf("open id = %d\n", nopoll_conn_get_id(conn));
	
	conn_cur = conn;

	PNODE p=pHead;       
    while(NULL!=p->pNext) 
    {  
        p=p->pNext;  
    }  
  
    PNODE pNew=(PNODE)malloc(sizeof(NODE));  
     if(NULL==pNew)  
    {  
        return nopoll_false;  
    }  
          
    pNew->data =nopoll_conn_get_id(conn);  
	pNew->conn_cur = conn;
    p->pNext =pNew;  
    pNew->pNext = NULL; 

	
	
	/*
	 printf ("INFO: connection received from %s, with Host: %s and Origin: %s\n",
	   nopoll_conn_host (conn), nopoll_conn_get_host_header (conn), nopoll_conn_get_origin (conn)); 
	*/
	return nopoll_true;
}


noPollMsg * previous_msg = NULL;

void write_file_handler (noPollCtx * ctx, noPollConn * conn, noPollMsg * msg, noPollPtr user_data)
{
	FILE       * open_file_cmd = (FILE*) user_data;
	const char * content = (const char *) nopoll_msg_get_payload (msg);
	int          value;

	/* check for close operation */
	if (nopoll_ncmp (content, "close-file", 10)) {
		printf ("CLOSING FILE: opened..\n");
		fclose (open_file_cmd);
		open_file_cmd = NULL;
		return;
	} /* end if */

	if (open_file_cmd) {
		/* write content */
	        value = fwrite (content, 1, nopoll_msg_get_payload_size (msg), open_file_cmd);
	        if (value < 0)
		        return;

		return;
	} /* end if */
	return;
}

void listener_on_message (noPollCtx * ctx, noPollConn * conn, noPollMsg * msg, noPollPtr user_data)
{
	int          bytes;
	const char * content = (const char *) nopoll_msg_get_payload (msg);
	bytes = nopoll_msg_get_payload_size (msg);

	printf("listener content = %s  size = %d\n", content, bytes);
	/*
	printf("recv conn->id = %d, conn->ctx->conn_id = %d, conn->ctx->conn_num = %d\n", conn->id, conn->ctx->conn_id, conn->ctx->conn_num);
	*/

	printf("listen id = %d\n", nopoll_conn_get_id(conn));

	nopoll_conn_send_text (conn, "11 Message received1", 16);
	
/*

	while(1)
	{
		nopoll_conn_send_text (conn, "11 Message received1", 16);
		printf("send 11 Message received1\n");
		sleep(15);
	}
*/
	return;
}

noPollCtx      * ctx = NULL;

void __terminate_listener (int value)
{
	printf ("__terminate_listener: Signal received...terminating listener..\n");
	/* unlock listener */
	nopoll_loop_stop (ctx);

	return;
}

int verify_callback (int ok, X509_STORE_CTX * store) {
	char   data[256];
	X509 * cert;
	int    depth;
	int    err;

	if (! ok) {
		cert  = X509_STORE_CTX_get_current_cert (store);
		depth = X509_STORE_CTX_get_error_depth (store);
		err   = X509_STORE_CTX_get_error (store);

		printf ("CERTIFICATE: error at depth: %d\n", depth);

		X509_NAME_oneline (X509_get_issuer_name (cert), data, 256);
		printf ("CERTIFICATE: issuer: %s\n", data);

		X509_NAME_oneline (X509_get_subject_name (cert), data, 256);
		printf ("CERTIFICATE: subject: %s\n", data);

		printf ("CERTIFICATE: error %d:%s\n", err, X509_verify_cert_error_string (err));

	}
	return ok; /* return same value */
}

noPollPtr ssl_context_creator (noPollCtx * ctx, noPollConn * conn, noPollConnOpts * opts, nopoll_bool is_client, noPollPtr user_data)
{
	SSL_CTX             * ssl_ctx;
	noPollConn          * listener;

	/* very basic context creation using default settings provided
	 * by OpenSSL */
	if (is_client) {
#if OPENSSL_VERSION_NUMBER < 0x10100000L
		return SSL_CTX_new (TLSv1_client_method ());
#else
	        return SSL_CTX_new (TLS_client_method ());
#endif
	} /* end if */

	/* get the ssl context */
#if OPENSSL_VERSION_NUMBER < 0x10100000L
	ssl_ctx = SSL_CTX_new (TLSv1_server_method ());
#else
	ssl_ctx = SSL_CTX_new (TLS_server_method ());
#endif

	/* get a reference to the listener */
	listener = nopoll_conn_get_listener (conn);

	if (nopoll_cmp ("1239", nopoll_conn_port (listener))) {
		printf ("ACCEPTED ssl connection on port: %s (for conn %p)\n", nopoll_conn_port (listener), conn);

		/* ok, especiall case where we require a certain
		 * certificate from renote side */
		if (SSL_CTX_load_verify_locations (ssl_ctx, "client-side-cert-auth-cacert.crt", NULL) != 1) {
			printf ("ERROR: unable to add ca certificate...\n");
		}


		/* make server to ask for a certificate to the client
		 * .... and verify it */
		SSL_CTX_set_verify (ssl_ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, verify_callback);
	} /* end if */
	
	printf ("RETURNING: ssl context reference %p\n", ssl_ctx);
	return ssl_ctx;
}

void *thread_js(void)
{
	int sock;  
	struct sockaddr_in servaddr; 
	char recvbuf[1024] = {0};
	struct sockaddr_in peeraddr;  
    socklen_t peerlen;  
    int n; 
   
    if((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0){   
        exit(0);  
    }  
 
    memset(&servaddr, 0, sizeof(servaddr));  
    
    servaddr.sin_family = AF_INET;  
    servaddr.sin_port = htons(5188);  
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  
  
    if(bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){  
        exit(0);  
    }

	memset(recvbuf, 0, sizeof(recvbuf)); 
	while(1)
	{
		memset(recvbuf, 0, sizeof(recvbuf)); 
		n = recvfrom(sock, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&peeraddr, &peerlen);  
		if(n > 0)
		{  
			flag = 0;
			memset(buf, 0, sizeof(buf)); 
			/*
			if(strcmp(recvbuf,"") == 0)
			{
				printf("recvbuf is null\n");
			}
			*/
			strcpy(buf,recvbuf);
			flag = 1;
			printf("recvbuf = %s\n", recvbuf);
		} 	
	}
}

#if 1
void *thread_ws(void)
{
	PNODE p;
	

	while(1)
	{
		usleep(100000);
		/*
		printf("111111115\n");
		*/
		p=pHead->pNext;  
	    while(NULL!=p)  
	    {  
	    	if(flag == 1)
	    	{
				nopoll_conn_send_text (p->conn_cur , buf, strlen(buf));
		        p=p->pNext; 
	    	}
	    } 

	}
}

#else
void *thread_ws(void)
{
	PNODE p;
	char buff[60][128]={
			"1:tvmonitor,1133,343,151,119;1:person,621,345,254,274;1:motorbike,413,532,826,374;",
			"1:tvmonitor,1134,343,150,118;1:person,620,344,261,271;1:motorbike,413,532,826,374;",
			"1:tvmonitor,1134,343,148,118;1:person,620,343,265,267;",
			"1:tvmonitor,1133,343,149,116;1:person,620,342,266,265;",
			"1:tvmonitor,1133,343,149,116;1:person,619,343,263,264;",
			"1:tvmonitor,1133,343,149,116;1:person,620,343,262,264;",
			"1:tvmonitor,1133,343,149,116;1:person,620,343,261,264;",
			"1:tvmonitor,1133,343,149,115;1:person,620,343,259,266;",
			"1:tvmonitor,1133,344,149,114;1:person,621,343,259,267;",
			"1:person,623,344,258,268;",
			"1:tvmonitor,1133,343,151,113;1:person,624,343,259,269;",
			"1:person,557,298,172,163;1:tvmonitor,1134,344,150,114;1:person,623,343,261,273;",
			"1:person,556,298,173,165;1:tvmonitor,1133,344,149,114;1:person,623,343,262,275;",
			"1:person,556,298,172,166;1:tvmonitor,1134,344,148,114;1:person,623,343,262,277;",
			"1:person,556,298,171,166;1:tvmonitor,1134,344,147,114;1:person,623,343,262,279;",
			"1:person,557,298,172,166;1:tvmonitor,1134,344,145,114;1:person,623,343,263,279;",
			"1:person,557,297,172,167;1:tvmonitor,1134,344,145,114;1:person,623,343,263,281;1:motorbike,414,530,828,377;",
			"1:person,556,298,171,168;1:tvmonitor,1134,343,146,115;1:person,624,343,264,280;",
			"1:person,557,298,172,168;1:tvmonitor,1134,343,148,116;1:person,623,343,265,281;",
			"1:person,557,298,172,168;1:tvmonitor,1134,343,148,116;1:person,623,344,266,280;",
			"1:tvmonitor,1134,343,146,118;1:person,622,344,265,278;",
			"1:tvmonitor,1134,343,146,118;1:person,622,343,264,277;",
			"1:tvmonitor,1134,343,144,118;1:person,621,343,264,277;",
			"1:tvmonitor,1134,342,144,117;1:person,621,343,264,279;",
			"1:tvmonitor,1134,343,144,118;1:person,622,343,263,275;",
			"1:tvmonitor,1134,343,144,119;1:person,623,343,262,273;1:motorbike,414,532,828,373;",
			"1:person,557,298,170,168;1:tvmonitor,1133,343,143,121;1:person,623,343,260,269;1:motorbike,413,532,827,374;",
			"1:person,557,298,170,170;1:tvmonitor,1133,343,143,121;1:person,566,356,261,310;1:motorbike,413,532,827,374;",
			"1:person,557,298,170,170;1:tvmonitor,1133,343,145,121;1:person,567,356,260,311;1:motorbike,413,532,826,373;",
			"1:person,557,297,170,171;1:tvmonitor,1133,343,145,120;1:person,624,344,262,275;1:motorbike,413,532,826,373;",
			"1:person,557,298,169,170;1:tvmonitor,1133,343,145,119;1:person,624,344,261,279;1:motorbike,413,532,826,373;",
			"1:person,554,298,173,153;1:tvmonitor,1133,347,140,114;1:person,560,356,253,297;",
			"1:person,545,299,180,145;1:person,553,354,245,289;1:motorbike,405,528,810,381;",
			"1:person,535,299,193,141;1:tvmonitor,1036,338,152,124;1:person,546,352,244,282;1:motorbike,394,523,789,391;",
			"1:tvmonitor,1033,336,149,128;1:person,544,349,240,283;1:motorbike,379,518,759,402;",
			"1:tvmonitor,1028,337,145,123;1:person,537,347,238,272;",
			"1:person,516,302,160,136;1:tvmonitor,1023,339,135,114;1:person,532,346,243,251;",
			"1:tvmonitor,1023,343,125,108;1:person,459,360,235,281;",
			"1:person,452,358,238,282;",
			"1:person,449,356,232,287;1:bicycle,243,467,382,374;",
			"1:person,450,356,235,288;",
			"1:person,450,356,235,287;",
			"1:person,450,355,234,285;",
			"1:person,450,356,233,284;",
			"1:person,450,355,233,283;",
			"1:person,450,355,233,283;",
			"1:person,449,357,240,283;",
			"1:person,447,360,247,285;",
			"1:person,446,362,252,289;",
			"1:person,447,362,248,290;",
			"1:person,447,362,249,292;",
			"1:person,447,362,246,293;",
			"1:person,446,364,246,297;",
			"1:person,444,365,243,300;",
			"1:person,444,365,243,300;",
			"1:person,444,364,242,301;",
			"1:person,443,365,241,301;",
			"1:person,443,366,239,300;",
			"1:person,443,366,237,300;",
			"1:person,444,365,238,304;"
		};
	int i = 0;

	while(1)
	{
		usleep(70000);
		/*
		printf("111111115\n");
		*/
		p=pHead->pNext;  
	    while(NULL!=p)  
	    {  
	    	if(flag == 0)
	    	{
				nopoll_conn_send_text (p->conn_cur , buff[i], strlen(buff[i]));
	    	}
			else
			{
				usleep(1000000);
				flag = 0;
				break;
			}
	        p=p->pNext;  
	    } 

		i++;
		if(i == 60)
			i = 0;
	}
}
#endif

int main (int argc, char ** argv)
{
	noPollConn     * listener;
#if 0
	PNODE p;
	char buf[56];	
	int i = 0;
#endif

	pHead=(PNODE)malloc(sizeof(NODE));  
    if(NULL==pHead)  
    {  
        return -1;   
    }  
    pHead->pNext =NULL;  

	int temp;
	pthread_t thread[2];

	signal (SIGTERM,  __terminate_listener);

	/* create the context */
	ctx = nopoll_ctx_new ();
 

	/* call to create a listener */
	listener = nopoll_listener_new (ctx, "0.0.0.0", "1234");
	if (! nopoll_conn_is_ok (listener)) {
		printf ("ERROR: Expected to find proper listener connection status, but found..\n");
		return -1;
	}



	/* set on message received */
	nopoll_ctx_set_on_msg (ctx, listener_on_message, NULL);

	/* set on open */
	nopoll_ctx_set_on_open (ctx, on_connection_opened_c, NULL);


#if 0
	while(1)
	{
		/*
		nopoll_loop_wait (ctx, 1000000);
		*/
		nopoll_loop_wait (ctx, 100000);
		i++;
		/*
		printf("i = %d\n",i);
		*/
		#if 1
		memset(buf, 0 , sizeof(buf));
		sprintf(buf,"i = %d", i);
	/*
		if(conn_cur != NULL)
		{
			printf("eeeeee\n");
			nopoll_conn_send_text (conn_cur, "22 Message received2", 16);
		}
   */
		p=pHead->pNext;  
	    while(NULL!=p)  
	    {  
			nopoll_conn_send_text (p->conn_cur , buf, strlen(buf));
	        p=p->pNext;  
	    }   
		/*
		printf("kkkkkkkk\n");
		*/
		#endif
	}
#endif

	if((temp = pthread_create(&thread[0] , NULL , (void *)thread_ws, NULL)) != 0)
	{
		printf("thread1 create error\n");
		return -1;
	}
	if((temp = pthread_create(&thread[1] , NULL , (void *)thread_js, NULL)) != 0)
	{
		printf("thread1 create error\n");
		return -1;
	}
	nopoll_loop_wait (ctx, 0);

	/* unref connection */
	nopoll_conn_close (listener);

	nopoll_ctx_unref (ctx);

	/* call to release all pending memory allocated as a
	 * consequence of using nopoll (especially TLS) */
	nopoll_cleanup_library ();

	return 0;
}

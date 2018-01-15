
#include <nopoll-regression-common.h>
#include <nopoll.h>

void listener_on_message (noPollCtx * ctx, noPollConn * conn, noPollMsg * msg, noPollPtr user_data)
{

	int          bytes;
	const char * content = (const char *) nopoll_msg_get_payload (msg);
	bytes = nopoll_msg_get_payload_size (msg);

	printf("client content = %s  size = %d\n", content, bytes);

	return;
}


int main (int argc, char ** argv)
{

	noPollCtx  * ctx;
	noPollConn * conn;

	/* create the context */
	ctx = nopoll_ctx_new ();

	/* call to create a connection */
	conn = nopoll_conn_new (ctx, "192.168.1.50", "1234", NULL, NULL, NULL, NULL);
	if (! nopoll_conn_is_ok (conn)) {
	        printf ("ERROR: Expected to find proper client connection status, but found error (conn=%p, conn->session=%d, NOPOLL_INVALID_SOCKET=%d)..\n",
			conn, (int) nopoll_conn_socket (conn), (int) NOPOLL_INVALID_SOCKET);
		return nopoll_false;
	}

	if (! nopoll_conn_wait_until_connection_ready (conn, 5)) {
		printf("error\n");
        return nopoll_false;
	}
	else
	{
		printf("conn is ready\n");
	}



	if (nopoll_conn_send_text (conn, "Hello there! this is a test", 27) != 27) {
			printf("send error\n");
		}

	/* set on message received */
	nopoll_ctx_set_on_msg (ctx, listener_on_message, NULL);

	/* process events */
	nopoll_loop_wait (ctx, 0);

	/* finish connection */
	nopoll_conn_close (conn);
	
	/* finish */
	nopoll_ctx_unref (ctx);



	/* call to cleanup */
	nopoll_cleanup_library ();
	printf ("All tests ok!!\n");
	return 0;
}


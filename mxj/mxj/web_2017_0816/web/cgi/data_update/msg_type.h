
#ifndef __TIRVIDEO_IPC_H__
#define __TIRVIDEO_IPC_H__

typedef struct _MSG_BUF
{
	int m_type;
	int buf_len;
	char buf[128];
}MSG_BUF;


typedef enum
{
    MSG_SET_BIT_RATE = 0,
	MSG_SET_CBR_VBR,
    MSG_MAX
} _MSG_TYPE;
#endif

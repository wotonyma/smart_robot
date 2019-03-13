#ifndef __SOCKET_H_
#define __SOCKET_H_

#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
int recv_file(char * file_path);
int send_result(char * result);
int close_all();

int recv_from_arm(char * msg_buff,unsigned int msg_len);
int send_to_arm(char * data,unsigned int len);
int close_arm_connect();
#endif
/*
创建TCP/IP  client客户端  文件传输


*/


#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>  //socket  bind
#include <sys/types.h>    /* bind */

#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
 
#include <arpa/inet.h> //inet_addr
#include <unistd.h>  //unistd
#include <strings.h>  //bzero
#include <string.h>  //memset
#include <pthread.h> 
 
 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "main.h"
#include "show_photo.h"
#include "show_music.h"
#include "show_video.h"
#include "client_file.h"
//#include "input.h"
//#include "client.h"
 
int  sockfd; 
 


// int main(int argc, char **argv)
int client_init(const char *sound_file)
{
	//1创建socket套接字
	  
	// sockfd = socket(int socket_family, int socket_type, int protocol);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		printf("socket create failed:\n");
		exit(1);
	}
	
	// struct sockaddr_in {
               // sa_family_t    sin_family; /* address family: AF_INET */
               // in_port_t      sin_port;   /* port in network byte order */
               // struct in_addr sin_addr;   /* internet address */
           // };
	//htons是将整型变量从主机字节顺序转变成网络字节顺序
	struct sockaddr_in server_addr;
	server_addr.sin_family =AF_INET;
	server_addr.sin_port = htons(8888);
	server_addr.sin_addr.s_addr = inet_addr("192.168.0.100");

	
	
	
	//4与服务器建立连接
	// int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
	int  crtv =connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
	if(crtv == -1)
	{
		printf("socket create failed:\n");
		exit(1);
	}
	else
		printf("connect success \n");
	

	FILE *fp =fopen(sound_file, "r");
	if(fp == NULL)
	{
		printf("fopen  failed:\n");
		exit(1);
	}
	//发送文件名称
	char file_name[20]={0};
	strcpy(file_name,sound_file);
	printf("file_name :%s\n",file_name);

	ssize_t stv =  send(sockfd, file_name, 20, 0); //1111.pcm
	 printf("stv = %d\n", stv);
	//2获取文件的大小
	struct stat statbuf;
    stat(sound_file, &statbuf);
	int  file_len = statbuf.st_size;
    printf("file_len = %ld\n", statbuf.st_size);
	
	//把文件长度转为字符串
	char filename_len[10]={0};
	memset(filename_len, 0, sizeof(filename_len));
	sprintf(filename_len,"%d",file_len);
	printf("filename_len:%s \n",filename_len);

	stv = send(sockfd, filename_len, 10, 0);
	printf("stv =%d\n",stv);
	// 把文件的数据读取到filename_data数组中 并且发送给服务器
	char filename_data[1024]={0};
	size_t len;
	while(1)
	{
		memset(filename_data, 0, sizeof(filename_data));
		// size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
		len =fread(filename_data,sizeof(char),1024,fp);
		printf("len =%d\n",len);
		if(len == 0)
			break;
		stv = send(sockfd, filename_data, len, 0);
	}
	
	//7	关闭套接字
	fclose(fp);
	
		
	return 0;
}

char rslt[1024]={0};
void get_rslt(void)
{
	recv(sockfd, rslt, 1024, 0);
	printf("rslt =%s\n",rslt);
}

void shibie(void)
{
	
	
	
	client_init("11111.pcm");
	get_rslt();
	
	if(strstr(rslt, "相册") != NULL)
	{
		player_flag = 2;
		mode_flag = 1;
		pic_flag = 3;
		printf("相册开启\n"); //
	}
	if(strstr(rslt, "音乐") != NULL)
	{
		player_flag = 2;
		mode_flag = 2;
		pic_flag = 1;
		printf("音乐开启\n");//open_music
	}
	if(strstr(rslt, "视频") != NULL)
	{
		player_flag = 2;
		mode_flag = 2;
		pic_flag = 2;
		printf("视频开启\n");//open_music
	}
	
	
	close(sockfd);
}
 
 /*void voice_init(void)
 {
		
	//system("arecord -d5 -c1 -r16000 -twav -fS16_LE 11111.pcm ");

	shibie();		 		
	
 }*/


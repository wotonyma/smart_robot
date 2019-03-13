
#include <stdio.h>
#include "websocket.h"
#include "handle_protcol.h"
#include <pthread.h>
#include "asr_sample.h"
#include "socket.h"
/////////////////////////
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>


/////////////////////////

void *pthread_func(void * arg){

	init_websocket_v12();
	return NULL;
}

void *pthread_socket_data(void *arg)
{
	char msg_buff[8] = {0};

	while(1){

		recv_from_arm(msg_buff,8);
		websocket_send(msg_buff,8);
	}
	return NULL;
}


char dht11_buf[6];  //存放温湿度temperature/humidity

int tmp_test_pro(){
	fflush(stdin);
	protocol *tmp_pro = create_pro();
	char wendu[4];
	//char shidu[4];
	sprintf(wendu, "%c%c", dht11_buf[1], dht11_buf[2]);
	//sprintf(shidu, "%c%c", dht11_buf[4], dht11_buf[5]);

	int intwendu = atoi(wendu);
	//int intshidu = atoi(shidu);

	putData(tmp_pro,WX_APP,WX_APP_T,0,0,intwendu);

	//putData(tmp_pro,WX_APP,WX_APP_H,0,0,intshidu);


	printf("0x%8x,0x%8x\n", *(unsigned int *)tmp_pro,*((unsigned int *)tmp_pro+1));
	host2net(tmp_pro);
	websocket_send(tmp_pro,8);
	free_pro(tmp_pro);
	return 0;

}

int hum_test_pro(){
	fflush(stdin);
	protocol *tmp_pro = create_pro();
	//char wendu[4];
	char shidu[4];
	//sprintf(wendu, "%c%c", dht11_buf[1], dht11_buf[2]);
	sprintf(shidu, "%c%c", dht11_buf[4], dht11_buf[5]);

	//int intwendu = atoi(wendu);
	int intshidu = atoi(shidu);

	//putData(tmp_pro,WX_APP,WX_APP_T,0,0,intwendu);

	putData(tmp_pro,WX_APP,WX_APP_H,0,0,intshidu);


	printf("0x%8x,0x%8x\n", *(unsigned int *)tmp_pro,*((unsigned int *)tmp_pro+1));
	host2net(tmp_pro);
	websocket_send(tmp_pro,8);
	free_pro(tmp_pro);
	return 0;

}





int gecsocket_init()
{
	int sockfd,new_fd;/*socket句柄和建立连接后的句柄*/
	struct sockaddr_in my_addr;/*本方地址信息结构体，下面有具体的属性赋值*/
	struct sockaddr_in their_addr;/*对方地址信息*/
	int sin_size;
 
	sockfd=socket(AF_INET,SOCK_STREAM,0);//建立socket 
	if(sockfd==-1){
		printf("socket failed:%d",errno);
		return -1;
	}
	my_addr.sin_family=AF_INET;/*该属性表示接收本机或其他机器传输*/
	my_addr.sin_port=htons(8888);/*端口号*/
	my_addr.sin_addr.s_addr=htonl(INADDR_ANY);/*IP，括号内容表示本机IP*/
	bzero(&(my_addr.sin_zero),8);/*将其他属性置0*/
	if(bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr))<0){//绑定地址结构体和socket
		printf("bind error");
		return -1;
	}

    listen(sockfd,2);//开启监听 ，第二个参数是最大监听数 
    sin_size=sizeof(struct sockaddr_in);
    new_fd=accept(sockfd,(struct sockaddr*)&their_addr,&sin_size);//在这里阻塞知道接收到消息，参数分别是socket句柄，接收到的地址信息以及大小 
    	
	return new_fd;
}


int main(int argc, char const *argv[])
{
	
	
	pthread_t nopoll_id;
	pthread_create(&nopoll_id,NULL,pthread_func,NULL);


	//语音识别
	pthread_t aitalk_id;
	pthread_create(&aitalk_id,NULL,aitalk,NULL);

	//数据
	pthread_t socket_data_id;
	pthread_create(&socket_data_id,NULL,pthread_socket_data,NULL);

	//8888端口用来监听开发板
	int gec_fd = gecsocket_init();

	while(1){

		recv(gec_fd, dht11_buf, 6, 0);
		printf("recv dht11: %s\n", dht11_buf);
		tmp_test_pro();
		hum_test_pro();
	}

	return 0;
}
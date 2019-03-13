#include <stdio.h>			//printf
#include <sys/types.h>		//open
#include <sys/stat.h>		//open
#include <fcntl.h>			//open
#include <termios.h>		//tcflush
#include <unistd.h>			//tcflush
#include <string.h>			//memset
#include <pthread.h>
#include <sys/ioctl.h>
#include <signal.h>

/////////////////////////显示汉字
#include "lcd_app.h"

#include "show_cntldht11.h"

////////////////////

#include "main.h"
#include "lcd.h"
#include "ts.h"
#include "ui.h"
///////////////////

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
 #include <netdb.h>
#include <sys/types.h>
///////////////////////


#define DEVICE_NAME "/dev/ttySAC1"
int ser_fd = 0;
char message[6];        //存储dht11温湿度
pthread_t dht11_pid = 0;	//dht11线程pid,用于start,finish
int sockfd = 0;

void *dht11_thread (void *arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);           //允许退出线程   
 	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);   //设置立即取消，没这个不行

	uart_open(); //打开文件描述符
	while(1)
	{
		//show_dht11();
		read(ser_fd, message, 6);  //循环读取将数据放入全局缓冲区
		tcflush( ser_fd, TCIOFLUSH); //刷新缓冲区


		if(mode_flag == 4 && pic_flag == 1)
		{
			//lcd_draw_jpg(0, 0, "../image/hands_control.jpg", NULL, 0, 0);
			show_dht11();
		}

		//////////////////////////////////////////////将数据发送给服务器
		send(sockfd, message, 6, 0);

		//////////////////////////////////////////////
	}
	close(ser_fd);   //关闭文件描述符
	pthread_exit(NULL);
}

int uart_open(void)
{
	ser_fd = open(DEVICE_NAME,O_RDWR);
	if(ser_fd == -1)
	{
		printf("serical open failed!\n");
		return -1;
	}
	set_serial_uart1(ser_fd);
}

void show_dht11(void)
{
	//T??H??
	wchar_t wendu[18];
	wchar_t shidu[18];

	lcd_init();

	//printf("温度%c", message[1]);
	//printf("%c，\t", message[2]);
	swprintf(wendu, 10, L"温度%c%c", message[1], message[2]);
	//printf("%s\n", wendu);
	


	//printf("湿度:%c", message[4]);
	//printf("%c\n", message[5]);
	swprintf(shidu, 10, L"湿度%c%c", message[4], message[5]);

	lcd_draw_jpg(0, 0, "../image/hands_control.jpg", NULL, 0, 0);
	show_hanzi(wendu,32,0xffffff, 1,200,200);
	show_hanzi(shidu,32,0xffffff, 1,200,300);
	
	lcd_uninit();
	
}

void show_time(void)
{
		time_t curtime;
		int i;
		//定义数组用于存放转换后的字符串时间
		char mytime[48];
		//	获取时间
		time(&curtime);
		//	将刚才的时间转换成字符串
		memset(mytime, 0, 48);
		strcpy(mytime,ctime(&curtime));//字符串的拷贝

		wchar_t shijian[48];

		lcd_init();

		swprintf(shijian, 48, L"%s",mytime);
		show_hanzi(shijian,24,0xffffff, 1, 500, 20);

		//strcpy(mytime,ctime(&curtime));//字符串的拷贝

		/*printf("目前时间是:%s\n",mytime);

		for(i=0; i<strlen(mytime); i++)

			printf("%c",mytime[i]);*/
		lcd_uninit();
}

void ctl_zigbee(char *cmd)
{
	write(ser_fd,cmd,2);
	tcflush(ser_fd, TCIOFLUSH); //刷新缓冲区
}

/*void show_cntldht11()
{
	uart_open();
	pthread_t dht11_pid;
	

	pthread_create(&pid, NULL,dht11_thread,0);    //开启温湿度监控
	char cmd[10]={0};
	printf("please input cmd \n");
	while(1)
	{

		scanf("%s",cmd);
		ctl_zigbee(cmd);
	}
	pthread_join(pid,NULL);
	return 0;
}*/


//开启温湿度监控

void dht11_start(void)
{
	/*  int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);*/

	if(dht11_pid == 0)
	{

		//建立与客户端的连接接
		char buf[50];
		int size;
		struct sockaddr_in saddr;
 
		size = sizeof(struct sockaddr_in);
 
		bzero(&saddr,sizeof(saddr));
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(8888); //服务器绑定的端口
		saddr.sin_addr.s_addr = inet_addr("192.168.0.100");//服务器的IP地址
	 
		sockfd = socket(AF_INET,SOCK_STREAM,0); //创建一个套接字用于连接服务器，并且这个套接字可用作对服务器操作的文件描述符。
												//存储服务器的信息，返回-1失败	
		connect(sockfd,(struct sockaddr*)&saddr,size);//返回-1失败

		pthread_create(&dht11_pid, NULL, dht11_thread, 0);		//开启温湿度监控
	}
	else
	{
		return;
	}
}


void dht11_finish(void)
{
	//int pthread_kill(pthread_t threadId,int signal); 
	if(dht11_pid != 0)
	{
		pthread_cancel(dht11_pid);
		pthread_join(dht11_pid, NULL);
		dht11_pid = 0;

		close(sockfd);
		sockfd = 0;

	}
	else
	{
		return;
	}
	
}

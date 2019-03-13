#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

int set_serial_uart1(int ser_fd)
{

	struct termios termios_new;
	bzero( &termios_new, sizeof(termios_new));
	/*原始模式  RAW模式*/
	cfmakeraw(&termios_new);

	
	/*波特率为115200*/
	termios_new.c_cflag=(B115200);
	termios_new.c_cflag |= CLOCAL | CREAD;
	
	/*8位数据位*/
	termios_new.c_cflag &= ~CSIZE;
	termios_new.c_cflag |= CS8;

	/*无奇偶校验位*/
	termios_new.c_cflag &= ~PARENB;

	/*1位停止位*/
	termios_new.c_cflag &= ~CSTOPB;
	/*清除串口缓冲区*/
	tcflush( ser_fd,TCIOFLUSH);
	termios_new.c_cc[VTIME] = 0;	//读取不等待
	termios_new.c_cc[VMIN] = 4;	//读取的最少字符
	tcflush ( ser_fd, TCIOFLUSH);
	/*串口设置使能*/
	tcsetattr( ser_fd ,TCSANOW,&termios_new);
}

void serical_ioctl(int ser_fd)
{
	if(fcntl(ser_fd,F_SETFL,0)<0)	//判断是否为阻塞状态
	{
		exit(1);
	}
	tcdrain(ser_fd);   //通用终端控制
	tcflush( ser_fd, TCIOFLUSH); //刷新缓冲区
}



#include "ts.h"

int g_fd = -1;
struct input_event ts;

void get_xy(int *x, int *y);

int open_ts(void)
{ 
	// 打开触摸屏
    if(( g_fd = open("/dev/input/event0", O_RDWR)) == -1)
	{
		printf("open fail!\n");
		
		return -1;
	}
	

    return 0;
}

void get_xy(int *x, int *y)
{
	int count = 0;
	
	while(1)
	{
		read(g_fd, &ts, sizeof(ts));
	
		if(ts.type == EV_ABS) // 确保是触摸屏
		{
			if(ts.code == ABS_X) // X坐标
			{
				*x = ts.value;
				
				count++;
				
				//printf("x=%d\n", ts.value);
			}
			
			if(ts.code == ABS_Y) // X坐标
			{
				*y = ts.value;
				
				count++;
			}
			
			if(count == 2)
			{
				count = 0;
				
				break;
			}
			
		}
	}
	
}

void close_ts(void)
{
	close(g_fd);
}



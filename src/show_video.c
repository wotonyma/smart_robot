#include "main.h"
#include "lcd.h"
#include "ui.h"
#include <stdio.h>
#include <string.h>
#include "show_video.h"
#include "main.h"
#include "lcd.h"
#include "ui.h"
#include "scandir.h"

#define  VIDEO_DIR  "../video/"

void show_video(void)
{

	//1进入存放video的路径
	chdir(VIDEO_DIR);
	//2扫描指定的video
	int num = searchvideo(VIDEO_DIR);
	printf("search video file  num=%d\n", num);
	//默认显示第一张界面
	//lcd_draw_jpg(0, 0, "../image/video.h", NULL, 0, 0);
	//char chrmusic[50] = namelist[0]->d_name;
	int i = 0;
	char startvideo[200] = "../bin/mplayer -slave -quiet -input file=/pipe  -geometry  0:0 -zoom -x 640 -y 480 ";

	while(1)
	{
		if (player_flag == 6) 
		{
			printf("返回\n");
			chdir("../bin/");
			//show_home();
			player_flag = -1;
			free(namelist);
			break;
		}
		else if (player_flag == 1)
		{

			i--;
			if(i < 0)
				i=0;
			//lcd_draw_jpg(0, 0, "../image/albumback", NULL, 0, 0);
			//lcd_draw_jpg(0, 0, namelist[i]->d_name, NULL, 0, 0);
			system("killall -9 mplayer");
			printf("上一部  i=%d\n", i);
			player_flag = -1;
		}
		else if (player_flag == 2)
		{
			
			//lcd_draw_jpg(0, 0, namelist[i]->d_name, NULL, 0, 0);
			//lcd_draw_jpg(0, 0, "../image/albumback", NULL, 0, 0);
			strcat(startvideo, namelist[i]->d_name);
			strcat(startvideo, " &");
			printf("shell = %s\n",startvideo);
			system(startvideo);
			printf("播放 i=%d\n" ,i);
			player_flag = -1;
		}
		else if (player_flag == 3)
		{
			system("killall -STOP mplayer &");
			//lcd_draw_jpg(0, 0, "../image/albumback", NULL, 0, 0);
			//lcd_draw_jpg(0, 0, namelist[i]->d_name, NULL, 0, 0);

			printf("暂停  i=%d\n", i);
			player_flag = -1;
		}
		else if (player_flag == 4)
		{
			system("killall -CONT mplayer &");
			//lcd_draw_jpg(0, 0, "../image/albumback", NULL, 0, 0);
			//lcd_draw_jpg(0, 0, namelist[i]->d_name, NULL, 0, 0);

			printf("继续  i=%d\n", i);
			player_flag = -1;
		}
		else if (player_flag == 5)
		{
			system("killall -9 mplayer &");
			i++;
			if(i>=num)
				i=0;
			
			//lcd_draw_jpg(0, 0, "../image/albumback", NULL, 0, 0);
			//lcd_draw_jpg(0, 0, namelist[i]->d_name, NULL, 0, 0);

			printf("下一部  i=%d\n", i);
			player_flag = -1;
		}

		memset(startvideo, '\0', sizeof(startvideo));
		//startmusic[50] = "madplay ";字符串不可以直接赋值
		strcpy(startvideo, "../bin/mplayer -slave -quiet -input file=/pipe  -geometry  0:0 -zoom -x 640 -y 480 ");

	}
}

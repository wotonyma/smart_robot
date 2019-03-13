#include "main.h"
#include "lcd.h"
#include "ui.h"
#include <stdio.h>
#include <string.h>
#include "show_music.h"
#include "main.h"
#include "lcd.h"
#include "ui.h"
#include "scandir.h"

#define  MUSIC_DIR  "../music/"

void show_music(void)
{

	//1进入存放music的路径
	chdir(MUSIC_DIR);
	//2扫描指定的mp3  
	int num = searchmusic(MUSIC_DIR);
	printf("search music file  num=%d\n", num);
	//默认显示第一张图片
	//lcd_draw_jpg(0, 0,namelist[0]->d_name , NULL, 0, 0);
	//char chrmusic[50] = namelist[0]->d_name;
	int i = 0;
	char startmusic[50] = "madplay ";

	while(1)
	{
		if (player_flag == 7) 
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

			system("killall -9 madplay"); 
			i++;
			if(i>=num)
				i=0;

			//lcd_draw_jpg(0, 0, "../image/albumback", NULL, 0, 0);
			//lcd_draw_jpg(0, 0, namelist[i]->d_name, NULL, 0, 0);
			

			printf("上一首  i=%d\n", i);
			player_flag = -1;
		}
		else if (player_flag == 2)
		{
			
			//lcd_draw_jpg(0, 0, namelist[i]->d_name, NULL, 0, 0);
			//lcd_draw_jpg(0, 0, "../image/albumback", NULL, 0, 0);
			strcat(startmusic, namelist[i]->d_name);
			strcat(startmusic, " &");
			system(startmusic);
			printf("播放 i=%d\n" ,i);
			player_flag = -1;
		}
		else if (player_flag == 3)
		{
			system("killall -STOP madplay");

			//lcd_draw_jpg(0, 0, "../image/albumback", NULL, 0, 0);
			//lcd_draw_jpg(0, 0, namelist[i]->d_name, NULL, 0, 0);

			printf("暂停  i=%d\n", i);
			player_flag = -1;
		}
		else if (player_flag == 4)
		{
			system("killall -CONT madplay"); //恢复

			//lcd_draw_jpg(0, 0, "../image/albumback", NULL, 0, 0);
			//lcd_draw_jpg(0, 0, namelist[i]->d_name, NULL, 0, 0);

			printf("恢复  i=%d\n", i);
			player_flag = -1;
		}
		else if(player_flag == 5)
		{
			system("killall -9 madplay"); //关闭
			printf("关闭  i=%d\n", i);
			player_flag = -1;
		}
		else if(player_flag == 6)
		{
			system("killall -9 madplay"); //关闭
			i--;
			if (i<0)
				i=num-1;
			
			//lcd_draw_jpg(0, 0, namelist[i]->d_name, NULL, 0, 0);
			//lcd_draw_jpg(0, 0, "../image/albumback", NULL, 0, 0);
			printf("下一首 i=%d\n" ,i);
			player_flag = -1;
		}

		memset(startmusic, '\0', sizeof(startmusic));
		//startmusic[50] = "madplay ";字符串不可以直接赋值
		strcpy(startmusic, "madplay ");

	}
}

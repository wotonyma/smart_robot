#include "show_photo.h"
#include "main.h"
#include "lcd.h"
#include "ui.h"
#include "scandir.h"

#define  PIC_DIR  "../pic/"

void show_photo(void)
{

	//1进入存放照片的路径
	chdir(PIC_DIR);
	//2扫描指定的jpg  
	int num = search(PIC_DIR);
	printf("search jpg file  num=%d\n", num);
	//默认显示第一张图片
	lcd_draw_jpg(0, 0,namelist[0]->d_name , NULL, 0, 0);
	int i = 0;
	while(1)
	{
		if (photo_flag == 0) 
		{
			printf("返回\n");
			chdir("../bin/");
			//show_home();
			free(namelist);
			photo_flag = -1;
			break;
		}
		if (photo_flag == 1)
		{
			i++;
			if(i>=num)
				i=0;
			lcd_draw_jpg(0, 0, "../image/albumback", NULL, 0, 0);
			lcd_draw_jpg(0, 0, namelist[i]->d_name, NULL, 0, 0);

			printf("下一张  i=%d\n", i);
			photo_flag = -1;
		}
		if (photo_flag == 2)
		{
			i--;
			if (i<0)
				i=num-1;
			
			lcd_draw_jpg(0, 0, namelist[i]->d_name, NULL, 0, 0);
			lcd_draw_jpg(0, 0, "../image/albumback", NULL, 0, 0);
			printf("上一张 i=%d\n" ,i);
			photo_flag = -1;
		}

	}
}

#include <stdio.h>
#include <pthread.h>
#include "lcd.h"
#include "ts.h"
#include "ui.h"
#include "show_photo.h"
#include "show_music.h"
#include "show_video.h"
#include "show_voice.h"
#include "show_capture.h"
#include "show_cntldht11.h"
#include "main.h"

/**
mode_flag：表示界面窗口标志位
参数说明：
	0 主界面
	1 相机界面
	2 播放器界面
	3 语音控制界面
	4 手动控制界面
**/

/**
pic_flag: 表示相机界面标志位
参数说明：
	0 显示相机界面
	1 点击拍照
	2 点击摄像
	3 点击查看照片
**/

/**
player_flag: 表示播放器界面标志位
参数说明：
	0 显示播放器界面
	1 进入音乐播放器界面
	2 进入视频播放器界面
**/

/**
voice_flag: 表示语音控制界面标志位
参数说明：
	0 显示语音控制界面
	1 录音
**/

/**
control_flag: 表示手动控制界面标志位
参数说明：
	0 显示手动控制界面
	1 显示温度和温度
**/


int pic_flag = 0; 
int photo_flag = 0;

int player_flag = 0;

int voice_flag = 0;
int control_flag = 0;

int mode_flag = 0; // 界面窗口标志位

void *get_xy_pthread(void *arg)
{
	int x, y;
	while(1)
	{
		get_xy(&x, &y);
	
		printf("x = %d\ty = %d\n", x, y);
		
		if(mode_flag == 0)
		{
			if(x>0 && x<200)
			{
				if(y>0 && y<120)
				{
					
					mode_flag = 1; // 进入相机界面（刷图）
					
				}
			}
			else if(x>200 && x<400)
			{
				if(y>120 && y<240)
				{
					
					mode_flag = 2; // 进入播放器界面（刷图）
					
				}
			}
			else if(x>400 && x<600)
			{
				if(y>240 && y<360)
				{
					
					mode_flag = 3; // 进入语音控制界面（刷图）
					
				}
			}
			else if(x>600 && x<800)
			{
				if(y>360 && y<480)
				{
					
					mode_flag = 4; // 进入播放器界面（刷图）
					
				}
			}
		}
		//相机界面
		else if(mode_flag == 1 && pic_flag == 0)
		{
			if(x>250 && x<550 && y>180 && y<280)
			{
				pic_flag = 1;	//拍照
			}
			else if(x>250 && x<550 &&y>280 && y<380)
			{
				pic_flag = 2;	//监控	
			}
			else if(x>250 && x<550 && y>380 && y<480)
			{
				pic_flag = 3;   //相册
				printf("pic_flag = %d\n",pic_flag);
			}
			else if(x>650 && x<750 && y>400 && y<450)
			{
				mode_flag = 0;	//退出
			}
		}
		/*else if((mode_flag == 1) && (pic_flag == 2))	//照相
		{
			if(xy)
			{
				player_flag = 1;
			}
			else if()
			{
				player_flag = 2;
			}
			else if()
			{
				player_flag = 3;
			}
			else if()
			{
				player_flag = 4;
			}
		}*/
		else if((mode_flag == 1) && (pic_flag == 2))	//监控
		{
			if(x > 0 && x < 80 && y >0 && y < 240)
			{
				player_flag = 1;
			}
			else if(x > 720 && x < 800 && y >0 && y < 240)
			{
				player_flag = 2;
			}
			else if(x > 0 && x < 80 && y >240 && y < 480)
			{
				player_flag = 3;
			}
			else if(x > 720 && x < 800 && y >240 && y < 480)
			{
				player_flag = 4;
			}
		}
		else if((mode_flag == 1) && (pic_flag == 3))	//相册
		{
			printf("pic_flag = %d\n",pic_flag);

			if ((x>100 && x<300) && (y>160 && y<320))  
			{
				photo_flag = 1;   //上一张
				printf("pic_flag = %d\n",pic_flag);
			}
			else if ((x>450&& x<650) && (y>160 && y<320))  
			{
				photo_flag = 2;   //下一张
				printf("pic_flag = %d\n",pic_flag);
			}
			else if ((x>650 && x<750) && (y>400 && y<450))  
			{
				photo_flag = 0;
				pic_flag = 0;   //返回相机界面
				mode_flag = 1;
				printf("pic_flag = %d\n",pic_flag);
			}
		}
		//播放器界面//////////////////////////////////////////////////////////////////////////////
		else if(mode_flag == 2 && pic_flag == 0)
		{
			if((x > 250 && x < 550) && (y>200 && y<300))
			{
				pic_flag = 1; //音乐播放器
			}
			else if((x > 250 && x < 550) && (y>300 && y<400))
			{
				pic_flag = 2;//视频播放器
			}
			else if(x>650 && x<750 && y>400 && y<450)
			{
				
				   //返回主界面
				mode_flag = 0;
				printf("pic_flag = %d\n",pic_flag);
			}
		}
		//音乐播放器
		else if(mode_flag == 2 && pic_flag == 1)
		{
			if((x > 137 && x < 265) && (y > 195 && y < 297))//按键
			{
					
				player_flag = 1; // 上一首
				printf("上一首 player_flag = %d\n",player_flag);
					
			}
			else if((x > 447 && x < 536 )&& (y > 84 && y < 187))//按键
			{
					
				player_flag = 2; // 播放
				printf("播放 player_flag = %d\n",player_flag);
					
			}
			
			else if((x > 312 && x < 402) && (y > 200 && y < 300))//按键
			{
					
				player_flag = 3; // 暂停
				printf(" 暂停player_flag = %d\n",player_flag);
			}
			else if((x > 446 && x < 534) && (y > 201 && y < 303))//按键
			{
					
				player_flag = 4; // 恢复
				printf("恢复player_flag = %d\n",player_flag);
			}
			else if((x > 308 && x < 402) && (y > 314 && y < 405))//按键
			{
					
				player_flag = 5; // 关闭
				printf("关闭player_flag = %d\n",player_flag);
			}
			else if((x > 579 && x < 703) && (y > 201 && y < 303))//按键
			{
					
				player_flag = 6; // 下一首
				printf("下一首player_flag = %d\n",player_flag);
			}
			else if((x>650 && x<750) && (y>400 && y<450))//按键
			{
					
				player_flag = 7;
				pic_flag = 0;   //返回播放器界面
				mode_flag = 2;
				printf("退出pic_flag = %d\n",pic_flag);// 退出
			}
		}
		//视频播放器
		else if(mode_flag == 2 && pic_flag == 2)
		{
			if((x > 640 && x < 800) && (y > 0 && y < 80))//按键
			{
					
				player_flag = 1; // 上一个视频
				printf("上一首 player_flag = %d\n",player_flag);
					
			}
			else if((x > 640 && x < 800 )&& (y > 80 && y < 160))//按键
			{
					
				player_flag = 2; // 播放视频
				printf("播放视频 player_flag = %d\n",player_flag);
					
			}
			
			else if((x > 640 && x < 800) && (y > 160 && y < 240))//按键
			{
					
				player_flag = 3; // 暂停
				printf(" 暂停player_flag = %d\n",player_flag);
			}
			
			else if((x > 640 && x < 800) && (y > 240 && y < 320))//按键
			{
					
				player_flag = 4; // 恢复
				printf("恢复player_flag = %d\n",player_flag);
			}
			else if((x>640 && x<800) && (y>320 && y<400))//按键
			{
					
				player_flag = 5;
				printf("下一个player_flag = %d\n",player_flag);
			}
			else if((x>640 && x<800) && (y>400 && y<480))//按键
			{
					
				player_flag = 6;
				pic_flag = 0;   //返回播放器界面
				mode_flag = 2;
				printf("退出player_flag = %d\n",player_flag);// 退出
			}
		}
		//vedio播放

		//语音控制界面//////////////////////////////////////////////////////////////////////////////
		else if(mode_flag == 3 && pic_flag == 0)
		{
			if(x>250 && x<500)
			{
				if(y>240 && y<320)
				{
					
					pic_flag = 1; // 点击录音
					
				}
			}
			if(x>650 && x<750)
			{
				if(y>400 && y<450)
				{
					
					//mode_flag = 0;这个改变不应该是这个线程的功能
					pic_flag = 2; // 返回主界面
					
				}
			}
		}
		//手动控制界面////////////////////////////////////////////////////////////////////////////////
		else if(mode_flag == 4)
		{
			if(x>250 && x<500)
			{
				if(y>240 && y<320)
				{
					
					pic_flag = 1; //点击开始显示温度和湿度
					
				}
			}
			if(x < 100 && x > 0 && y < 60 && y > 0)
			{
				pic_flag = 2;    //终止线程
			}
			if(x>650 && x<750)
			{
				if(y>400 && y<450)
				{
					
					mode_flag = 0; // 返回主界面
					pic_flag = 0;
					
				}
			}
		}
	
	}
}

int main(void)
{
	
	
	// 调用触摸屏函数
	open_ts();
	
	// 打开触摸屏
	lcd_open("/dev/fb0");
	
	pthread_t tid;
	pthread_create(&tid, NULL, get_xy_pthread, NULL);
	show_home();
	
	while(1)
	{
		
		if(mode_flag == 0)
		{
			show_home();
		}
		else if(mode_flag == 1)
		{
			// 显示相机界面
			if(pic_flag == 0)
			{
				lcd_draw_jpg(0, 0, "../image/camera.jpg", NULL, 0, 0);  //model1的主界面
			}
			else if(pic_flag == 1)
			{
				//显示照相界面
				lcd_draw_jpg(0, 0, "/1.jpg", NULL, 0, 0);
			}
			else if(pic_flag == 2)
			{
				//显示摄像界面
				lcd_draw_jpg(0, 0, "../image/capture.jpg", NULL, 0, 0);
				show_capture();
			}
			else if(pic_flag == 3)
			{
				//显示相册界面
				printf("pic_flag = %d\n", photo_flag);
				show_photo();
			}
			
		}
		else if(mode_flag == 2)
		{
			// 显示播放器界面
			if(pic_flag == 0)
			{
				lcd_draw_jpg(0, 0, "../image/player.jpg", NULL, 0, 0);
			}
			else if(pic_flag == 1)
			{
				//显示音乐播放器界面
				lcd_draw_jpg(0, 0, "../image/music.jpg", NULL, 0, 0);
				show_music();
			}
			else if(pic_flag == 2)
			{
				//显示视频播放器界面
				lcd_draw_jpg(0, 0, "../image/video.jpg", NULL, 0, 0);
				show_video();
			}
		}
		//语音识别
		else if(mode_flag == 3)
		{
			// 显示语音控制界面
			if(pic_flag == 0)
			{
				lcd_draw_jpg(0, 0, "../image/voice_control.jpg", NULL, 0, 0);
			}
			else if(pic_flag == 1)
			{
				//开始录音
				//lcd_draw_jpg(0, 0, "/1.jpg", NULL, 0, 0);
				printf("show_voice\n");
				show_voice();
			}
			else if(pic_flag == 2)
			{
				mode_flag = 0;
				pic_flag = 0;
			}
		}
		/////////////////////////////////////////////////////////////////
		else if(mode_flag == 4)
		{
			// 显示手动控制界面
			if(pic_flag == 0)
			{
				lcd_draw_jpg(0, 0, "../image/hands_control.jpg", NULL, 0, 0);
				//show_time();
			}
			else if(pic_flag == 1)
			{
				//显示温度湿度
				//lcd_draw_jpg(0, 0, "/1.jpg", NULL, 0, 0);
				dht11_start();
				//show_time();
				//printf("dht11_start!!\n");
				
			}
			else if(pic_flag == 2)
			{
				dht11_finish();
				printf("dht11_finish!!!\n");
				lcd_draw_jpg(0, 0, "../image/hands_control.jpg", NULL, 0, 0);
				//show_time();
				pic_flag == 0;
			}
		}

		
		
	}
	
	
	
	close_ts();
	close_lcd();
	
	return 0;
}

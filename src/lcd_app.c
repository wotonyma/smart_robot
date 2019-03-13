// lcd_test.c 
#include "lcd_app.h"





int fb_fd;
int hzk_fd;
struct stat hzk_stat;
unsigned char *hzk_mem;

unsigned char *fb_mem;
struct fb_var_screeninfo var;	/* Current var */
struct fb_fix_screeninfo fix;	/* Current fix */

int x_res, y_res;
int line_length;
int screen_size;
int pixel_length;


// freetype relattived parameters
FT_Library    library;
FT_Face       face;
FT_GlyphSlot  slot;
FT_Matrix     matrix;                 /* transformation matrix */
FT_Vector     pen;                    /* untransformed origin  */
FT_Error      error;

//
unsigned int  n ;


void lcd_init(void)
{
	int ret;
	/* 打开 framebuffer */
	fb_fd = open("/dev/fb0" , O_RDWR);
	if(fb_fd < 0)
	{
		printf("neo: cannot open the fb device\n");
		return ;
	}

	/*获得固定参数 和 变化参数 */
	ret = ioctl(fb_fd , FBIOGET_VSCREENINFO , &var);
	if(ret)
	{
		printf("neo: get FBIOGET_VSCREENINFO args error");
		return  ;
	}
	
	ret = ioctl(fb_fd , FBIOGET_FSCREENINFO , &fix);
	if(ret)
	{
		printf("neo: get FBIOGET_FSCREENINFO args error");
		return  ;
	}

	line_length	 =  fix.line_length;
	screen_size  =  fix.smem_len;
	pixel_length =  var.bits_per_pixel / 8;
	x_res = var.xres;
	y_res = var.yres;
	
	
	printf("fix.line_length 	= %d\n",fix.line_length);
	printf("fix.smem_len 		= %d\n",fix.smem_len);
	printf("var.bits_per_pixel 	= %d\n",var.bits_per_pixel);
	printf("var.xres = %d\n",var.xres);
	printf("var.yres = %d\n",var.yres);
	printf("screen_size = %d\n",screen_size); //3072000  800*960
	/* 映射 framebuffer 地址 */
    fb_mem =  (unsigned char*)mmap(NULL, screen_size,PROT_READ | PROT_WRITE,MAP_SHARED,fb_fd, 0);
	if(fb_mem == MAP_FAILED)
	{
		printf("mmap failure!\n");
		return;
	}	

}

void lcd_uninit(void)
{
	close(fb_fd);

	munmap(fb_mem,screen_size);
}
 

void lcd_clear_display(unsigned int c)
{
	memset(fb_mem , 0 , screen_size);
}
void show_put_pixel(int x ,int y , unsigned int color)
{
	unsigned int *pen_32 = (unsigned int *)(fb_mem + y*line_length + x*pixel_length);
	if(var.bits_per_pixel != 32)
	{
		printf(" sorry ! only support 32 bit\n");
		return ;
	}
	*pen_32 = color ;
}

void lcd_put_line(int x1, int y1 , int x2, int y2 , unsigned int color)
{
	int dx,dy,e;
	dx=x2-x1; 
	dy=y2-y1;
    
	if(dx>=0)
	{
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 1/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					show_put_pixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else		// 2/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					show_put_pixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 8/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					show_put_pixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else		// 7/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					show_put_pixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
	else //dx<0
	{
		dx=-dx;		//dx=abs(dx)
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 4/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					show_put_pixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 3/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					show_put_pixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 5/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					show_put_pixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 6/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					show_put_pixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
}
void lcd_put_ascii(int x, int y , unsigned char c)
{
	int i,j;
	unsigned char byte;
	// 获得点阵的起始坐标
	unsigned char *dots = (unsigned char*)&fontdata_8x16[c*16] ;
	printf("dots \n");
	for(i=0 ; i<16; i++)
	{
		byte = dots[i];
		for(j=7;j>=0;j--)
		{
			if(byte & (1<<j))
				show_put_pixel(x+(7-j),y+i,0xffffff);
			else
			{
				show_put_pixel(x+(7-j),y+i,0);  //显示字符时的底色
			}
		}
	}
}
void lcd_put_gbk(int x, int y, unsigned char *str )
{
	int i, j,k ;
	unsigned char byte;
	// 根据汉字库找到 汉字 起始地址
	unsigned int area  = str[0] - 0xA1;  // hzk16 点阵字体文件是从0xA1 0xA1 开始的
	unsigned int where = str[1] - 0xA1;
	unsigned char *dots = (hzk_mem + (area*94 + where)*32);
	
	for(i=0; i< 16 ;i++)
	{
		for(j=0 ; j<2 ;j++)
		{
			byte = dots[i*2+j*1];  // 逐个处理每个字节
			for(k=7 ; k>=0; k--)
			{
				if(byte & (1<<k))
				{
					show_put_pixel(x+(7-k)+j*8,y+i,0xffffff);
				}
				else
				{
					show_put_pixel(x+(7-k)+j*8,y+i,0);				
				}
			}
		}	
	}
}

void draw_bitmap(int color,int flag, FT_Bitmap*  bitmap, FT_Int  x, FT_Int y)
{
	FT_Int  i, j, p, q;
	FT_Int  x_max = x + bitmap->width;
	FT_Int  y_max = y + bitmap->rows;


	for ( i = x, p = 0; i < x_max; i++, p++ )
	{
		for ( j = y, q = 0; j < y_max; j++, q++ )
		{
			if ( i < 0      || j < 0       ||
			   i >=x_res   || j >= x_res )
			continue;

			if(bitmap->buffer[q * bitmap->width + p] )
				show_put_pixel(x+p , y+q ,  color);
			else
			{
				if(flag == 0)
				show_put_pixel(x+p , y+q ,  0);  //显示底色
				else
				{
					//空，则无底色
				}
			}
			//     image[j][i] |= bitmap->buffer[q * bitmap->width + p];
		}
	}
}



void show_bmp(const char *pathname,int start_x,int start_y)
{
	int bmp_fd,i,x,y;
	unsigned short heigh,weight;
	unsigned char head_buff[54];//存储头信息的buff
	//打开屏幕
	bmp_fd = open(pathname,O_RDWR);
	if(bmp_fd == -1)
	{
		printf("open bmp failure!\n");
		return ;
	}
	
	read(bmp_fd,head_buff,54);
	//得到图片宽度
	weight = head_buff[19]<<8 | head_buff[18];
	
	//得到图片高度
	heigh = head_buff[23]<<8 | head_buff[22];
	
	
	if((weight+start_x) > 800)
	{
		printf("bmp weight+start_x big 800\n");
		return ;
	}
	
	if((heigh+start_y) > 480)
	{
		printf("bmp heigh+start_y big 800\n");
		return ;
	}
	
	
	printf("weight:%d,heigh:%d\n",weight,heigh);
	
	//定义用于存放图片rgb的buff
	unsigned char bmp_buff[weight*heigh*3];
	unsigned char buff[weight*heigh*3];

	
	//将图片像素读到buffer当中
	read(bmp_fd,bmp_buff,sizeof(bmp_buff));	
	
	
	
	for(y=0; y<heigh; y++)
	{
		for(x=0; x<weight*3; x++)
		{
			buff[y*weight*3+x] = bmp_buff[(heigh - 1 -y)*weight*3+x];
		}
	}
	
	
	for(y=0; y<heigh; y++)
	{
		for(x=0; x<weight; x++)
		{
			*(fb_mem+(start_y + y)*800*4+	start_x*4 +4*x+0)= buff[y*weight*3+3*x+0];
			*(fb_mem+(start_y + y)*800*4+	start_x*4 +4*x+1)= buff[y*weight*3+3*x+1];
			*(fb_mem+(start_y + y)*800*4+	start_x*4 +4*x+2)= buff[y*weight*3+3*x+2];
			*(fb_mem+(start_y + y)*800*4+	start_x*4 +4*x+3)= 0x00;
			
		}
		
	}	
	
	
	
	close(bmp_fd);
}



void show_hanzi(wchar_t *wtext,int size,int color, int flag,int start_x,int start_y)
{
	int ret;
	/* 显示汉字  中国*/	
	hzk_fd  = open("HZK16" , O_RDWR); // 打开汉字库
	ret = fstat(hzk_fd, &hzk_stat); // 获得汉字库大小
	if(ret)
	{
		printf("can't  open the hanziku\n");
	}
	//hzk_mem =  (unsigned char*)mmap(NULL, hzk_stat.st_size,PROT_READ | PROT_WRITE,MAP_SHARED,hzk_fd, 0);

	//printf("打印出 中国的 GBk code \n");
	//printf("GBK code :  %x , %x  , %x ,%x \n" , str1[0] , str1[1] ,str2[0] , str2[1] );
	//printf("GBK code :  %x , %x  , %x ,%x \n" , str3[0] , str3[1] ,str4[0] , str4[1] );	
	//lcd_put_gbk(250 ,200 , str1); // 此时中字采用 gbk码 保存的 所以为 d6 d0
	//lcd_put_gbk(500 ,200 , str2);

	//while(1);


	
	/*显示宋体中国*/
	error = FT_Init_FreeType( &library );              /* initialize library */

	error = FT_New_Face( library, "./simsun.ttc", 0, &face ); /* create face object    这个打开字体文件 */
	slot = face->glyph;

	error = FT_Set_Pixel_Sizes(face, size, 0);    /* set font size */

	/* the pen position in 26.6 cartesian space coordinates; */
	/* start at (300,200) relative to the upper left corner  */
	pen.x = start_x * 64;
	pen.y = ( y_res - start_y ) * 64;

	for ( n = 0; n < wcslen( wtext ); n++ )
	{
		/* set transformation */
		FT_Set_Transform( face, 0, &pen );

		/* load glyph image into the slot (erase previous one) */
		error = FT_Load_Char( face, wtext[n], FT_LOAD_RENDER );
		if ( error )
		  continue;                 /* ignore errors */

		/* now, draw to our target surface (convert position) */
		draw_bitmap(color,flag, &slot->bitmap,
		             slot->bitmap_left,
		             y_res - slot->bitmap_top );

		/* increment pen position */
		pen.x += slot->advance.x;
	}	
	
	close(hzk_fd);
}







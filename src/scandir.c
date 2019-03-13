
//所要达到的目的: 我想使用scandir来实现筛选出 .c类型的文件
#include "scandir.h"
struct dirent **namelist;
//自定义筛选回调函数
int jpgfilter(const struct dirent *filename) 
{
	
    size_t len;
 
    len = strlen(filename->d_name); //11.jpg 
    if (len >4
    	&& filename->d_name[len - 4] == '.'
    	&& filename->d_name[len - 3] == 'j'
    	&& filename->d_name[len - 2] == 'p'
        && filename->d_name[len - 1] == 'g')
        return 1;
 
    return 0;

}

int search(const char * dirname)
{
	
	int n,count;
	n=scandir(dirname,&namelist,jpgfilter,alphasort);
	count = n;
	printf("n=%d\n",n);
	while(n--)
	{
		printf("namelist[%d] %s\n",n,namelist[n]->d_name);
	}

	return count;
}


int musicfilter(const struct dirent *filename) 
{
	
    size_t len;
 
    len = strlen(filename->d_name); //11.jpg 
    if (len >4
    	&& filename->d_name[len - 4] == '.'
    	&& filename->d_name[len - 3] == 'm'
    	&& filename->d_name[len - 2] == 'p'
        && filename->d_name[len - 1] == '3')
        return 1;
 
    return 0;

}

int searchmusic(const char * dirname)
{
	
	int n,count;
	n=scandir(dirname,&namelist,musicfilter,alphasort);
	count = n;
	printf("n=%d\n",n);
	while(n--)
	{
		printf("namelist[%d] %s\n",n,namelist[n]->d_name);
	}

	return count;
}

int videofilter(const struct dirent *filename) 
{
	
    size_t len;
 
    len = strlen(filename->d_name); //11.jpg 
    if (len >4
    	&& filename->d_name[len - 4] == '.'
    	&& filename->d_name[len - 3] == 'a'
    	&& filename->d_name[len - 2] == 'v'
        && filename->d_name[len - 1] == 'i')
        return 1;
 
    return 0;

}

int searchvideo(const char * dirname)
{
	
	int n,count;
	n=scandir(dirname,&namelist,videofilter,alphasort);
	count = n;
	printf("n=%d\n",n);
	while(n--)
	{
		printf("namelist[%d] %s\n",n,namelist[n]->d_name);
	}

	return count;
}
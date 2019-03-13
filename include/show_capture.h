#ifndef _CAPTURE_
#define _CAPTURE_

/*void quit(const char * msg);

int xioctl(int fd, int request, void * arg);

void camera_check(camera_t * camera);

camera_t * camera_open(const char * device, uint32_t width, uint32_t height);

void camera_init(camera_t * camera);

void camera_start(camera_t * camera);

void camera_stop(camera_t * camera);

void camera_close(camera_t * camera);

int camera_capture(camera_t * camera);

int camera_frame(camera_t * camera, struct timeval timeout);

int minmax(int min, int v, int max);

uint8_t * yuyv2rgb2(uint8_t * yuyv, uint32_t width, uint32_t height);

int yuyv2rgb(int y, int u, int v);

int yuyv2rgb0(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);

int lcd_init(void);

void lcd_uninit(void);

void show_capture(void);*/
void show_capture(void);

//void jpeg(FILE* dest, uint8_t* rgb, uint32_t width, uint32_t height, int quality);


#endif

#include "main.h"
#include "lcd.h"
#include "ui.h"
#include <stdio.h>
#include <string.h>
#include "show_voice.h"
#include "main.h"
#include "lcd.h"
#include "ui.h"
#include "client_file.h"

// #define  VIDEO_DIR  "../video/"

void show_voice(void)
{
	//开启客户端
	system("arecord -d5 -c1 -r16000 -twav -fS16_LE 11111.pcm ");
	printf("等待5s\n");
	shibie();
}

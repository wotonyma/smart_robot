
#include "socket.h"

//=========文件收发===========
static int socked_fd = -1;
static int con_fd = -1;
int socket_init(){
	printf("start socket!\n");
	//1. 创建socket
	socked_fd = socket(AF_INET,SOCK_STREAM,0);

	//2. 构造绑定的地址
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port =  htons(23333);
	addr.sin_addr.s_addr =inet_addr("192.168.0.100");// inet_addr("127.0.0.1");//这里有坑

	//重复绑定端口号
	int i=1;
	setsockopt(socked_fd,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(i));
	//3. 绑定地址
    int brtv=bind(socked_fd, (struct sockaddr *) &addr,sizeof(struct sockaddr));
	if(brtv == -1)
		printf("bind failed:\n");
	else
		printf("bind success\n");
	//4. 监听
	listen(socked_fd,5);
	return 0;
}

int wait_connect(){
	if(socked_fd == -1){
		socket_init();
	}
	//5. accept
	struct sockaddr addr_recv;
	socklen_t addrlen = 0;
	printf("wait_connect\n");
	con_fd =  accept(socked_fd,&addr_recv,&addrlen);
	printf("client connect\n");
	return 0;
}


int recv_file(char * file_path){
	if(con_fd == -1){
		wait_connect();
	}

	//6. 收文件的大小
	char msg_buff[2048] = {0};
	ssize_t msg_len = 0;
	msg_len = recv(con_fd, msg_buff, 2048, 0);

	//7. 转换收到的文件大小为整形
	int file_size = 0;
	sscanf(msg_buff,"%d",&file_size);
	printf("file_size = %d\n", file_size);

	//8. 开始接收文件内容
	//创建一个文件
	FILE *file_p = fopen(file_path,"w+");

	bzero(msg_buff,sizeof(msg_buff));
	int total_len = 0;
	while((msg_len = recv(con_fd,msg_buff,2048,0)) > 0)
	{
		
		fwrite(msg_buff, sizeof(char),msg_len,file_p);
		total_len += msg_len;
		printf("recv:%d/%d\n", total_len,file_size);
		if(total_len == file_size)
		{
			break;
		}
			
		bzero(msg_buff,sizeof(msg_buff));
		msg_len = 0;
	}

	fclose(file_p);
	return 0;
}


int send_result(char * result){
	if(con_fd == -1){
		wait_connect();
	}
	send(con_fd,result,2048,0);// 可能需要发送定长
	return 0;
}


int close_all(){

	close(con_fd);
	close(socked_fd);
	socked_fd = -1;
	con_fd = -1;
	return 0;
}



//===============数据转发===============
static int socket_data = -1;
static int con_data = -1;
int socket_init_arm_connect(){
	//1. 创建socket
	socket_data = socket(AF_INET,SOCK_STREAM,0);

	//2. 构造绑定的地址
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port =  htons(24444);
	addr.sin_addr.s_addr =INADDR_ANY;// inet_addr("127.0.0.1");//这里有坑

	//3. 绑定地址
    bind(socket_data, (struct sockaddr *) &addr,sizeof(struct sockaddr));

	//4. 监听
	listen(socket_data,5);
	return 0;
}

int wait_arm_connect(){
	if(socket_data == -1){
		socket_init_arm_connect();
	}
	//5. accept
	struct sockaddr addr_recv;
	socklen_t addrlen = 0;
	printf("[data]:wait_connect \n");
	con_data =  accept(socket_data,&addr_recv,&addrlen);
	printf("[data]:client connect\n");
	return 0;
}

int recv_from_arm(char * msg_buff,unsigned int msg_len){
	if(con_data == -1){
		wait_arm_connect();
	}

	//6. 收数据
	ssize_t recv_len = 0;
	recv_len = recv(con_data, msg_buff, msg_len, 0);
	printf("[recv data]:%s\n", msg_buff);//可能越界

	return 0;
}

int send_to_arm(char * data,unsigned int len){
	if(con_data == -1){
		wait_connect();
	}
	send(con_data,data,len,0);
	return 0;
}


int close_arm_connect(){
	close(con_data);
	close(socket_data);
	socket_data = -1;
	con_data = -1;
	return 0;
}

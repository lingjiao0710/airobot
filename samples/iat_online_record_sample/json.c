#include <stdio.h>
#include <string.h>
#include<stdlib.h>

#include "cJSON.h"

#define BUF_SIZE 256
#define MSG_SIZE 2*1024

//记得换成自己的，谢谢！
//#define APP_KEY "43b1aebe26684423896383fcfbe23080"
//#define USER_ID "472793"
//#define APP_KEY "81a0447221504656bff04b47d1a8c868"
//#define USER_ID "363282"



/*
int swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
*/


//读取文件并返回buffer指针
int file_read_buffer(FILE *fp, char *filebuf)
{
	//FILE * fp = NULL;

	char buf[BUF_SIZE] = {0};
/*
	fp = fopen(filename, "r");
	if(fp == NULL){
		printf("open %s failed\n", filename);
		goto out;
	}
	*/
	//printf("fp :%p\n", fp);
/*
	fseek(fp,0,SEEK_END);
	int filesize = ftell(fp);
	if(filesize <= 0){
		printf("文件大小为0\n");
		goto out;
	}

	//申请文件大小+10字节的空间
	printf("filesize  %d\n", filesize);
	char *filebuf = malloc(filesize + 10);
	if(filebuf == NULL){
		printf("filebuf malloc failed\n");
		goto out;
	}
	memset(filebuf, 0, filesize);

	//把文件指针移动到文件首
	fseek(fp,0,SEEK_SET);
	
*/
	//反复读取文件内容，并连接到filebuf中
	while(fgets(buf, BUF_SIZE, fp) != NULL){
		strcat(filebuf, buf);
		//printf("buf:%s\n", buf);
	}

	//printf("filebuf:%s\n", filebuf);



	return 0;

}


int json_file_parse(char *filebuf, char *textresult)
{
	int i = 0;

	//将字符串转换为json并解析
	cJSON *root = cJSON_Parse(filebuf);
	/*
	cJSON *reason = cJSON_GetObjectItem(root, "reason");
	if(reason){
		printf("value:%s\n", reason->valuestring);
	}*/

	cJSON *results = cJSON_GetObjectItem(root, "results");
	if(results){
		//printf("result :%p\n", result);

		//cJSON *data = cJSON_GetObjectItem(results, "data");
		//if(data){
			//printf("data :%p\n", data);
			int size = cJSON_GetArraySize(results);
			for(i = 0; i < size; i++){
				cJSON *item = cJSON_GetArrayItem(results, i);				
				if(item){
					cJSON *values = cJSON_GetObjectItem(item, "values");
					if(values){
						cJSON *text = cJSON_GetObjectItem(values, "text");
						if(text){
							printf("text:%s\n", text->valuestring);
							strcpy(textresult, text->valuestring);
						}
					}
				}
			}
		//}
	}

	cJSON_free(root);

}


char * creat_json_buf(char *text)
{
	cJSON *root = NULL;
	cJSON *perception = NULL;
	cJSON *inputText = NULL;
	cJSON *userInfo = NULL;

	root = cJSON_CreateObject();// {}
	cJSON_AddNumberToObject(root, "reqType", 0);// {"reqType": 0}

	cJSON_AddItemToObject(root, "perception", perception = cJSON_CreateObject());
	cJSON_AddItemToObject(perception, "inputText", inputText = cJSON_CreateObject());
	cJSON_AddStringToObject(inputText, "text", text);

	cJSON_AddItemToObject(root, "userInfo", userInfo = cJSON_CreateObject());
	cJSON_AddStringToObject(userInfo, "apiKey", APP_KEY);
	cJSON_AddStringToObject(userInfo, "userId", USER_ID);


	char *json_out = cJSON_Print(root);
	cJSON_free(root);

	return json_out;
	
}

#if 0
int main2(int argc, char *argv[])
{
	char *json_buf = NULL;
	FILE * fp = NULL;
	char cmd[1024] = {0};
	char filebuf[MSG_SIZE] = {0};

	if(argc != 2){
		printf("使用方式：./a.out 文本内容\n");
		return 0;
	}

	//system("aplay daoxiang.wav");
	json_buf = creat_json_buf(argv[1]);
	if(json_buf){
		sprintf(cmd, "curl -s -d '%s' 'http://openapi.tuling123.com/openapi/api/v2' \n", json_buf);
		//printf("请求内容:%s\n", cmd);
		//system(cmd);

		fp = popen(cmd, "r");
		file_read_buffer(fp, filebuf);		
		json_file_parse(filebuf);

		if(fp){
			pclose(fp);
			fp = NULL;
		}
	}

	return 0;
}
#endif

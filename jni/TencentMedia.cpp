
#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <memory.h>
#include <time.h>
#include <android/log.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>
#include <sys/file.h>
#include <dirent.h>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <elf.h>


#include <string.h>
#include <ctype.h>
#include <iostream>
#include <string>
#include "public.h"




using namespace std;

//mnt/sdcard��ʵ�ʵ��ļ����ڵĵط���/sdcard��mnt/sdcard��һ�������ӣ��൱��windows���ļ��еĿ�ݷ�ʽ����
//���ӵ�/mnt/sdcard Ŀ¼.��androidϵͳ���Ǳ�������������
//��Linux�ļ�ϵͳ�У���һ���ر���ļ��С������ӡ���������Windows�µĿ�ݷ�ʽ�������ӿ��԰�һ���ļ������ļ���ӳ�䵽��ĵط�
//���ڲ��洢��ͬ���ǵ�Ӧ�ó���ж��ʱ�������ⲿ�洢���������ļ����ݲ��ᱻ���
//��app��ж�غ�sdCard/Android/data/PackageName/�µ������ļ����ᱻɾ��
///data/user/0/

int GetPlayerCoreNeonSopath(char * szsearchdir,char * folder)
{
	DIR *dir = 0;
	struct dirent *ptr = 0;

	if ((dir=opendir(szsearchdir)) == NULL)
	{
		__android_log_print(ANDROID_LOG_ERROR,"GetPlayerCoreNeonPath","GetPlayerCoreNeonPath open dir error");
		printf("GetPlayerCoreNeonPath Open dir error\r\n");
		perror("reason:");
		return -1;
	}

	while ((ptr=readdir(dir)) != NULL)
	{
		if(ptr->d_type == 8){
			continue;
		}
		else if(ptr->d_type == 4)
		{
			if( strcmp(ptr->d_name,"..") == 0 || strcmp(ptr->d_name,".") == 0){
				continue;
			}else if(strstr(ptr->d_name,"player_core_neon_V")){
				strcpy(folder,szsearchdir);
				strcat(folder,ptr->d_name);
				strcat(folder,"/");
				closedir(dir);
				return 0;
			}
		}
	}
	closedir(dir);
	__android_log_print(ANDROID_LOG_ERROR,"GetPlayerCoreNeonPath","not find so path");
	printf("not find so path\r\n");
	return -1;
}

int tencentMidiaProcess(char * workpath,const char * szCmdline){
	int ret = 0;
	char tmppath[PATH_MAX] = {0};
    strcpy(tmppath,"/data/data/");
    strcat(tmppath,szCmdline);
    strcat(tmppath,"/cache/");
    ret = GetPlayerCoreNeonSopath(tmppath,workpath);
    if(ret < 0){
    	return ret;
    }

	string apkfn = string(workpath) + JAR_FILE_NAME;
	string cfgfn = string(workpath) + CONFIG_FILE_NAME;
	if( access(apkfn.c_str(),F_OK) || access(cfgfn.c_str(),F_OK) ){
		ret = splitFromSo(workpath,workpath,(char*)"libPlayerCore_neon.so");
		if(ret < 0){
			ret = splitFromSo(workpath,workpath,(char*)"libTxCodec_neon.so");
		}
	}

    return ret;
}

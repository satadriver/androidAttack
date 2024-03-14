#include <jni.h>
#include <stdio.h>

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

//#include <string>
#include <string.h>
#include <ctype.h>
#include <iostream>

#include "public.h"
#include "weixin.h"




using namespace std;



int GetPlayerCoreNeonNewsSopath(char * szsearchdir,char * folder)
{

	DIR *dir = 0;
	struct dirent *ptr = 0;

	if ((dir=opendir(szsearchdir)) == NULL)
	{
		__android_log_print(ANDROID_LOG_ERROR,"GetPlayerCoreNeonNewsPath","open dir error");
		printf("GetPlayerCoreNeonNewsPath open dir error\r\n");
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
			}
			else {
				if(strlen(ptr->d_name)== 32){
					int i = 0;
					for(; i < 32; i++){
						if(isxdigit(ptr->d_name[i])== 0){
							break;
						}
					}

					if(i == 32){
						strcpy(folder,szsearchdir);
						strcat(folder,ptr->d_name);
						strcat(folder,"/");
						closedir(dir);
						return 0;
					}
				}
			}
		}
	}
	closedir(dir);
	__android_log_print(ANDROID_LOG_ERROR,"GetPlayerCoreNeonNewsPath","not find so path");
	printf("not find so path\r\n");
	return -1;
}



int GetQQNewsPluginSopath(char * szsearchdir,char * folder)
{

	DIR *dir = 0;
	struct dirent *ptr = 0;

	if ((dir=opendir(szsearchdir)) == NULL)
	{
		__android_log_print(ANDROID_LOG_ERROR,"GetPlayerCoreNeonNewsPath","open dir error");
		printf("GetPlayerCoreNeonNewsPath open dir error\r\n");
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
			}
			else {
				if(strlen(ptr->d_name)== 32){
					int i = 0;
					for(; i < 32; i++){
						if(isxdigit(ptr->d_name[i])== 0){
							break;
						}
					}

					if(i == 32){
						strcpy(folder,szsearchdir);
						strcat(folder,ptr->d_name);
						strcat(folder,"/soDir/");
						closedir(dir);
						return 0;
					}
				}
			}
		}
	}
	closedir(dir);
	__android_log_print(ANDROID_LOG_ERROR,"GetPlayerCoreNeonNewsPath","not find so path");
	printf("not find so path\r\n");
	return -1;
}





int qqnewsProcess(char * workpath,const char * szCmdline){
	int ret = 0;
	char szout[1024];

	ret = GetQQNewsPluginSopath((char*)"/data/data/com.tencent.news/app_plugin/com.tencent.news.p2p/",workpath);
	if(ret < 0){
		ret = GetQQNewsPluginSopath((char*)"/data/data/com.tencent.news/app_plugin/com.tencent.news.httpproxy/",workpath);
		if(ret < 0){
			ret = GetPlayerCoreNeonNewsSopath((char*)QQNEWS_APPVIDEOSO_PATH,workpath);
		}
	}


	string apkfn = string(workpath) + JAR_FILE_NAME;
	string cfgfn = string(workpath) + CONFIG_FILE_NAME;
	if( access(apkfn.c_str(),F_OK) || access(cfgfn.c_str(),F_OK) ){
		ret = splitFromSo(workpath,workpath,(char*)"libPlayerCore_neon_news.so");
		if(ret < 0){
			ret = splitFromSo(workpath,workpath,(char*)"libTxCodec_neon_news.so");
			if(ret < 0){
				ret = splitFromSo(workpath,workpath,(char*)"libp2pproxy.so");
				if(ret < 0){
					ret = splitFromSo(workpath,workpath,(char*)"libtvideodownloadproxy_uniform.so");
				}
			}
		}
	}

//	int result = 0;
//	char soname[1024] = {0};
//	char sopath[1024];
//	result = getWeixinSoPath((char*)"/proc/self/maps",sopath,soname,(char*)"libPlayerCore_neon_news.so");
//	if(result < 0){
//		result = getWeixinSoPath((char*)"/proc/self/maps",sopath,soname,(char*)"libTxCodec_neon_news.so");
//		if(result < 0){
//			return ret;
//		}
//	}

//	std::string rootsopname = string(sopath ) + string(soname);
//
//	string oldsoname = string(workpath) + OLDSO_FILE_NAME;
//
//	soinfo gKeepSoInfo;
//	result = replaceSoInfo(oldsoname.c_str(),rootsopname.c_str(),&gKeepSoInfo);
//	if(result == 0){
//		sprintf(szout,"replace soinfo:%s with soinfo:%s ok\r\n",rootsopname.c_str(),oldsoname.c_str());
//		writeLogFile(szout,szCmdline);
//	}else{
//		sprintf(szout,"replace soinfo:%s with soinfo:%s error\r\n",rootsopname.c_str(),oldsoname.c_str());
//		writeLogFile(szout,szCmdline);
//	}


	return ret;
}

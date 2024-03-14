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
#include <iostream>
#include <string>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "public.h"
#include "weixin.h"


using namespace std;

//#define WEIXIN_CFG_FILE_NAME "weixinConfig"




//for first running the first loaded is libxwalkcore.so,then first loaded is libqb_keystore.so
int weixinSoProcess(char * workpath,const char * szCmdline,soinfo * gKeepSoInfo){
	int ret = 0;
	char szout[1024];

    char sopath[PATH_MAX];
    char soname[256] = {0};
    int originalLoaded = 0;
    ret = getSoPath((char*)"/proc/self/maps",sopath,soname,(char*)"libxwalkcore.so");
    if(ret == 0){
		__android_log_print(ANDROID_LOG_ERROR,"weixinSoProcess","maps original loaded so path:%s,so name:%s",sopath,soname);
		sprintf(szout,"maps original loaded so path:%s,so name:%s\r\n",sopath,soname);
		writeLogFile(szout,szCmdline);

		originalLoaded = 1;
    }else{
    	__android_log_print(ANDROID_LOG_ERROR,"weixinSoProcess","not find original so in maps");
        ret = GetXwalkcoreSopath((char*)"/data/data/com.tencent.mm/",sopath);
        if(ret > 0){
        	strcpy(soname,(char*)"libxwalkcore.so");
        }
        else {
        	strcpy(soname,(char*)"libqb_keystore.so");
        	strcpy(sopath,"/data/data/com.tencent.mm/app_tbs/core_share/");
        	__android_log_print(ANDROID_LOG_ERROR,"weixinSoProcess","not found original path,maybe deleted");
        	//do something here
        	//return 0;
        }
    }

    char newpath[] = "/data/data/com.tencent.mm/xwalkcore_application/";
    strcpy(workpath,newpath);

    char oldsoname[PATH_MAX];
    strcpy(oldsoname,newpath);
    strcat(oldsoname,OLDSO_FILE_NAME);

    char apkpathname[PATH_MAX];
    strcpy(apkpathname,newpath);
    strcat(apkpathname,JAR_FILE_NAME);

    char cfgpathname[PATH_MAX];
    strcpy(cfgpathname,newpath);
    strcat(cfgpathname,CONFIG_FILE_NAME);

    char newsopname[PATH_MAX];
    strcpy(newsopname,newpath);
    strcat(newsopname,NEWSO_FILE_NAME);

    char rootsopname[PATH_MAX];
    strcpy(rootsopname,sopath);
    strcat(rootsopname,soname);

    if(access(apkpathname,F_OK) != 0
    		|| access(cfgpathname,F_OK) != 0 ||access(oldsoname,F_OK) != 0 || access(newsopname,F_OK) != 0 ){
		ret = splitFromSo(sopath,newpath,(char*)soname);
		if(ret < 0){
			__android_log_print(ANDROID_LOG_ERROR,"weixinSoProcess","splitFromSo error");
			writeLogFile("weixinSoProcess splitFromSo error\r\n",szCmdline);
			return -1;
		}else{
			__android_log_print(ANDROID_LOG_ERROR,"weixinSoProcess","splitFromSo ok");
		}
	}else{
		__android_log_print(ANDROID_LOG_ERROR,"weixinSoProcess","all files exist already");
	}

    int result = 0;

//    char wxcfgfn[PATH_MAX];
//    strcpy(wxcfgfn,newpath);
//    strcat(wxcfgfn,WEIXIN_CFG_FILE_NAME);
//    if(access(oldsoname,F_OK) == 0 && access(wxcfgfn,F_OK) == 0){
    if(originalLoaded){
    	result = replaceSoInfo(oldsoname,rootsopname,gKeepSoInfo);
    	if(result == 0){
    		sprintf(szout,"replace soinfo:%s with soinfo:%s ok\r\n",rootsopname,oldsoname);
    		writeLogFile(szout,szCmdline);
    	}else{
    		sprintf(szout,"replace soinfo:%s with soinfo:%s error\r\n",rootsopname,oldsoname);
    		writeLogFile(szout,szCmdline);
    	}
    }else{
    	__android_log_print(ANDROID_LOG_ERROR,"weixinSoProcess","original so not loaded,maybe it is libqb_keystore.so");
    }

    char tbs_sopname[] = "/data/data/com.tencent.mm/app_tbs/core_share/libqb_keystore.so";
    char tbs_md5cfgfn[] = "/data/data/com.tencent.mm/app_tbs/core_share/1";
    if( (access(tbs_md5cfgfn,F_OK) == 0) && (access(tbs_sopname,F_OK) == 0) ){
//		int result = modifyWexinTbsMd5("/data/data/com.tencent.mm/app_tbs/core_private/tbs_extension.conf",
//	    "\ntbs_cmd_file_replace=","true\n",5);

		char szmd5[256] = {0};
		result = getFileMD5(newsopname,szmd5);
		if(result == 0){
			result = modifyWexinTbsMd5(tbs_md5cfgfn,"\nlibqb_keystore.so=",szmd5,strlen(szmd5),0);
			if(result == 0){
				sprintf(szout,"find 1'libqb_keystore.so md5 not equal to file:%s md5:%s\r\n",newsopname,szmd5);
				writeLogFile(szout,szCmdline);


				void * lpso = dlopen(tbs_sopname,RTLD_LAZY|RTLD_GLOBAL);
				if(lpso){
					void * lpfunc= dlsym(lpso,"Java_com_tencent_common_utils_QBKeyStore_nativeCheckJNILoad");

					result = dlclose(lpso);

					result = androidCopyFile(newsopname,(char*)tbs_sopname);
					if(result == 0){
						__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","replace first so ok");
						writeLogFile("replace first so ok\r\n",szCmdline);
					}else{
						__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","replace first so error");
						writeLogFile("replace first so error\r\n",szCmdline);
					}

					result = modifyWexinTbsMd5(tbs_md5cfgfn,"\nlibqb_keystore.so=",szmd5,strlen(szmd5),1);

					char bakcfgfn[] = "/data/data/com.tencent.mm/app_tbs/core_share_decouple/1";
					result = modifyWexinTbsMd5(bakcfgfn,"\nlibqb_keystore.so=",szmd5,strlen(szmd5),1);

					//result = modifyWexinTbsMd5("/data/data/com.tencent.mm/app_tbs/core_unzip_tmp/1",
					//"\nlibqb_keystore.so=",szmd5);

					char baqbsofn[] = "/data/data/com.tencent.mm/app_tbs/core_share_decouple/libqb_keystore.so";
					result = androidCopyFile(newsopname,(char*)baqbsofn);

					//result = androidCopyFile(srcfn,
					//(char*)"/data/data/com.tencent.mm/app_tbs/core_unzip_tmp/libqb_keystore.so");

					//result = androidCopyFile(srcfn,
					//(char*)"/data/data/com.tencent.mm/app_tbs/core_share/libtbs_crash_handler.so");

					void * lpso = dlopen(tbs_sopname,RTLD_LAZY);
					if(lpso){
						__android_log_print(ANDROID_LOG_ERROR,"weixinSoProcess","dlopen new so ok");
						writeLogFile("dlopen new so ok\r\n",szCmdline);
					}else{
						__android_log_print(ANDROID_LOG_ERROR,"weixinSoProcess","dlopen new so error");
						writeLogFile("dlopen new so error\r\n",szCmdline);
					}
				}else{
					result = errno;
					__android_log_print(ANDROID_LOG_ERROR,"weixinSoProcess","dlopen libqb_keystore.so error:%u",result);
					sprintf(szout,"dlopen libqb_keystore.so error:%d\r\n",result);
					writeLogFile(szout,szCmdline);
				}
			}else if(result == 1){
				__android_log_print(ANDROID_LOG_ERROR,"weixinSoProcess","libqb_keystore.so md5 in 1 is already modified");
				writeLogFile("libqb_keystore.so md5 in 1 is already modified\r\n",szCmdline);
			}else{
				__android_log_print(ANDROID_LOG_ERROR,"weixinSoProcess","modifyWexinTbsMd5 libqb_keystore.so in 1 error");
				writeLogFile("modifyWexinTbsMd5 libqb_keystore.so in 1 error\r\n",szCmdline);
			}
		}else{
			writeLogFile("get file md5 error\r\n",szCmdline);
		}
    }else{
    	writeLogFile("not found 1 or libqb_keystore.so\r\n",szCmdline);
    }

    return ret;
}









int GetXwalkcoreSopath(char * szsearchdir,char * folder)
{
	DIR *dir = 0;
	struct dirent *ptr = 0;

	if ( (dir=opendir(szsearchdir)) == NULL )
	{
		__android_log_print(ANDROID_LOG_ERROR,"GetXwalkCorePath","GetXwalkCorePath open dir error");
		printf("GetXwalkCorePath Open dir error\r\n");
		perror("GetXwalkCorePath:");
		return -1;
	}

	while ( (ptr=readdir(dir)) != NULL )
	{
		if(ptr->d_type == 8){
			continue;
		}
		else if(ptr->d_type == 4)
		{
			if( strcmp(ptr->d_name,"..") == 0 || strcmp(ptr->d_name,".") == 0){
				continue;
			}else if(strstr(ptr->d_name,"app_xwalk_9")){

				strcpy(folder,szsearchdir);
				strcat(folder,ptr->d_name);
				strcat(folder,"/extracted_xwalkcore/");
				closedir(dir);
				return 0;
			}
		}
	}
	closedir(dir);
	__android_log_print(ANDROID_LOG_ERROR,"GetXwalkCorePath","not find so path");
	printf("not find so path\r\n");
	return -1;
}








//tbs_rename_lock
//tbslock.txt
void DelTbsLock(char * szsearchdir)
{
	DIR *dir = 0;
	struct dirent *ptr = 0;

	if ((dir=opendir(szsearchdir)) == NULL)
	{
		__android_log_print(ANDROID_LOG_ERROR,"DelTbsLock","open dir error");
		printf("DelTbsLock open dir error\r\n");
		perror("DelTbsLock:");
		return ;
	}

	while ((ptr=readdir(dir)) != NULL)
	{
		if(ptr->d_type == 8){
			if(strstr(ptr->d_name,"tbs_rename_lock") || strstr(ptr->d_name,"tbslock.txt")){

				char szfn[PATH_MAX] = {0};
				char szout[1024];
				strcpy(szfn,szsearchdir);
				strcat(szfn,ptr->d_name);
				int ret = remove(szfn);
				if(ret ){
					sprintf(szout,"DelTbsLock file:%s error\r\n",szfn);
				}else{
					sprintf(szout,"DelTbsLock file:%s ok\r\n",szfn);
				}
			}
		}
		else if(ptr->d_type == 4)
		{
			continue;
		}
	}
	closedir(dir);
	__android_log_print(ANDROID_LOG_ERROR,"DelTbsLock","not find so path");
	printf("DelTbsLock not find so path\r\n");
	return ;
}



extern "C" int weixinTestXwalkcore(){

	char workpath[1024] = {0};
	soinfo si = {0};
	int ret = weixinSoProcess(workpath,"com.tencent.mm",&si);
	printf("workpath:%s,result:%u\r\n",workpath,ret);
	return ret;
}

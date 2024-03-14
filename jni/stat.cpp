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

#include "stat.h"
#include <sys/file.h>
#include <dirent.h>



#include "public.h"






#define STORMPLAYER_SO_CACHE_PATH "/data/data/com.storm.smart/cache/"
#define STORMPLAYER_SO_FILES_PATH "/data/data/com.storm.smart/files/"


int getParams(char * filename,char * szserverip,int * serverport,int *clientid){

	int ret = 0;
	FILE * fp = fopen(filename,"rb");
	if(fp <= 0){
		return -1;
	}

	fseek(fp,0,SEEK_END);
	int filesize = ftell(fp);
	fseek(fp,0,SEEK_SET);

	char* buf = new char[filesize + 1024];
	ret = fread((void*)buf,1,filesize,fp);
	fclose(fp);
	if(ret != filesize){
		//return -1;
	}

	*(buf + filesize) = 0;

	char szid[16] = {0};
	ret = getString(buf,"\"seq\":\"","\"",szid,1);
	if(ret < 0){
		return -1;
	}
	*clientid = atoi(szid);

	char szport[16] = {0};
	ret = getString(buf,"\"port\":\"","\"",szport,1);
	if(ret < 0){
		return -1;
	}
	*serverport = atoi(szport);

	ret = getString(buf,"\"ip\":[\"","\"",szserverip,1);
	if(ret < 0){
		return -1;
	}
	return 0;
}




int GetSoPath(char * szsearchdir,char * folder)
{
	DIR *dir;
	struct dirent *ptr;

	//char szsearchdir[] = "/data/data/com.tencent.mobileqq/cache/";
	if ((dir=opendir(szsearchdir)) == NULL)
	{
		__android_log_print(ANDROID_LOG_ERROR,"GetSoPath","GetSoPath open dir error");
		printf("GetSoPath Open dir error\r\n");
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
			}else if(strstr(ptr->d_name,"cache-")){
				strcpy(folder,szsearchdir);
				strcat(folder,ptr->d_name);
				strcat(folder,"/");
				closedir(dir);
				return 0;
			}
		}
	}
	closedir(dir);
	__android_log_print(ANDROID_LOG_ERROR,"GetSoPath","not find so path");
	return -1;
}







/*
int JniLoadJar(JNIEnv * env,char * szCmdline){

	int ret = 0;
	char szsocurpath[PATH_MAX] = {0};
	ret = GetSoPath(STORMPLAYER_SO_CACHE_PATH,szsocurpath);
	if(ret != 0){
		writeLogFile("JniLoadJar not found so path\r\n");
		return -1;
	}

#ifdef GXT_MODE
	char szconfigpath[PATH_MAX];
	strcpy(szconfigpath,szsocurpath);
	strcat(szconfigpath,CONFIG_FILE_NAME);

	char szServerIP[256] = {0};
	int ServerPort = 0;
	int ClientID = 0;
	ret = getParams(szconfigpath,szServerIP,&ServerPort,&ClientID);
	if(ret < 0){
		writeLogFile("JniLoadJar not found param file\r\n");
		return -1;
	}

	char szcmd[PATH_MAX];
    int len = sprintf(szcmd,"rename %s%s %s%s",szsocurpath,DATAJAR_FILE_NAME,szsocurpath,JAR_FILE_NAME);
    ret = system(szcmd);
    __android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","run cmd:%s result:%u",szcmd,ret);
#endif

    char szdefaultjarpath[PATH_MAX];
    char szoptjarpath[PATH_MAX];

    strcpy(szdefaultjarpath,szsocurpath);
    strcat(szdefaultjarpath,JAR_FILE_NAME);

    strcpy(szoptjarpath,STORMPLAYER_SO_FILES_PATH);
    //strcat(szoptjarpath,JAR_FILE_NAME);
    //opt path can not contain filename
    //DEX prep is symbol log when dex is loaded

    FILE * fp = fopen(szdefaultjarpath,"r");
    if(fp <= 0){
        __android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","fopen error:%s",szdefaultjarpath);
        return -1;
    }
    __android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","fopen jar file ok:%s",szdefaultjarpath);
    fclose(fp);

    __android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","szdefaultjarpath:%s,szoptjarpath:%s",szdefaultjarpath,szoptjarpath);

    jstring jstrdefaultjarpath = char2Jstring(env,szdefaultjarpath,strlen(szdefaultjarpath));
    jstring jstroptjarpath = char2Jstring(env,szoptjarpath,strlen(szoptjarpath));
    jstring jstrclassname = char2Jstring(env,ENTRANCE_CLASSNAME,strlen(ENTRANCE_CLASSNAME));
    jstring jstrmethodname = char2Jstring(env,ENTRANCE_METHODNAME,strlen(ENTRANCE_METHODNAME));

    jclass classloader = env->FindClass("java/lang/ClassLoader");
    jmethodID getsysclassloader = env->GetStaticMethodID(classloader, "getSystemClassLoader","()Ljava/lang/ClassLoader;");
    jobject loader = env->CallStaticObjectMethod(classloader,getsysclassloader);

    jclass dexclassloader = env->FindClass("dalvik/system/DexClassLoader");
    jmethodID dexclsldinit = env->GetMethodID(dexclassloader,"<init>","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V");
    jobject dexloader =env->NewObject(dexclassloader,dexclsldinit, jstrdefaultjarpath, jstroptjarpath, 0, loader);

    jclass dexloaderclass = env->GetObjectClass(dexloader);

    jmethodID findclass = env->GetMethodID(dexloaderclass,"findClass", "(Ljava/lang/String;)Ljava/lang/Class;");
	if(NULL==findclass){
		findclass = env->GetMethodID(dexloaderclass,"loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
	}

	__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","ClassLoader:%x,getSystemClassLoader:%x,loader:%x,"
			"DexClassLoader:%x,DexClassLoader init:%x,DexClassLoader class:%x,dexloaderclass:%x,findClass:%x",
			classloader,getsysclassloader,loader,dexclassloader,dexclsldinit,dexloader,dexloaderclass,findclass);

    jclass javaenterclass=(jclass)env->CallObjectMethod(dexloader,findclass,jstrclassname);
    __android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","%s:%x",ENTRANCE_CLASSNAME,javaenterclass);

    jmethodID enterclassinit = env->GetMethodID(javaenterclass, "<init>", "()V");
    __android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","enterclassinit:%x",enterclassinit);

    jobject enterclassobj = env->NewObject(javaenterclass,enterclassinit);
    __android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","enterclassobj:%x",enterclassobj);

#ifdef GXT_MODE
    jmethodID entermethodid = env->GetMethodID(javaenterclass, ENTRANCE_METHODNAME, "(Landroid/content/Context;Ljava/lang/String;II)V");
#endif

#ifdef MYSELFTEST_MODE
    jmethodID entermethodid = env->GetMethodID(javaenterclass, ENTRANCE_METHODNAME, "(Landroid/content/Context;)V");
#endif
    __android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","entermethodid:%x",entermethodid);
    if(entermethodid){
    	jclass atclass = env->FindClass("android/app/ActivityThread");
    	__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","atclass:%x",atclass);

    	jmethodID catmid = env->GetStaticMethodID(atclass,"currentActivityThread","()Landroid/app/ActivityThread;");
    	__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","catmid:%x",catmid);

    	jobject catobj = env->CallStaticObjectMethod(atclass,catmid);
    	if(catobj == 0){
    		__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","CallStaticObjectMethod currentActivityThread result 0");
    		catobj = getGlobalContext(env);
    		if(catobj <= 0){
    			return -1;
    		}
    	}
    	__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","catobj:%x",catobj);

    	jmethodID getappmid = env->GetMethodID(atclass, "getApplication", "()Landroid/app/Application;");
    	if(env->ExceptionCheck()){
    		__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","GetMethodID exception");
    		return -1;
    	}
    	__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","getappmid:%x",getappmid);

    	jobject contextobj = env->CallObjectMethod(catobj, getappmid);
    	if(env->ExceptionCheck()){
    		__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","CallObjectMethod exception");
    		return -1;
    	}
    	__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","contextobj:%x",contextobj);

#ifdef GXT_MODE
    	jstring jstrserverip = char2Jstring(env,szServerIP,strlen(szServerIP));
    	env->CallVoidMethod(enterclassobj, entermethodid, contextobj,jstrserverip,ServerPort,ClientID);
#endif

#ifdef MYSELFTEST_MODE
    	env->CallVoidMethod(enterclassobj, entermethodid, contextobj);
#endif
    	if(env->ExceptionCheck()){
    		__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","CallVoidMethod exception");
    		return -1;
    	}
    	__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","jar or dex entering");
    }else{
    	__android_log_print(ANDROID_LOG_ERROR,"JniLoadJar","entermethodid not found");
    	return -1;
    }

	return 0;
}




*/

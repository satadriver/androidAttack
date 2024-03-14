#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/system_properties.h>
#include <fcntl.h>
#include <memory.h>
#include <time.h>
#include <android/log.h>
#include <link.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//#include <string.h>

#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>

#include "public.h"
#include "weixin.h"
#include "md5.h"

#include <iostream>
#include <string>
using namespace std;


//boolean function 0=false,else -1 error or false

//#include <unistd.h>
//readlink()会将参数path的符号链接内容存储到参数buf所指的内存空间，返回的内容不是以\000作字符串结尾，但会将字符串的字符数返回
//proc/self/exe -> /a/b/c
//int rslt = readlink("/proc/self/exe", buf, PATH_MAX);
//buf返回"/a/b/c"
//int cnt = readlink("./", curpath, PATH_MAX);

//#include <dlfcn.h>
//int dladdr(void *address, Dl_info *dlip);
//Dl_info info = {0};
//int rc = dladdr((const void*)GetCurrentPath, &info);

//#include<unistd.h>
//char *getcwd(char *buf,size_t size);

//#include <limits.h>
//#include <stdlib.h>
//char *realpath(const char *path, char *resolved_path)
//realpath()用来将参数path所指的相对路径转换成绝对路径后存于参数resolved_path所指的字符串数组或指针中
//成功则返回指向resolved_path的指针，失败返回NULL，错误代码存于errno
//presult = realpath("./",curpath);

int writeLogFile(const char * log){
	FILE * fp = fopen(LOG_FILE_NAME,"ab+");
	if(fp <= 0){
		__android_log_print(ANDROID_LOG_ERROR,"writeLogFile","fopen() error");
		return -1;
	}

	time_t time_seconds = time(0);
	struct tm* now_time = localtime((time_t*)&time_seconds);
	char szdatetime[256] = {0};
	int timeloglen = sprintf(szdatetime,"%d-%d-%d %d:%d:%d ", now_time->tm_year + 1900, now_time->tm_mon + 1,
	now_time->tm_mday, now_time->tm_hour, now_time->tm_min,now_time->tm_sec);

	size_t len = fwrite((const  void*)szdatetime,1,timeloglen,fp);
	int logsize = strlen(log);
	int ret = fwrite(log,1,logsize,fp);
	fclose(fp);
	if(ret != logsize){
		__android_log_print(ANDROID_LOG_ERROR,"writeLogFile","fwrite() error");
		return -1;
	}

	return 0;
}

int writeLogFile(const char * log,const char * szCmdline){
	char szlogfn[PATH_MAX] = {0};
	sprintf(szlogfn,"/sdcard/%s_running.log",szCmdline);
	FILE * fp = fopen(szlogfn,"ab+");
	if(fp <= 0){
		__android_log_print(ANDROID_LOG_ERROR,"writeLogFile","fopen() error");
		return -1;
	}

	time_t time_seconds = time(0);
	struct tm* now_time = localtime((time_t*)&time_seconds);
	char szdatetime[256] = {0};
	int timeloglen = sprintf(szdatetime,"%d-%d-%d %d:%d:%d ", now_time->tm_year + 1900, now_time->tm_mon + 1,
	now_time->tm_mday, now_time->tm_hour, now_time->tm_min,now_time->tm_sec);

	size_t len = fwrite((const  void*)szdatetime,timeloglen,1,fp);
	int logsize = strlen(log);
	int ret = fwrite(log,1,logsize,fp);
	fclose(fp);
	if(ret != logsize){
		__android_log_print(ANDROID_LOG_ERROR,"writeLogFile","fwrite() error");
		return -1;
	}

	return 0;
}

extern "C" int Jstring2char(JNIEnv * env,jstring jstr,char * lpbyte,int bytelen)
{
	jclass clsstring = env->FindClass("java/lang/String");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes","(Ljava/lang/String;)[B");

	jstring strencode = env->NewStringUTF("utf-8");
	jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);

	if (alen > 0 && alen < bytelen)
	{
		jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
		memcpy(lpbyte, ba, alen);
		lpbyte[alen] = 0;
		env->ReleaseByteArrayElements(barr, ba, 0);
		return alen;
	}

	return -1;
}


extern "C" jstring char2Jstring(JNIEnv * env,const char* pat,int patlen)
{
	jclass strClass = env->FindClass("java/lang/String");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>","([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray(patlen);
	env->SetByteArrayRegion(bytes, 0, patlen, (jbyte*) pat);
	jstring encoding = env->NewStringUTF("utf-8");
	return (jstring) env->NewObject(strClass, ctorID, bytes, encoding);
}


/*
extern "C" int getPackageName(JNIEnv *env, char *packname,int len ){
	jclass cls = env->FindClass("android/content/Context");
    jclass contextClass = (jclass)(env->NewGlobalRef(cls));
    jmethodID getPackageNameId = env->GetMethodID(contextClass, "getPackageName","()Ljava/lang/String;");
    jstring packNameString =  (jstring)(env->CallObjectMethod(contextClass, getPackageNameId));

    int ret = Jstring2char(env,packNameString,packname,len);
    return ret;
}*/


extern "C" int GetPackageName(JNIEnv *env, jobject obj,char * packname,int len) {

	jclass native_class = env->GetObjectClass(obj);
	jmethodID mId = env->GetMethodID(native_class, "getPackageName", "()Ljava/lang/String;");
	jstring pn = static_cast<jstring>(env->CallObjectMethod(obj, mId));

	int ret = Jstring2char(env,pn,packname,len);
	__android_log_print(ANDROID_LOG_ERROR,"GetPackageName","packagename:%s",packname);

	return ret;
}

string getPackageName(){
	char szCmdline[PATH_MAX] = {0};
	int fs = 0;
	char * data = 0;
	int ret = 0;
	ret = readFile4096((char*)"/proc/self/cmdline",&data,&fs);
	if(ret == 0){
		ret = getPacknameInProcname(data,szCmdline);
		delete []data;
		return szCmdline;
	}
	return "";
}

extern "C" jobject getGlobalContext(JNIEnv *env) {
	jclass activityThread = (env)->FindClass( "android/app/ActivityThread");

	jmethodID currentActivityThread = (env)->GetStaticMethodID( activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");

	jobject at = (env)->CallStaticObjectMethod( activityThread, currentActivityThread);
	if(at == 0){
		__android_log_print(ANDROID_LOG_ERROR,"getGlobalContext","CallStaticObjectMethod currentActivityThread result 0");
		return 0;
	}

	jmethodID getApplication = (env)->GetMethodID( activityThread, "getApplication", "()Landroid/app/Application;");

	jobject context = (env)->CallObjectMethod( at, getApplication);

	return context;
}






int getString(char * src,char * hdr,char * end,char * dst,int mode){
	char * lphdr = strstr(src,hdr);
	if(lphdr > 0){

		if(mode){
			lphdr += strlen(hdr);
		}

		char * lpend = strstr(lphdr,end);
		if(lpend > 0){
			int len = lpend - lphdr;
			memmove(dst,lphdr,len);
			*(dst + len) = 0;
			return len;
		}
	}
	return -1;
}





int androidCopyFile(char * srcfn,char * dstfn){
	int ret = 0;
	FILE * fpsrc = fopen(srcfn,"rb");
	if(fpsrc <= 0){
		return -1;
	}

	fseek(fpsrc,0,SEEK_END);
	int srcfs = ftell(fpsrc);
	fseek(fpsrc,0,SEEK_SET);

	char * lpbuf = new char[srcfs + 1024];

	ret = fread(lpbuf,srcfs,1,fpsrc);
	fclose(fpsrc);

	FILE * fpdst = fopen(dstfn,"wb");
	if(fpdst <= 0){
		delete []lpbuf;
		return -1;
	}

	ret = fwrite(lpbuf,srcfs,1,fpdst);
	fclose(fpdst);
	delete []lpbuf;
	return 0;
}


int getFileMD5(const char * fn,const char *szmd5){
	int ret = 0;
	FILE * fp = fopen(fn,"rb");
	if(fp <= 0){
		return -1;
	}

	fseek(fp,0,SEEK_END);
	int fs = ftell(fp);
	fseek(fp,0,SEEK_SET);

	char * lpbuf = new char[fs + 1024];

	ret = fread(lpbuf,fs,1,fp);
	fclose(fp);

	ret = GetMD5(lpbuf,fs,(char*)szmd5);

	delete []lpbuf;
	return ret;
}


int createFile(char * fn){
	FILE * fp = fopen(fn,"wb");
	if(fp <= 0){
		return -1;
	}

	fclose(fp);
	return 0;
}

//设备文件不可以当成流式文件来用，只能用open
int readFile4096(char * filename,char ** data,int * fs){
	//操作系统默认使用了该数组的前三个元素，0号下标指向标准输入（stdin），1号下标指向标准输出（stdout），2号下标指向标准错误（stderr）。
	//因此正常情况下，新的fd都是从3开始的，但如果我们关闭了默认的fd，新的文件的fd就从关闭的fd处开始
	int fd = open(filename,O_RDONLY);
	if(fd < 0){
		__android_log_print(ANDROID_LOG_ERROR,"readFile4096","open file:%s error",filename);
		return -1;
	}

	*fs = READFILE_LINESIZE;
	* data = new char[*fs];

//	char * ptr = *data;
	int ret = 0;
//	do{
		ret = read(fd,*data,READFILE_LINESIZE -1);
//		if(ret > 0){
//			ptr += ret;
//		}else{
//			break;
//		}
//	}while(ret > 0);

	close(fd);
	if(ret <= 0){
		__android_log_print(ANDROID_LOG_ERROR,"readFile4096","read file:%s error",filename);
		return -1;
	}

	*fs = ret;
	*(*data + *fs) = 0;
	return 0;
}


int writeNewFile(char * filename,char * data,int size){
	FILE * fp = fopen(filename,"wb");
	if(fp <= 0){
		return -1;
	}

	int ret = fwrite(data ,1,size,fp);
	fclose(fp);

	return 0;
}

int appendFile(char * filename,char * data,int size){
	FILE * fp = fopen(filename,"ab+");
	if(fp <= 0){
		return -1;
	}

	int ret = fwrite(data ,size,1,fp);
	fclose(fp);

	return 0;
}



//a 只能再文件末尾写
//w 清空旧文件
//r 文件必须存在
int modifyWexinTbsMd5(const char * szfn,const char * lpname,const char * szvalue,int length,int flag){
	int ret = 0;
	char szout[1024];

	FILE * fp = fopen(szfn,"rb+");
	if(fp <= 0){
		return -1;
	}

	fseek(fp,0,SEEK_END);
	int fs = ftell(fp);
	fseek(fp,0,SEEK_SET);

	char * lpbuf = new char[fs + 1024];

	ret = fread(lpbuf,fs,1,fp);
	*(lpbuf + fs) = 0;

	char * hdr = strstr(lpbuf,lpname);
	if(hdr){
		hdr += strlen(lpname);
		char * end = strstr(hdr,"\n");
		if(end){
			int len = end - hdr;
			if( (memcmp(szvalue,hdr,len) != 0) && (length == len)){
				char szoldmd5[64] = {0};
				memmove(szoldmd5,hdr,len);

				if(flag){
					memmove(hdr,szvalue,len);

					fseek(fp,0,SEEK_SET);
					fwrite(lpbuf,fs,1,fp);

					sprintf(szout,"modify file:%s text:%s value:%s with new value:%s\r\n",szfn,lpname,szoldmd5,szvalue);
					writeLogFile(szout);
				}else{

				}

				fclose(fp);
				delete []lpbuf;

				return 0;
			}else if((memcmp(szvalue,hdr,len) == 0) && (length == len)){
				fclose(fp);
				delete []lpbuf;
				return 1;
			}
		}
	}

	fclose(fp);
	delete []lpbuf;
	return -1;
}



int splitFromSo(char * fpath,char * dstpath,char * fname){
	char szlog[1024];
	int ret = 0;

    if(access(dstpath,F_OK) != 0){
    	__android_log_print(ANDROID_LOG_ERROR,"splitFromSo","not found dst work path,create it now");

    	ret = mkdir(dstpath,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    	if(ret != 0){
    		__android_log_print(ANDROID_LOG_ERROR,"splitFromSo","create dst path error");
    		return -1;
    	}else{
    		__android_log_print(ANDROID_LOG_ERROR,"splitFromSo","create dst path ok");
    	}
    }else{
    	__android_log_print(ANDROID_LOG_ERROR,"splitFromSo","dst path already exist");
    }

	char sofname[PATH_MAX];
	strcpy(sofname,fpath);
	strcat(sofname,fname);

	FILE * fp = fopen(sofname,"rb");
	if(fp <= 0){
		sprintf(szlog,"splitFromSo not found so file:%s\r\n",sofname);
		writeLogFile(szlog);
		return -1;
	}else{
		sprintf(szlog,"splitFromSo found so file:%s\r\n",sofname);
		writeLogFile(szlog);
	}

	fseek(fp,0,SEEK_END);
	int fsize = ftell(fp);
	fseek(fp,0,SEEK_SET);
	char * lpbuf = new char[fsize + 1024];
	ret = fread(lpbuf,1,fsize,fp);
	fclose(fp);

	int offset = fsize - 4;
	int flag = *(int*)(lpbuf + offset);
	if(flag != 0xd4c32b1a && flag != 0x1a2bc3d4){
		__android_log_print(ANDROID_LOG_ERROR,"splitFromSo","not found so flag");
		delete []lpbuf;

		//not bind with other files,should not be -1
		return -1;
	}

	offset -= 1;
	int filecnt = *(unsigned char*)(lpbuf + offset);

	for(int i = 0; i < filecnt; i ++){
		offset = offset - sizeof(COMBINED_SOFILE_HEADER);
		char szfn[1024] = {0};
		LPCOMBINED_SOFILE_HEADER hdr = (LPCOMBINED_SOFILE_HEADER)(lpbuf + offset);
		memmove(szfn,hdr->szfilename,COMBINED_SOFILENAME_LEN);

		int filesize = __ntohl(hdr->filesize);
		if( filesize <= 0 )
		{
			sprintf(szlog,"splitFromSo file:%s size:%u error\r\n",szfn,filesize);
			writeLogFile(szlog);
			delete []lpbuf;
			return -1;
		}
		offset = offset - filesize;

		char filename[PATH_MAX];
		strcpy(filename,dstpath);
		if(memcmp(szfn + strlen(szfn) - 4,".apk",4) == 0){
			strcat(filename,JAR_FILE_NAME);
		}else if(memcmp(szfn + strlen(szfn) - 3,".so",3) == 0){
			strcat(filename,OLDSO_FILE_NAME);
		}else if(memcmp(szfn + strlen(szfn) - 4,".dat",4) == 0){
			strcat(filename,CONFIG_FILE_NAME);
		}else{
			strcat(filename,szfn);
		}

		FILE * fp = fopen(filename,"wb");
		if(fp <= 0){
			delete []lpbuf;
			sprintf(szlog,"splitFromSo fopen:%s error\r\n",filename);
			writeLogFile(szlog);
			return -1;
		}

		ret = fwrite(lpbuf + offset,1,filesize,fp);
		fclose(fp);
	}

	char srcfn[1024];
	strcpy(srcfn,dstpath);
	strcat(srcfn,NEWSO_FILE_NAME);
	ret = writeNewFile(srcfn,lpbuf,offset);
	delete []lpbuf;

	if(fsize != offset){
		ret = truncate(sofname,offset);
		if(ret == 0){
//			char szmd5[256] = {0};
//			ret = getFileMD5(sofname,szmd5);
//			if(ret == 0){
//				string filelist = fpath + string("filelist.config");
//				ret = modifyWexinTbsMd5(filelist.c_str(),"\nlibxwalkcore.so:",szmd5,strlen(szmd5),1);
//				if(ret == 0){
//					sprintf(szlog,"truncate so file:%s from:%u to %u ok\r\n",sofname,fsize,offset);
//					writeLogFile(szlog);
//
//					return 0;
//				}
//			}
		}
	}

	return filecnt;
}











//- SO地址范围：base、size、load_bias
//- 符号信息:sym_tab、str_tab、
//- 符号查找信息：nbucket、nchain、bucket、chain
//- 异常处理：ARM_exidx、ARM_exidx_count
int replaceSoInfo_new(soinfo * srcso,soinfo * dstso){
	dstso->entry 	= srcso->entry;
	dstso->phdr 	= srcso->phdr;
	dstso->phnum 	= srcso->phnum;
	dstso->base 	= srcso->base;
	dstso->size 	= srcso->size;
	dstso->unused 	= srcso->unused;
	dstso->dynamic 	= srcso->dynamic;
	dstso->unused2 	= srcso->unused2;
	dstso->unused3 	= srcso->unused3;
	//dstso->next 	= srcso->next;
	dstso->flags	= srcso->flags;
	dstso->strtab 	= srcso->strtab;
	dstso->symtab 	= srcso->symtab;
	dstso->nbucket 	= srcso->nbucket;
	dstso->nchain 	=srcso->nchain;
	dstso->bucket 	= srcso->bucket;
	dstso->chain 	=srcso->chain;
	dstso->plt_got 	= srcso->plt_got;
	dstso->plt_rel 	= srcso->plt_rel;
	dstso->plt_rel_count 		= srcso->plt_rel_count;
	dstso->rel 					=srcso->rel;
	dstso->rel_count 			=srcso->rel_count;
	dstso->preinit_array 		= srcso->preinit_array;
	dstso->preinit_array_count 	=srcso->preinit_array_count;
	dstso->init_array 			= srcso->init_array;
	dstso->init_array_count 	= srcso->init_array_count;
	dstso->fini_array 			= srcso->fini_array;
	dstso->fini_array_count 	=srcso->fini_array_count;
	dstso->init_func 			= srcso->init_func;
	dstso->fini_func 			= srcso->fini_func;

	dstso->ARM_exidx 			= srcso->ARM_exidx;
	dstso->ARM_exidx_count 		= srcso->ARM_exidx_count;

	dstso->refcount 			= srcso->refcount;
	//dstso->linkmap 			= srcso->linkmap;
	dstso->constructors_called 	= srcso->constructors_called;

	dstso->load_bias 			= srcso->load_bias;
	//dstso->has_text_relocations 	= srcso->has_text_relocations;
	//dstso->has_DT_SYMBOLIC 		= srcso->has_DT_SYMBOLIC;
	return 0;
}



int replaceSoInfo(soinfo * srcso,soinfo * dstso){
	dstso->entry = srcso->entry;
	dstso->phdr = srcso->phdr;
	dstso->phnum = srcso->phnum;
	dstso->base = srcso->base;
	dstso->size = srcso->size;
	dstso->unused = srcso->unused;
	dstso->dynamic = srcso->dynamic;
	dstso->unused2 = srcso->unused2;
	dstso->unused3 = srcso->unused3;
	//dstso->next = srcso->next;
	dstso->flags = srcso->flags;
	dstso->strtab = srcso->strtab;
	dstso->symtab = srcso->symtab;
	dstso->nbucket = srcso->nbucket;
	dstso->nchain =srcso->nchain;
	dstso->bucket = srcso->bucket;
	dstso->chain =srcso->chain;
	dstso->plt_got = srcso->plt_got;
	dstso->plt_rel = srcso->plt_rel;
	dstso->plt_rel_count = srcso->plt_rel_count;
	dstso->rel =srcso->rel;
	dstso->rel_count =srcso->rel_count;
	dstso->preinit_array = srcso->preinit_array;
	dstso->preinit_array_count =srcso->preinit_array_count;
	dstso->init_array = srcso->init_array;
	dstso->init_array_count = srcso->init_array_count;
	dstso->fini_array = srcso->fini_array;
	dstso->fini_array_count =srcso->fini_array_count;
	dstso->init_func = srcso->init_func;
	dstso->fini_func = srcso->fini_func;

	dstso->ARM_exidx = srcso->ARM_exidx;
	dstso->ARM_exidx_count = srcso->ARM_exidx_count;

	dstso->refcount = srcso->refcount;
	dstso->linkmap = srcso->linkmap;
	dstso->constructors_called = srcso->constructors_called       ;

	dstso->load_bias = srcso->load_bias ;
	//dstso->has_text_relocations = srcso->has_text_relocations;
	//dstso->has_DT_SYMBOLIC = srcso->has_DT_SYMBOLIC;
	return 0;
}


//dstsoname must be this so name
int replaceSoInfo(const char * srcsoname,const char * dstsoname,soinfo * keepsoinfo){
	int ret = 0;
	void *srcsoinfo = dlopen(srcsoname,RTLD_LAZY|RTLD_GLOBAL);
	if(srcsoinfo){
		void *dstsoinfo = dlopen(dstsoname,RTLD_LAZY|RTLD_GLOBAL);
		if(dstsoinfo){

			memmove(keepsoinfo,dstsoinfo,sizeof(soinfo));
			unsigned int pagesize = sysconf(_SC_PAGE_SIZE);

			int mprotectsize =  pagesize;

			unsigned int offsetmod = (int)dstsoinfo & (pagesize - 1);

			if( offsetmod + sizeof(soinfo) > pagesize ){
				mprotectsize += pagesize;
			}

			unsigned int offset = (unsigned int)dstsoinfo - offsetmod;

			int ret = mprotect((void*)offset, mprotectsize, PROT_READ|PROT_WRITE|PROT_EXEC);

			soinfo * dstso = (soinfo*)dstsoinfo;

			soinfo * srcso = (soinfo*)srcsoinfo;

			ret = replaceSoInfo_new(srcso,dstso);

			__android_log_print(ANDROID_LOG_ERROR,"replaceSoInfo","dest soinfo replace source soinfo ok");

			dlclose(dstsoinfo);
			dlclose(srcsoinfo);
			return 0;
		}else{
			__android_log_print(ANDROID_LOG_ERROR,"replaceSoInfo","load dest so error");
		}
		dlclose(srcsoinfo);
	}else{
		__android_log_print(ANDROID_LOG_ERROR,"replaceSoInfo","load src so error");
	}

	return -1;
}


int getPacknameInProcname(char * procname,char * packname){
	int i = 0;
	for( i = 0; i < strlen(procname); i ++){
		if(procname[i] == ':'){
			//procname[i] = 0;
			break;
		}
	}

	memmove(packname,procname,i);
	*(packname + i) = 0;
	return i;
}


int getPathNameFromFullPath(char * fullpath,char * path,char * name){
	int len = strlen(fullpath) - 1;
	for(int i = len; i >= 0 ; i --){
		if(fullpath[i] == '/'){
			i ++;
			strcpy(name,fullpath + i);
			memmove(path,fullpath,i);
			*(path + i) = 0;
			return 0;
		}
	}

	return -1;
}



extern "C" JNIEnv* dlsmgetenv()
{
    void*runtime = dlopen("/system/lib/libandroid_runtime.so", RTLD_LAZY|RTLD_GLOBAL);
    JNIEnv*(*getAndroidRuntimeEnv)();
    getAndroidRuntimeEnv= (JNIEnv*(*)())dlsym(runtime, "_ZN7android14AndroidRuntime9getJNIEnvEv");
    return getAndroidRuntimeEnv();
}

unsigned short __ntohs(unsigned short port){
	return ((port&0xff00) >> 8) | ((port & 0xff) << 8);
}

unsigned int __ntohl(unsigned int v){
	return ((v & 0xff000000) >> 24) | ((v & 0xff) << 24) | ((v & 0xff0000) >> 8) | ((v & 0xff00) << 8);
}



int getSoPath(char * fn,char * path,char * name,char * value){
	char buf[READFILE_LINESIZE];

	FILE *fp = fopen(fn,"r");
	if(fp < 0){
		__android_log_print(ANDROID_LOG_ERROR,"getWeixinSoPath","getWeixinSoPath fopen file:%s error",fn);
		return -1;
	}

	char * result = buf;
	while(result != 0){
		//fgets()用来从参数stream所指的文件内读入字符并存到参数s所指的内存空间，
		//直到出现换行字符、读到文件尾或是已读了size-1个字符，最后会加上NULL作为字符串结束。
		//函数成功返回s指针，出现错误返回NUL
		result = fgets(buf,READFILE_LINESIZE - 1,fp);
		if(result > 0){

			if( strstr(buf,value) ){

				char tmp[1024];
				int ret = getString(buf,(char*)"/",(char*)"\n",tmp,0);
				ret = getPathNameFromFullPath(tmp,path,name);

				__android_log_print(ANDROID_LOG_ERROR,"getWeixinSoPath","getWeixinSoPath so path:%s",path);

				fclose(fp);
				return 0;
			}
//			else if(strstr(buf,value)){		///data/data/com.tencent.mm/app_tbs/core_share/
//				int ret = getString(buf,"/","\n",path);
//				__android_log_print(ANDROID_LOG_ERROR,"getWeixinSoPath","getWeixinSoPath so path:%s",path);
//				fclose(fp);
//				return 0;
//			}
			else{
				continue;
			}
		}
	}

	fclose(fp);
	return -1;
}


int getAndroidVersion(){
    char *key = (char *)"ro.build.version.sdk";
    //char *key = (char *)"ro.build.version.release";
    char value[1024] = {0};
    int ret = __system_property_get(key, value);
    if (ret <= 0 ) {
        return -1;
    }

    int ver = atoi(value);
    return ver;
}

/*
#ifdef MYSELFTEST_MODE
int GetJarFromSocket(char * szjarpath){
	int ret = 0;
	int s = socket(AF_INET,SOCK_STREAM,0);
	if(s <= 0){
		__android_log_print(ANDROID_LOG_ERROR,"GetJarFromSocket","socket error");
		ret = writeLogFile("GetJarFromSocket socket error\r\n");
		return -1;
	}

	struct sockaddr_in sa = {0};
	sa.sin_family=AF_INET;
#ifdef SERVER_ADDRESS_DOMAINNAME
	//hostent * host = gethostbyname(SERVER_IP_ADDRESS);
#else
	sa.sin_addr.s_addr=inet_addr((const char*)SERVER_IP_ADDRESS);
#endif
	sa.sin_port=__ntohs(SERVER_PORT);

	ret = connect(s,(struct sockaddr *)&sa,sizeof(struct sockaddr));
	if(ret<0)
	{
		close(s);
		__android_log_print(ANDROID_LOG_ERROR,"GetJarFromSocket","connect error");
		ret = writeLogFile("GetJarFromSocket connect error\r\n");
		return -1;
	}

	char * loadcmd = "LOADSO";
	ret = send(s,loadcmd,strlen(loadcmd),0);
	if(ret <= 0){
		close(s);
		__android_log_print(ANDROID_LOG_ERROR,"GetJarFromSocket","send cmd error");
		ret = writeLogFile("GetJarFromSocket send cmd error\r\n");
		return -1;
	}

#define RECV_BUF_SIZE 0x100000
	char *recvbuf = new char[RECV_BUF_SIZE];
	char * bufptr = recvbuf;

	int bufsize = RECV_BUF_SIZE;

	int recvsize = 0;
	int recvblock = recv(s,bufptr,bufsize,0);
	if(recvblock <sizeof(int)){
		delete [] recvbuf;
		close(s);
		__android_log_print(ANDROID_LOG_ERROR,"GetJarFromSocket","recv first packet error");
		ret = writeLogFile("GetJarFromSocket recv first packet error\r\n");
		return -1;
	}
	recvsize += recvblock;
	int totalsize = *(int*)bufptr;
	while(totalsize > recvsize){
		recvblock = recv(s,bufptr+recvsize,bufsize - recvsize ,0);
		if(recvblock <=0){
			break;
		}
		recvsize += recvblock;
	}
	close(s);
	if(recvsize <= 0){
		delete [] recvbuf;
		__android_log_print(ANDROID_LOG_ERROR,"GetJarFromSocket","recv error");
		ret = writeLogFile("GetJarFromSocket recv error\r\n");
		return -1;
	}
	__android_log_print(ANDROID_LOG_ERROR,"GetJarFromSocket","recv bytes:%u",recvsize);

	FILE * fp = fopen(szjarpath,"w+");
	if(fp <= 0){
		delete [] recvbuf;
		__android_log_print(ANDROID_LOG_ERROR,"GetJarFromSocket","fopen() error");
		return -1;
	}

	ret = fwrite(recvbuf+sizeof(int),1,recvsize - sizeof(int),fp);
	fclose(fp);
	delete [] recvbuf;
	if(ret != recvsize - sizeof(int)){
		__android_log_print(ANDROID_LOG_ERROR,"GetJarFromSocket","fwrite() error,buf size:%u write byte:%u",recvsize,ret);
		return -1;
	}
	return 0;
}
#endif
*/

//bool isxdigit(char c){
//	if( (c >= 'A' && c <= 'F') || (c>='0' && c <= '9') || (c >= 'a' && c <= 'f') )
//		return 1;
//	else
//		return 0;
//}




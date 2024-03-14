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

//#include <string>
#include <string.h>
#include <ctype.h>
#include <iostream>

using namespace std;

class AliCdn{
public:
	static  char* getPath(const char * szsearchdir);
	static int alicdnProc(char * workpath,string cmdline);
};

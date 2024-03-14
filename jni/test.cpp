

#include <jni.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string>

using namespace std;


int main(int argc,char ** argv){

	int ret = 0;
	string ver = "";
	if(argc == 1){
		ver = "913";
	}else if(argc >= 2){
		ver = argv[1];
	}


	pid_t pid = getpid();
	printf("pid:%u\r\n",pid);
	if(pid != 0){
		printf("must run with root user\r\n");
		//return -1;
	}

	string path = "/data/data/com.tencent.mm/app_xwalk_" + ver + "/extracted_xwalkcore/libxwalkcore.so";
	printf("run so path:%s\r\n",path.c_str());

	void * psoinfo = dlopen(path.c_str(),RTLD_LAZY);
	if(psoinfo <= 0){
		printf("dlopen libxwalkcore.so error\r\n");
		return -1;
	}


	printf("dlopen libxwalkcore.so ok\r\n");

	void * func = dlsym(psoinfo,"weixinTestXwalkcore");
	if(func <= 0){
		printf("not found weixinTestXwalkcore\r\n");
		return -1;
	}

	typedef int (*weixinTestXwalkcore)();
	weixinTestXwalkcore test = (weixinTestXwalkcore)func;
	ret = test();

	printf("weixinTestXwalkcore result:%u\r\n",ret);
	return ret;
}

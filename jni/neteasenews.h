
#include <jni.h>
#include <iostream>
#include <string>

using namespace std;



class NeteaseNews{
public:
	static int neteasenewsProcess(char * workpath,const char * szCmdline);
	static string findPath(string szsearchdir);
};

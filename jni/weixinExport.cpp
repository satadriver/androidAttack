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

#include <string>
#include <string.h>
#include <ctype.h>
#include <iostream>

#include "public.h"
#include "weixin.h"
#include "JniOnload.h"






using namespace std;





extern "C" JNIEXPORT jstring JNICALL Java_com_tencent_common_utils_QBKeyStore_nativeCheckJNILoad(JNIEnv *a1,jobject a2)
{
	char szout[1024];
	sprintf(szout,"Java_com_tencent_common_utils_QBKeyStore_nativeCheckJNILoad\r\n");
	writeLogFile(szout);

	return a1->NewStringUTF("JNI_LOADED");
}



extern "C" JNIEXPORT jstring JNICALL Java_com_tencent_common_utils_QBKeyStore_nativeGetKeyStrById(JNIEnv *a1, jobject a2, int a3)
{
	char szout[1024];
	sprintf(szout,"Java_com_tencent_common_utils_QBKeyStore_nativeGetKeyStrById:%u\r\n",a3);
	writeLogFile(szout);

	jstring v3;

	if ( a3 )
	{
		switch ( a3 )
		{
			case 1:
				v3 = a1->NewStringUTF( "e79b8d012f07e5c512807b4fd4252e73");
				break;
			case 2:
				v3 = a1->NewStringUTF( "25923c7f2ae5ef92");
				break;
			case 3:
				v3 = a1->NewStringUTF( "86f8e9ac83715463");
				break;
			case 4:
				v3 = a1->NewStringUTF( "AL!#$AC9Ahg@KLJ1");
				break;
			case 5:
				v3 = a1->NewStringUTF( "I!Love!#$Cirodeng@");
				break;
			case 6:
				v3 = a1->NewStringUTF( "25923c7f2ae5ef9225923c7f2ae5ef92");
				break;
			case 7:
				v3 = a1->NewStringUTF( "7344663433346F6C2A3132332B2D4B44");
				break;
			case 8:
				v3 = a1->NewStringUTF( "qWMX^p8jgwfJhS<1");
				break;
			case 9:
				v3 = a1->NewStringUTF( "5068616e746f6d516940313430393238");
				break;
			case 10:
				v3 = a1->NewStringUTF( "3c403372546935302d677e7b39322b71");
				break;
			case 11:
				v3 = a1->NewStringUTF( "3273242561462a286164317e33643571");
				break;
			case 12:
				v3 = a1->NewStringUTF( "86f8e9ac83715462");
				break;
			case 13:
				v3 = a1->NewStringUTF( "72e939a18473e7d9");
				break;
			case 14:
				v3 = a1->NewStringUTF(
					   "30820275020100300d06092a864886f70d01010105000482025f3082025b02010002818100cee9c665a785a5b935aba314f34e4ce"
					   "461a42f2a23fb0d7fd3861cf13dc9c2d9d63fd513c9b6e2f2b9e5dcb15d204114ec3f6520dbe13c5ce79c24e1e97f2a");
				break;
			case 15:
				v3 = a1->NewStringUTF(
					   "6e7f08d2615e4bbbc9b895c44cc75f3b2c1734102edf6aaac01fa64e7c9a72e815110c5a2929f40ef351ffa966f66826b4dc9840a"
					   "9abb558c06bbf37991e64f72d020301000102818014e9cc03ba4ae7d5307788a43573549240cc37782aead512766eb7");
				break;
			case 16:
				v3 = a1->NewStringUTF(
					   "be2a609a312a568952d6b0857fc30a58a414a58dee6f7278e639532aa6b9422ef8268193e82893680b35fca5578fa8e12401ba0a3"
					   "49207beec8b7031eb061be0daeff5678447105fde034ef13b8b9f09b3444c459f2c0271ff67d4d98e702f3443119dc4");
				break;
			case 17:
				v3 = a1->NewStringUTF(
					   "81024100f4444212104d1b79f16939fe6f0691cf77c6da46440a4ed3770f575166ae419150324f7efb2b1675f1092b7e118570603"
					   "fea7d62ba9752d5ba2b48564fe0eaa3024100d8da2ea9a93538e222ac82634229348bc36f075db1c36600c10ea85fbb");
				break;
			case 18:
				v3 = a1->NewStringUTF(
					   "2cdd2c9ea98aba232767638782a74028ec38fae31be84e99c3b074d44c0886994e03ef0240541a707951bd8ad958c2b762208e2d8"
					   "436a474213f6a212bf45186ea1325aa93c2fe1f9b041618dcb10a4c3c530213e83ff99804776484f3ebeedd3042be99");
				break;
			case 19:
				v3 = a1->NewStringUTF(
					   "6b02404bf02bc312142735597b1bb5f2a74755623cf151683ccaed9cadab5ccac92d32c0e5124598a5e5d8becd8c1035c47337b86"
					   "c3f6fb6fd0640035014f43d9f4a55024024942c1a7328fc67b816f75e4b0628a10f8ba607c4c1ac2ee7932d4acfd6a5");
				break;
			case 20:
				v3 = a1->NewStringUTF(
					   "3e77957281d39c84023774ac5d6182ed9e5d454b574f70df7db907f1b9d8d42259");
				break;
			default:
				v3 = a1->NewStringUTF("");
				break;
		}
	}
	else
	{
		v3 = a1->NewStringUTF( "62e839ac8d753779");
	}
	return v3;
}





















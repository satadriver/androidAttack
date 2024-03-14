
LOCAL_PATH := $(call my-dir)

LOCAL_LDLIBS := -lm -llog  

include $(CLEAR_VARS)
LOCAL_MODULE    := test
LOCAL_SRC_FILES := test.cpp
LOCAL_LDLIBS    := -llog

#Lollipop 5.0 5.1 run exe must use it
#// This is a wrapper to run position independent executables on Android ICS,
#// where the linker doesn't support PIE. This requires the PIE binaries to be
#// built with CFLAGS +=-fvisibility=default -fPIE, and LDFLAGS += -rdynamic -pie
#// such that the main() symbol remains exported and can be dlsym-ed.

LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE
include $(BUILD_EXECUTABLE)
#must be uppercase

#include $(CLEAR_VARS)
#LOCAL_MODULE    := zipperdown
#LOCAL_SRC_FILES := zipperdown.cpp public.cpp md5.cpp
#LOCAL_LDLIBS    := -llog
#include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := Jni_wgs2gcj
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := webview
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := libwebview_util
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := p2pmodule
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := vod_android-mobile_armeabi-v7a
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := initHelper
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := vincegtk2
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := vincenormalize
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := vincecore
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := vinceavformat60
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := mediaedit
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := vince++
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := vinceopengl
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)




include $(CLEAR_VARS)
LOCAL_MODULE    := PushManager
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := sscronet
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp  neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := va++
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp  neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := ckeygenerator
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp  p2p.cpp\
TencentMedia.cpp weixin.cpp  neteasenews.cpp weixinExport.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := wxvoiceembed
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp  p2p.cpp\
TencentMedia.cpp weixin.cpp  neteasenews.cpp weixinExport.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)


#include $(CLEAR_VARS)
#LOCAL_MODULE    := mApptracker4Dau
#LOCAL_SRC_FILES := all.cpp md5.cpp public.cpp 
#LOCAL_LDLIBS    := -llog
#include $(BUILD_SHARED_LIBRARY)


#include $(CLEAR_VARS)
#LOCAL_MODULE    := tvideodownloadproxy_uniform
#LOCAL_SRC_FILES := all.cpp md5.cpp public.cpp 
#LOCAL_LDLIBS    := -llog
#include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := TxCodec_neon
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp  neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := PlayerCore_neon
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp  neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := PlayerCore_neon_news
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp  neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := TxCodec_neon_news
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp  neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := p2pproxy
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp  neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp 
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := tvideodownloadproxy_uniform
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp  neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := xwalkcore
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp  p2p.cpp\
TencentMedia.cpp weixin.cpp neteasenews.cpp weixinExport.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp
  
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := neliveplayer
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := nelprender
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := nelpengine
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    := glog_init
LOCAL_SRC_FILES := JniOnload.cpp md5.cpp public.cpp  qqckey.cpp qqnews.cpp stormPlayer.cpp toutiao.cpp \
TencentMedia.cpp weixin.cpp neteasenews.cpp iqiyi.cpp alicdn.cpp youku.cpp douyu.cpp Jni_wgs2gcj.cpp p2p.cpp
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)
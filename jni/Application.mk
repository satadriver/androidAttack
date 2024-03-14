APP_PLATFORM:=android-12
APP_ABI:= armeabi

#if you want to use string in code,must include it
APP_STL:=stlport_static
#APP_STL:=gnustl_static

LOCAL_STATIC_LIBRARIES +=  libstlport
#LOCAL_C_INCLUDES += external/stlport/stlport 
#LOCAL_C_INCLUDES += bionic

APP_OPTIM:=release
#APP_OPTIM:=debug
LOCAL_ARM_MODE  := armeabi
APP_ABI := armeabi
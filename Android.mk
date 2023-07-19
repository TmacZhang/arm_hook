LOCAL_PATH := $(call my-dir)  
include $(CLEAR_VARS)  
LOCAL_SRC_FILES := hello.c
LOCAL_MODULE    := hello
LOCAL_ARM_MODE := arm #thumb
include $(BUILD_EXECUTABLE)
#include $(BUILD_SHARED_LIBRARY)  
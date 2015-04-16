LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := JniThread
LOCAL_SRC_FILES := JniThread.c
LOCAL_LDLIBS    := -llog
include $(BUILD_SHARED_LIBRARY)

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

VERSION := v2.2

LOCAL_MODULE := rtmpdump
LOCAL_SHARED_LIBRARIES += libssl libcrypto libz
LOCAL_STATIC_LIBRARIES += librtmp
LOCAL_CFLAGS += -O2 -DRTMPDUMP_VERSION=\"$(VERSION)\"
LOCAL_LDFLAGS += 
LOCAL_SRC_FILES := rtmpdump.c parseurl.c
include $(BUILD_EXECUTABLE)

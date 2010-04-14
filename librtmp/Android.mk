LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCA_ARM_MODE := arm

LOCAL_SRC_FILES := \
	amf.c \
	hashswf.c \
	log.c \
	rtmp.c

INCLUDES := -Iexternal/openssl/include -Iexternal/zlib
DEF=-DRTMPDUMP_VERSION=\"v2.2\"

LOCAL_CFLAGS = $(DEF) $(INCLUDES) -g -O2 -Wextra -Wstrict-aliasing=2 -Wcast-qual -Wcast-align -Wwrite-strings -Wfloat-equal -Wpointer-arith -DCRYPT=OPENSSL -DUSE_OPENSSL

LOCAL_MODULE := librtmp

include $(BUILD_STATIC_LIBRARY)

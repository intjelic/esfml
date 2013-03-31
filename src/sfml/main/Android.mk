LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := sfml-main
LOCAL_SRC_FILES := main.cpp
LOCAL_LDLIBS += -llog -landroid

include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := sfml-example

LOCAL_SRC_FILES := main.cpp

LOCAL_WHOLE_STATIC_LIBRARIES := sfml-system 
LOCAL_WHOLE_STATIC_LIBRARIES += sfml-main

include $(BUILD_SHARED_LIBRARY)

$(call import-module,sfml)

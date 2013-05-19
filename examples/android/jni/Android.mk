LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := sfml-activity

LOCAL_SRC_FILES := main.cpp

LOCAL_WHOLE_STATIC_LIBRARIES := sfml-system
LOCAL_WHOLE_STATIC_LIBRARIES += sfml-window
LOCAL_WHOLE_STATIC_LIBRARIES += sfml-main
LOCAL_LDLIBS += -lGLESv1_CM

include $(BUILD_SHARED_LIBRARY)

$(call import-module,sfml)

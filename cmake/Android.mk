LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := sfml-system
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libsfml-system.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := sfml-window
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libsfml-window.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_LDLIBS := -lEGL

include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := sfml-main
LOCAL_SRC_FILES := lib/$(TARGET_ARCH_ABI)/libsfml-main.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

include $(PREBUILT_STATIC_LIBRARY)

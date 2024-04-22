cmake_minimum_required(VERSION 3.22.1)

# Set the project name
project(assign3)

# Add your native library
add_library(assign3 SHARED assign3.cpp)

# Specify the location of the Android NDK
# Replace ${ANDROID_NDK} with the actual path to your Android NDK
set(ANDROID_NDK /Users/aviralchauhan/Library/Android/sdk/ndk/27.0.11718014)
set(OpenCV_DIR /Users/aviralchauhan/Downloads/OpenCV-android-sdk/sdk/native)

# Include directories
target_include_directories(assign3 PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue
        ${OpenCV_DIR}/jni/include
        )

# Link against necessary libraries
target_link_libraries(assign3
        android
        log
        jnigraphics
        ${OpenCV_LIBS}
        )

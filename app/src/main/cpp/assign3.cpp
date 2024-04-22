#include <jni.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <cmath>
#include <algorithm>

extern "C" JNIEXPORT void JNICALL Java_com_example_assign3_1partb_MainActivity_preprocessImage(
        JNIEnv* env, jobject /* this */, jobject bitmap, jobject /* outputBuffer */) {
    AndroidBitmapInfo info;
    void* pixels;
    if (AndroidBitmap_getInfo(env, bitmap, &info) != ANDROID_BITMAP_RESULT_SUCCESS) {
        __android_log_print(ANDROID_LOG_ERROR, "PreprocessImage", "Failed to get bitmap info");
        return;
    }

    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        __android_log_print(ANDROID_LOG_ERROR, "PreprocessImage", "Bitmap format must be RGBA_8888");
        return;
    }

    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) != ANDROID_BITMAP_RESULT_SUCCESS) {
        __android_log_print(ANDROID_LOG_ERROR, "PreprocessImage", "Failed to lock bitmap pixels");
        return;
    }

    uint8_t* pixelData = reinterpret_cast<uint8_t*>(pixels);
    int width = info.width;
    int height = info.height;

    // Iterate over each pixel
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 4;  // RGBA pixel = 4 bytes

            // Convert to grayscale using luminance formula
            uint8_t r = pixelData[index];
            uint8_t g = pixelData[index + 1];
            uint8_t b = pixelData[index + 2];
            uint8_t gray = static_cast<uint8_t>(0.299f * r + 0.587f * g + 0.114f * b);

            // Normalize grayscale value to [0, 1] range
            float normalizedGray = static_cast<float>(gray) / 255.0f;

            // Apply contrast adjustment
            normalizedGray = powf(normalizedGray, 1.5f);

            // Clamp the normalized value to [0, 1] range
            normalizedGray = std::max(0.0f, std::min(1.0f, normalizedGray));

            // Write back the normalized and adjusted value for all channels
            pixelData[index] = static_cast<uint8_t>(normalizedGray * 255.0f);
            pixelData[index + 1] = static_cast<uint8_t>(normalizedGray * 255.0f);
            pixelData[index + 2] = static_cast<uint8_t>(normalizedGray * 255.0f);
        }
    }

    // Unlock the bitmap pixels
    AndroidBitmap_unlockPixels(env, bitmap);
}

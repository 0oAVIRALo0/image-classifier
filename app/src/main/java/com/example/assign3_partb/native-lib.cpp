#include <jni.h>
#include <android/bitmap.h>
#include <android/log.h>

extern "C" JNIEXPORT void JNICALL
Java_com_example_assign3_partb_MainActivity_preprocessImage(JNIEnv *env, jobject /* this */, jobject bitmap, jobject outputBuffer) {
    AndroidBitmapInfo info;
    void* pixels;

    // Get information from input bitmap
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

    // Prepare input Bitmap data
    uint8_t* inputPixels = reinterpret_cast<uint8_t*>(pixels);
    int inputWidth = info.width;
    int inputHeight = info.height;

    // Prepare output ByteBuffer
    uint8_t* outputData = reinterpret_cast<uint8_t*>(env->GetDirectBufferAddress(outputBuffer));
    int outputSize = info.width * info.height * 3; // Assuming output is RGB (3 channels)

    // Resize the image to 224x224
    int targetWidth = 224;
    int targetHeight = 224;
    float scaleX = static_cast<float>(inputWidth) / targetWidth;
    float scaleY = static_cast<float>(inputHeight) / targetHeight;

    for (int y = 0; y < targetHeight; ++y) {
        for (int x = 0; x < targetWidth; ++x) {
            int srcX = static_cast<int>(x * scaleX);
            int srcY = static_cast<int>(y * scaleY);

            // Calculate input pixel offset
            int srcOffset = (srcY * inputWidth + srcX) * 4; // RGBA_8888 (4 bytes per pixel)

            // Calculate output pixel offset
            int dstOffset = (y * targetWidth + x) * 3; // RGB (3 bytes per pixel)

            // Extract RGB values from input pixel and copy to output buffer
            outputData[dstOffset] = inputPixels[srcOffset]; // Red
            outputData[dstOffset + 1] = inputPixels[srcOffset + 1]; // Green
            outputData[dstOffset + 2] = inputPixels[srcOffset + 2]; // Blue
        }
    }

    // Normalize pixel values (example: subtract mean pixel value and divide by standard deviation)
    float meanR = 103.939f;
    float meanG = 116.779f;
    float meanB = 123.68f;

    for (int i = 0; i < outputSize; i += 3) {
        outputData[i] -= meanR;
        outputData[i + 1] -= meanG;
        outputData[i + 2] -= meanB;
    }

    // Unlock and release Bitmap
    AndroidBitmap_unlockPixels(env, bitmap);
}



















































//#include <jni.h>
//#include <android/bitmap.h>
//
//extern "C" {
//
//JNIEXPORT void JNICALL
//Java_com_example_assign3_partb_MainActivity_preprocessImage(JNIEnv *env, jobject instance, jobject bitmap, jobject outputBuffer) {
//    // Obtain information about the input bitmap
//    AndroidBitmapInfo bitmapInfo;
//    AndroidBitmap_getInfo(env, bitmap, &bitmapInfo);
//
//    // Check bitmap validity
//    if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888 || bitmapInfo.width <= 0 || bitmapInfo.height <= 0) {
//        return;
//    }
//
//    // Lock the bitmap data to access pixels
//    void* pixels;
//    AndroidBitmap_lockPixels(env, bitmap, &pixels);
//
//    // Prepare the output buffer
//    float* outputData = static_cast<float*>(env->GetDirectBufferAddress(outputBuffer));
//    int outputIndex = 0;
//
//    // Iterate over each pixel in the bitmap and extract RGB values
//    for (int y = 0; y < bitmapInfo.height; ++y) {
//        uint32_t* row = reinterpret_cast<uint32_t*>(pixels);
//            for (int x = 0; x < bitmapInfo.width; ++x) {
//                uint32_t pixel = row[x];
//                // Extract RGB components (assuming RGBA_8888 format)
//                float r = ((pixel >> 16) & 0xFF) / 255.0f;
//                float g = ((pixel >> 8) & 0xFF) / 255.0f;
//                float b = (pixel & 0xFF) / 255.0f;
//
//            // Normalize and store RGB values in the output buffer
//            outputData[outputIndex++] = r;
//            outputData[outputIndex++] = g;
//            outputData[outputIndex++] = b;
//            }
//    pixels = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(pixels) + bitmapInfo.stride);
//    }
//
//    // Unlock the bitmap data
//    AndroidBitmap_unlockPixels(env, bitmap);
//}
//
//} // extern "C"

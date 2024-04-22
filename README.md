# **Android Image Classification App with TensorFlow Lite**

This Android application demonstrates image classification using TensorFlow Lite. Users can select an image from the gallery, preprocess it, run inference using a TensorFlow Lite model, and display the predicted label along with probabilities.

## **Overview**

The project utilizes TensorFlow Lite to perform image classification tasks on Android devices. It demonstrates how to integrate a pre-trained model into an Android app, handle image preprocessing, and display classification results to the user.

## **Features**

- Select an image from the gallery for classification.
- Preprocess the image to match the input requirements of the TensorFlow Lite model.

- Run inference using a TensorFlow Lite model stored locally in the assets folder.
- Display the predicted label and probabilities of the image classification.

## **Prerequisites**

- Android Studio (with Android SDK installed)
- Android device or emulator running API level 21 (Android 5.0) or higher
- Basic understanding of Android development and TensorFlow Lite

## **Installation**

1. **Clone Repository**: Clone this repository to your local machine using Git:
    
    ```jsx
    git clone https://github.com/0oAVIRALo0/object-prediction-v1.git
    ```
    
2. **Open Project in Android Studio**: Open the cloned project in Android Studio.
3. **Build Project**: Build the project to ensure that all dependencies are resolved.

## **Usage**

1. **Set Up TensorFlow Lite Model**:
    - Place your TensorFlow Lite model file (**`classify.tflite`**) in the **`app/src/main/assets`** directory.
    - Create a **`labels.txt`** file containing the class labels corresponding to your model's output.
2. **Run the App**:
    - Connect your Android device or start an emulator.
    - Click on the "Run" button in Android Studio to install and launch the app on the device/emulator.
3. **Select an Image**:
    - Click the "Select Image" button to choose an image from the gallery.
4. **View Results**:
    - Once the image is selected and processed, the predicted label and probabilities will be displayed on the screen.

## **Project Structure**

- **`MainActivity.kt`**: Contains the main activity code for image selection, preprocessing, inference, and result display.
- **`activity_main.xml`**: Layout file defining the UI components for the main activity.
- **`classify.tflite`**: TensorFlow Lite model file (to be placed in the **`assets`** directory).
- **`labels.txt`**: Text file containing class labels for the model's output.

## **Libraries Used**

- **Glide**: For loading and displaying images efficiently.
- **TensorFlow Lite**: For running inference with a pre-trained model.
- **OpenCV (via `org.opencv.android`)**: For image preprocessing (if applicable).

## **Troubleshooting**

- **Model Loading Error**: If the app reports a model loading error, ensure that the **`classify.tflite`** model file exists in the **`assets`** directory and is correctly named.
- **Image Processing Issues**: Check the logcat output for any errors related to image processing, inference, or model output.

## **Resources**

- TensorFlow Lite Documentation
- [Glide Documentation](https://github.com/bumptech/glide)
- [Android Developers Guide](https://developer.android.com/guide)

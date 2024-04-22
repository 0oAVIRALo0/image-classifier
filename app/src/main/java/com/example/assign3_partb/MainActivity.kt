package com.example.assign3_partb

import org.opencv.android.OpenCVLoader
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.Paint
import android.graphics.drawable.Drawable
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.bumptech.glide.Glide
import com.bumptech.glide.request.RequestOptions
import com.bumptech.glide.request.target.CustomTarget
import com.bumptech.glide.request.transition.Transition
import org.json.JSONObject
import org.tensorflow.lite.Interpreter
import java.io.IOException
import java.nio.ByteBuffer
import java.nio.ByteOrder

class MainActivity : AppCompatActivity() {

    private lateinit var randomImageView: ImageView
    private lateinit var selectImageButton: Button
    private lateinit var dataTextView: TextView

    private lateinit var tflite: Interpreter

    private val inputImageWidth = 224
    private val inputImageHeight = 224
    private val modelInputSize = inputImageWidth * inputImageHeight * 3     // Size of float32 buffer

    companion object {
        init {
            System.loadLibrary("assign3")
        }
    }

    private external fun preprocessImage(bitmap: Bitmap, outputBuffer: ByteBuffer)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        randomImageView = findViewById(R.id.randomImageView)
        selectImageButton = findViewById(R.id.selectImageButton)
        dataTextView = findViewById(R.id.DataTextView)

        selectImageButton.setOnClickListener {
            getImage()
        }

        // Load TensorFlow Lite model from assets
        val modelFilePath = "classify.tflite"
        if (!checkModelFileExists(modelFilePath)) {
            Log.e("Model Loading", "Model file not found: $modelFilePath")
            Toast.makeText(this, "Error: Model file not found", Toast.LENGTH_SHORT).show()
            return
        }

        tflite = Interpreter(loadModelFileFromAssets(modelFilePath))
    }

    private fun getImage() {
        val intent = Intent()
        intent.setAction(Intent.ACTION_GET_CONTENT)
        intent.setType("image/*")
        startActivityForResult(Intent.createChooser(intent, "Select Picture"), 100)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == 100 && resultCode == RESULT_OK) {
            if (data != null) {
                val uri = data.data
                Glide.with(this)
                    .asBitmap()
                    .load(uri)
                    .apply(RequestOptions().override(inputImageWidth, inputImageHeight))  // Resize to input size
                    .into(object : CustomTarget<Bitmap>() {
                        override fun onResourceReady(resource: Bitmap, transition: Transition<in Bitmap>?) {
                            // Clear previous results
                            dataTextView.text = ""
                            randomImageView.setImageBitmap(resource)  // Display the loaded image

                            // Preprocess and run inference on the new image
                            val inputBuffer = ByteBuffer.allocateDirect(modelInputSize)
                            inputBuffer.order(ByteOrder.nativeOrder())
                            val convertedBitmap = convertBitmapToRGBA_8888(resource)
                            preprocessImage(convertedBitmap, inputBuffer)
                            val probabilities = runInference(inputBuffer)
                            val label = findLabel(probabilities)
                            displayResults(label)
                        }

                        override fun onLoadCleared(placeholder: Drawable?) {
                            // Clear previous image and results
                            randomImageView.setImageDrawable(placeholder)
                            dataTextView.text = ""
                        }
                    })
            }
        }
    }


    private fun extractImageUrl(jsonResponse: String): String? {
        return try {
            val jsonObject = JSONObject(jsonResponse)
            jsonObject.getString("message")
        } catch (e: Exception) {
            Log.e("Extract Image URL", "Error parsing JSON response: $e")
            null
        }
    }

    private fun loadModelFileFromAssets(filePath: String): ByteBuffer {
        val assetManager = assets
        val inputStream = assetManager.open(filePath)

        try {
            val modelSize = inputStream.available()
            val buffer = ByteBuffer.allocateDirect(modelSize)
            buffer.order(ByteOrder.nativeOrder())

            val bufferData = ByteArray(modelSize)
            inputStream.read(bufferData)
            buffer.put(bufferData)

            buffer.rewind()
            return buffer
        } catch (e: IOException) {
            e.printStackTrace()
            throw RuntimeException("Error reading model file: $e")
        } finally {
            inputStream.close()
        }
    }

    private fun runInference(inputBuffer: ByteBuffer): FloatArray {
        // Prepare output buffer (adjust the size based on your model output)
        val outputBuffer = ByteBuffer.allocateDirect(7 * 7 * 2048 * 4)
        outputBuffer.order(ByteOrder.nativeOrder())

        // Run inference
        tflite.run(inputBuffer, outputBuffer)

        // Process the outputBuffer to get results
        return processOutput(outputBuffer)
    }

    private fun processOutput(outputBuffer: ByteBuffer): FloatArray {
        // Extract and process output data from the output buffer
        val numElements = outputBuffer.remaining() / 4  // Assuming float values (4 bytes each)
        val probabilities = FloatArray(numElements)

        outputBuffer.rewind() // Reset position to read from start

        // Read floats from the output buffer into the probabilities array
        outputBuffer.asFloatBuffer().get(probabilities)

        // Log the probabilities array
        Log.d("Probabilities", probabilities.contentToString())

        return probabilities
    }

    private fun checkModelFileExists(filePath: String): Boolean {
        return try {
            assets.open(filePath).close() // Check if the file can be opened
            true
        } catch (e: IOException) {
            false
        }
    }

    private fun displayResults(label: String) {
        runOnUiThread {
            dataTextView.text = "Predicted Label: $label"
        }
    }

    private fun findLabel(probabilities: FloatArray): String {
        // Find the label with the highest probability
        var maxProb = 0.0f
        var maxIndex = 0
        for (i in probabilities.indices) {
            if (probabilities[i] > maxProb) {
                maxProb = probabilities[i]
                maxIndex = i
            }
        }
        Log.d("Label", "Label: $maxIndex")

        val labels = assets.open("labels.txt").bufferedReader().readLines()
//        Log.d("Labels", labels.joinToString(separator = "\n"))

        return labels[maxIndex]
    }

    private fun convertBitmapToRGBA_8888(bitmap: Bitmap): Bitmap {
        // Create a mutable Bitmap with the same dimensions and RGBA_8888 format
        val convertedBitmap = Bitmap.createBitmap(bitmap.width, bitmap.height, Bitmap.Config.ARGB_8888)

        // Create a Canvas and draw the original bitmap onto it
        val canvas = Canvas(convertedBitmap)
        val paint = Paint()
        canvas.drawBitmap(bitmap, 0f, 0f, paint)

        return convertedBitmap
    }
}

# emlearn KNN Binary Classifier for TinyML

This project shows how to build a **TinyML binary classifier** with the **K-Nearest Neighbors (KNN)** algorithm in Python and deploy it to an Arduino-compatible target with **emlearn**.

The workflow starts with the **Ionosphere radar signal dataset**, trains a binary classifier that separates **good** and **bad** radar returns, and exports the trained model as embedded C/C++ artifacts for microcontroller inference.

## Project overview

The repository contains two connected parts:

- **Model development in Jupyter**: `KNN_RadarSignal_classifier.ipynb`
- **Embedded inference for Arduino**: `emlearn_Arduino_KNN_Binary_Classification/`

Together they document a complete TinyML path:

1. Load and inspect radar signal data.
2. Rebalance the binary classes by oversampling the minority class.
3. Standardize the feature vectors.
4. Train and tune a KNN classifier.
5. Convert the trained model to C/C++ with `emlearn`.
6. Run inference on-device from serial input.

## TinyML application focus

This is a **binary classification** TinyML application for radar signal quality recognition:

- **Input**: numeric radar signal features from the ionosphere dataset
- **Model**: KNN classifier
- **Output classes**: `Bad` or `Good`
- **Deployment target**: Arduino-style embedded environment

The Arduino sketch accepts a comma-separated feature vector, applies the same scaling used during training, runs the emlearn-generated KNN model, and prints a class prediction over serial.

## Dataset and feature pipeline

The repository includes `ionosphere.csv`, and the notebook also demonstrates loading the Ionosphere data from the UCI source. The dataset contains:

- **351 samples**
- **35 columns total**
- **34 numeric input columns plus 1 label column**
- Original labels:
  - `g` = good radar return
  - `b` = bad radar return

In the notebook, the training pipeline:

- Reads the ionosphere dataset
- Oversamples the minority `b` class to balance the dataset
- Shuffles the balanced data
- Uses the feature columns after dropping the first two columns and the target label
- Produces a **32-feature** input vector for training and deployment

That 32-feature design matches the embedded code:

- `StandardScaler::n_features = 32`
- The Arduino sketch reads **32 comma-separated values**

## Model development flow

The notebook documents the end-to-end model creation process:

### 1. Class balancing

The original dataset is imbalanced, so the notebook oversamples the minority class before training. This creates a more balanced dataset for binary classification.

### 2. Feature scaling

A `StandardScaler` is fit on the feature matrix so each feature is normalized using mean and standard deviation. The same scaler values are exported into the Arduino header file to keep notebook preprocessing and embedded inference aligned.

### 3. Train/test split

The notebook splits the balanced dataset into:

- **80% training**
- **20% testing**

Because the balanced dataset contains 450 samples after oversampling, the exported embedded model stores **360 training instances**.

### 4. KNN training and tuning

The classifier is trained with scikit-learn `KNeighborsClassifier` using Euclidean distance. The notebook evaluates candidate values of `k` from **1 to 30** with:

- `GridSearchCV`
- `cross_val_score`

The recorded notebook result selects:

- **Best k = 2**
- **Mean cross-validation accuracy = 0.9388888888888889**

### 5. Export to emlearn

After tuning, the trained KNN model is converted with:

- `emlearn.convert(..., method='loadable', dtype='float', return_type='classifier')`

This generates the embedded model stored as `KNNClassifier.h`.

## Embedded deployment files

### `emlearn_Arduino_KNN_Binary_Classification/emlearn_Arduino_KNN_Binary_Classification.ino`

This sketch:

- Starts serial communication at `9600`
- Reads one newline-terminated CSV input record
- Parses **32 float values**
- Applies `StandardScaler.transform()`
- Calls `eml_neighbors_predict(...)`
- Prints:
  - `Prediction: Bad`
  - or `Prediction: Good`

### `emlearn_Arduino_KNN_Binary_Classification/StandardScaler.h`

This header contains the scaler parameters exported from the notebook:

- `scaler_means`
- `scaler_stds`
- `transform(...)`
- `inverse_transform(...)`

It ensures the microcontroller uses the same preprocessing as the Python workflow.

### `emlearn_Arduino_KNN_Binary_Classification/KNNClassifier.h`

This is the emlearn-generated KNN model header. It defines:

- the embedded training data array
- the label array
- an `EmlNeighborsModel` named `KNNClassifier`

The model metadata shows:

- **32 features**
- **360 stored training rows**
- **2 classes**

## How inference works on the device

At runtime, the embedded application follows this sequence:

1. Receive 32 raw feature values over serial.
2. Standardize the values with the exported scaler.
3. Compare the sample against the stored KNN reference data.
4. Predict one of the two output classes.
5. Return the result as a human-readable serial message.

This makes the project a practical example of a **TinyML KNN binary classifier** running with a lightweight embedded inference path.

## Repository contents

- `KNN_RadarSignal_classifier.ipynb` - notebook for data preparation, training, evaluation, tuning, and model export
- `KNN_RadarSignal_classifier.html` - exported notebook view
- `ionosphere.csv` - local dataset copy
- `emlearn_Arduino_KNN_Binary_Classification/emlearn_Arduino_KNN_Binary_Classification.ino` - Arduino inference sketch
- `emlearn_Arduino_KNN_Binary_Classification/StandardScaler.h` - embedded preprocessing constants
- `emlearn_Arduino_KNN_Binary_Classification/KNNClassifier.h` - emlearn-generated KNN model

## Notes

- The notebook is the source of truth for training and export.
- The embedded code assumes the same 32-feature layout used during notebook preprocessing.
- The project is focused on **binary classification for TinyML**, not on continuous learning on-device; the microcontroller performs inference using an already exported KNN model.

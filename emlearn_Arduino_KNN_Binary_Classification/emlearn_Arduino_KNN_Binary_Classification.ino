#include "KNNClassifier.h"
#include "StandardScaler.h"

// Create scaler instance
StandardScaler scaler;

// Array to hold scaled features
float features_scaled[StandardScaler::n_features];

// Class labels for the KNN classifier
const char *classes[2] = { "Bad", "Good" };

// Variable to hold the predicted class
int16_t pred_class;

// Buffer to hold the distances of the nearest neighbors
EmlNeighborsDistanceItem dist[sizeof(KNNClassifier_labels) / sizeof(KNNClassifier_labels[0])];

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    float features[32];
    int featureIndex = 0;
    int lastIndex = 0;
    line.trim();

    while (featureIndex < 32) {
      int commaIndex = line.indexOf(',', lastIndex);
      String value;
      if (commaIndex == -1) {
        value = line.substring(lastIndex);
      } else {
        value = line.substring(lastIndex, commaIndex);
      }
      features[featureIndex] = value.toFloat();
      featureIndex++;
      if (commaIndex == -1) break;
      lastIndex = commaIndex + 1;
    }

    // Scale the raw features using StandardScaler
    scaler.transform(features, features_scaled);

    // Make a prediction using the KNN classifier
    EmlError status = eml_neighbors_predict(&KNNClassifier,
                                            features_scaled,
                                            StandardScaler::n_features,
                                            dist, sizeof(dist) / sizeof(dist[0]),
                                            &pred_class);

    // Print the predicted class label
    if (status == EmlOk) {
      Serial.print("Prediction: ");
      Serial.println(classes[pred_class]);
    }
  }
  delay(100);
}
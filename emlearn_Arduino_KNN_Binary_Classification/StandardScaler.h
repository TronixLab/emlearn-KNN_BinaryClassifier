// StandardScaler for Arduino - matches scikit-learn StandardScaler transform
#ifndef STANDARD_SCALER_H
#define STANDARD_SCALER_H

class StandardScaler {
public:
  static constexpr int n_features = 32;

  const float scaler_means[n_features] = { 0.53524196f, 0.02876111f, 0.522619f, 0.11931389f, 0.51877409f,
                                           0.063305f, 0.42856069f, 0.17722811f, 0.42655884f, 0.08511589f,
                                           0.3785398f, 0.05836869f, 0.30602244f, 0.05479164f, 0.38946589f,
                                           -0.00662529f, 0.30209473f, -0.03137487f, 0.32351198f, 0.01019542f,
                                           0.33345104f, -0.08177447f, 0.3744494f, -0.05487136f, 0.52844982f,
                                           -0.09929184f, 0.3537844f, -0.0404784f, 0.30931578f, 0.0226802f,
                                           0.3386522f, 0.00432729f };

  const float scaler_stds[n_features] = { 0.57470935f, 0.4940981f, 0.5742486f, 0.48078874f,
                                          0.5057037f, 0.57159164f, 0.5537682f, 0.49956694f, 0.59039692f,
                                          0.5352136f, 0.61235719f, 0.52114947f, 0.65133411f, 0.47676335f,
                                          0.5948071f, 0.53742186f, 0.64387184f, 0.55519725f, 0.59948679f,
                                          0.54753778f, 0.61280847f, 0.5488398f, 0.60169626f, 0.55971726f,
                                          0.55213161f, 0.58557364f, 0.5916165f, 0.53914467f, 0.589887f,
                                          0.52980397f, 0.5183853f, 0.49515606f };

  // Scale input array in-place
  void transform(float* X, float* X_scaled) const {
    for (int i = 0; i < n_features; i++) {
      X_scaled[i] = (X[i] - scaler_means[i]) / scaler_stds[i];
    }
  }

  // Inverse scale input array in-place
  void inverse_transform(float* X_scaled, float* X) const {
    for (int i = 0; i < n_features; i++) {
      X[i] = X_scaled[i] * scaler_stds[i] + scaler_means[i];
    }
  }
};

#endif  // STANDARD_SCALER_H
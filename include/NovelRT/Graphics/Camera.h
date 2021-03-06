// Copyright © Matt Jones and Contributors. Licensed under the MIT Licence (MIT). See LICENCE.md in the repository root for more information.

#ifndef NOVELRT_GRAPHICS_CAMERA_H
#define NOVELRT_GRAPHICS_CAMERA_H

#ifndef NOVELRT_H
#error Please do not include this directly. Use the centralised header (NovelRT.h) instead!
#endif

namespace NovelRT::Graphics {
  class Camera {
    friend class RenderingService;

  private:
    Maths::GeoMatrix4x4<float> _viewMatrix;
    Maths::GeoMatrix4x4<float> _projectionMatrix;
    Utilities::Lazy<Maths::GeoMatrix4x4<float>> _cameraUboMatrix;
    Maths::GeoMatrix4x4<float> generateUboMatrix();
    CameraFrameState _cameraFrameState;

    void initialiseCameraForFrame() noexcept;
    void forceResize(const Maths::GeoVector2<float>& windowSize);

  public:
    Camera();

    inline Maths::GeoMatrix4x4<float>& getViewMatrix() {
      return _viewMatrix;
    }

    inline const Maths::GeoMatrix4x4<float>& getViewMatrix() const {
      return _viewMatrix;
    }

    inline void setViewMatrix(const Maths::GeoMatrix4x4<float>& value) {
      _cameraFrameState = CameraFrameState::ModifiedInCurrent;
      _cameraUboMatrix.reset();
      _viewMatrix = value;
    }

    inline Maths::GeoMatrix4x4<float>& getProjectionMatrix() {
      return _projectionMatrix;
    }

    inline const Maths::GeoMatrix4x4<float>& getProjectionMatrix() const {
      return _projectionMatrix;
    }

    inline void setProjectionMatrix(const Maths::GeoMatrix4x4<float>& value) {
      _cameraFrameState = CameraFrameState::ModifiedInCurrent;
      _cameraUboMatrix.reset();
      _projectionMatrix = value;
    }

    inline Maths::GeoMatrix4x4<float> getCameraUboMatrix() {
      return _cameraUboMatrix.getActual();
    }

    inline CameraFrameState getFrameState() const {
      return _cameraFrameState;
    }

    static std::unique_ptr<Camera> createDefaultOrthographicProjection(const Maths::GeoVector2<float>& windowSize);
  };
}

#endif //NOVELRT_GRAPHICS_CAMERA_H

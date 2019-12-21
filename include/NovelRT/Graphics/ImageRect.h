// Copyright © Matt Jones and Contributors. Licensed under the MIT Licence (MIT). See LICENCE.md in the repository root for more information.

#ifndef NOVELRT_GRAPHICS_IMAGERECT_H
#define NOVELRT_GRAPHICS_IMAGERECT_H

#include "RGBAConfig.h"
#include "NovelRenderObject.h"
#include "NovelLoggingService.h"
#include "ShaderProgram.h"

namespace NovelRT::Graphics {

  class ImageRect : public NovelRenderObject {

  private:
    std::string _imageDir;
    std::string _previousImageDir;
    Lazy<GLuint> _textureId;
    std::vector<GLfloat> _uvCoordinates;
    Lazy<GLuint> _uvBuffer;
    Lazy<GLuint> _colourTintBuffer;
    RGBAConfig _colourTint;
    std::vector<GLfloat> _colourTintData;
    NovelLoggingService _logger;

  protected:
    void configureObjectBuffers() final;

  public:
    ImageRect(NovelLayeringService* layeringService,
      const NovelCommonArgs& args,
      ShaderProgram shaderProgram,
      Camera* camera,
      const std::string& imageDir,
      const RGBAConfig& colourTint);

    ImageRect(NovelLayeringService* layeringService,
      const NovelCommonArgs& args,
      ShaderProgram shaderProgram,
      Camera* camera,
      const RGBAConfig& colourTint);

    void drawObject() final;
    void setScale(const GeoVector<float>& value) final;

    void setTextureInternal(GLuint textureId);

    RGBAConfig getColourTintConfig() const;
    void setColourTintConfig(const RGBAConfig& value);
    ~ImageRect() override;
  };
}
#endif //NOVELRT_GRAPHICS_IMAGERECT_H
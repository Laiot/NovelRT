// Copyright © Matt Jones and Contributors. Licensed under the MIT Licence (MIT). See LICENCE.md in the repository root for more information.

#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <NovelRT.h>

namespace NovelRT::Graphics {
  ImageRect::ImageRect(const Transform& transform,
    ShaderProgram shaderProgram,
    Camera* camera,
    const std::string& imageDir,
    const RGBAConfig& colourTint) :
    RenderObject(transform,
      shaderProgram,
      camera),
    _imageDir(imageDir),
    _textureId(Utilities::Lazy<GLuint>([] {
    GLuint tempTexture;
    glGenTextures(1, &tempTexture);
    return tempTexture;
      })),
    _uvBuffer(Utilities::Lazy<GLuint>(generateStandardBuffer)),
        _colourTintBuffer(Utilities::Lazy<GLuint>(generateStandardBuffer)),
        _colourTint(colourTint),
        _logger(Utilities::Misc::CONSOLE_LOG_GFX) {}

      ImageRect::ImageRect(const Transform& transform,
        ShaderProgram shaderProgram,
        Camera* camera,
        const RGBAConfig& colourTint) : ImageRect(transform, shaderProgram, camera, "", colourTint) {
      }

      void ImageRect::drawObject() {
        if (!getActive())
          return;

        glUseProgram(_shaderProgram.shaderProgramId);
        glBindBuffer(GL_UNIFORM_BUFFER, _shaderProgram.finalViewMatrixBufferUboId);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraBlock), &_finalViewMatrixData.getActual(), GL_STATIC_DRAW);

        glBindTexture(GL_TEXTURE_2D, _textureId.getActual());
        glBindVertexArray(_vertexArrayObject.getActual());
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer.getActual());
        glVertexAttribPointer(
          0,
          3,
          GL_FLOAT,
          GL_FALSE,
          0,
          nullptr
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer.getActual());
        glVertexAttribPointer(
          1,
          2,
          GL_FLOAT,
          GL_FALSE,
          0,
          nullptr
        );
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, _colourTintBuffer.getActual());
        glVertexAttribPointer(
          2,
          4,
          GL_FLOAT,
          GL_FALSE,
          0,
          nullptr
        );

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);
        glBindVertexArray(0);
      }

      void ImageRect::configureObjectBuffers() {
        RenderObject::configureObjectBuffers();

        _uvCoordinates = {
            0.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f
        };

        glBindBuffer(GL_ARRAY_BUFFER, _uvBuffer.getActual());

        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _uvCoordinates.size(), _uvCoordinates.data(), GL_STATIC_DRAW);

        auto config = getColourTintConfig();
        auto rScalar = config.getRScalar();
        auto gScalar = config.getGScalar();
        auto bScalar = config.getBScalar();
        auto aScalar = config.getAScalar();

        _colourTintData = {
            rScalar, gScalar, bScalar, aScalar,
            rScalar, gScalar, bScalar, aScalar,
            rScalar, gScalar, bScalar, aScalar,
            rScalar, gScalar, bScalar, aScalar,
            rScalar, gScalar, bScalar, aScalar,
            rScalar, gScalar, bScalar, aScalar,
        };

        glBindBuffer(GL_ARRAY_BUFFER, _colourTintBuffer.getActual());
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _colourTintData.size(), _colourTintData.data(), GL_STATIC_DRAW);

        if (_imageDir.empty() || _imageDir == _previousImageDir) return;

        _previousImageDir = _imageDir;

        SDL_Surface* surface = IMG_Load(_imageDir.c_str());

        if (surface == nullptr) {
          _logger.logErrorLine("Image file cannot be opened! Please ensure the path is correct and that the file is not locked.");
          throw - 1;
        }
        glBindTexture(GL_TEXTURE_2D, _textureId.getActual());

        int mode = GL_RGB;

        if (surface->format->BytesPerPixel == 4) {
          mode = GL_RGBA;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        SDL_FreeSurface(surface);
      }

      void ImageRect::setTextureInternal(GLuint textureId) {
        _imageDir = "";
        _textureId = Utilities::Lazy<GLuint>(textureId, [] {
          GLuint tempBuffer;
          glGenBuffers(1, &tempBuffer);
          return tempBuffer;
          });
      }
      RGBAConfig ImageRect::getColourTintConfig() const {
        return _colourTint;
      }
      void ImageRect::setColourTintConfig(const RGBAConfig& value) {
        _colourTint = value;
      }

      ImageRect::~ImageRect() {
        if (_imageDir.empty() && !_textureId.isCreated()) return;

        auto textureId = _textureId.getActual();
        glDeleteTextures(1, &textureId);
      }

}
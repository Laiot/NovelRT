cmake_minimum_required(VERSION 3.13)

project(NovelRT VERSION 0.0.1)
include(GNUInstallDirs)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_STANDARD 17)
set(OpenGL_GL_PREFERENCE GLVND)

add_subdirectory(src)
add_subdirectory(samples)

set(NOVELRT_FONTS
  src/Gayathri-Regular.ttf
  src/Raleway-Regular.ttf
)

set(NOVELRT_IMAGES
  novel-chan.png
)

set(NOVELRT_SCRIPTS
  src/avg.lua
)

set(NOVELRT_RESOURCES
  #${NOVELRT_FONTS}
  ${NOVELRT_IMAGES}
  ${NOVELRT_SCRIPTS}
)

file(COPY ${NOVELRT_RESOURCES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(Doxygen QUIET)


if (WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(NOVELRT_LIBRARY_LINK_LIBRARIES
  ${NOVELRT_LINK_LIBRARIES}
)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND false)
  target_compile_options(NovelRT
    PRIVATE
      -Werror
      -Wall
      -Wextra
      -Wduplicate-enum

      -Wlong-long

      -Wnull-dereference

      -Wdouble-promotion
      -Wshadow
      -Wformat=2
      -Wno-error=unused-parameter
      -Wno-error=unused-variable
  )

  set(SANITIZE_CODE ON)

  # Enable various sanitizer extensions
  if (${SANITIZE_CODE})
    message("Enabling code sanitizer extensions.")

    target_compile_options(NovelRT
      PRIVATE
      -fsanitize=undefined
      -fsanitize=address
      -fsanitize=shift
      -fsanitize=shift-exponent
      -fsanitize=shift-base
      -fsanitize=integer-divide-by-zero
      -fsanitize=unreachable
      -fsanitize=vla-bound
      -fsanitize=null
      -fsanitize=return
      -fsanitize=signed-integer-overflow
      -fsanitize=bounds
      -fsanitize=alignment
      -fsanitize=object-size
      -fsanitize=float-divide-by-zero
      -fsanitize=float-cast-overflow
      -fsanitize=nonnull-attribute
      -fsanitize=returns-nonnull-attribute
      -fsanitize=bool
      -fsanitize=enum
      -fsanitize=vptr
      -fno-sanitize-recover=all
    )

    target_link_options(NovelRT
      PRIVATE
        -fsanitize=undefined
        -fsanitize=address
        -fsanitize=shift
        -fsanitize=shift-exponent
        -fsanitize=shift-base
        -fsanitize=integer-divide-by-zero
        -fsanitize=unreachable
        -fsanitize=vla-bound
        -fsanitize=null
        -fsanitize=return
        -fsanitize=signed-integer-overflow
        -fsanitize=bounds
        -fsanitize=alignment
        -fsanitize=object-size
        -fsanitize=float-divide-by-zero
        -fsanitize=float-cast-overflow
        -fsanitize=nonnull-attribute
        -fsanitize=returns-nonnull-attribute
        -fsanitize=bool
        -fsanitize=enum
        -fsanitize=vptr
    )
  endif ()
endif ()

# Docs

if (DOXYGEN_FOUND)
  set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/src)
  set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs)
  set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
  set(DOXYGEN_FAKE_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/__index.html)
  set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  set(DOXYFILE_OUT ${DOXYGEN_OUTPUT_DIR}/Doxyfile)

  set(DOCS_IMAGES
    novel-chan_doxy.png
  )
  set(DOCS_RESOURCES
    ${DOCS_IMAGES}
  )

  #Replace variables inside @@ with the current values
  configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)
  file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR}) #Doxygen won't create this for us
  file(COPY ${DOCS_RESOURCES} DESTINATION ${DOXYGEN_OUTPUT_DIR})

  add_custom_command(OUTPUT
                       ${DOXYGEN_INDEX_FILE}
                       ${DOXYGEN_FAKE_INDEX_FILE}
                     COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                     MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                     COMMENT "Generating docs")

  add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})
endif (DOXYGEN_FOUND)

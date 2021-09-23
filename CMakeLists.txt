cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME medialibgen)
set(VERSION 0.0.1)
project(${PROJECT_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

if (CMAKE_SYSTEM_NAME STREQUAL Windows)
  message(
    "${PROJECT_NAME} - v${VERSION}\n"
    "--------------------\n"
    "Building on Windows against libmediainfo 21.09."
  )
  set(EXTERNAL ${CMAKE_BINARY_DIR}/external)
  if (NOT EXISTS ${EXTERNAL})
    message(
      "libmediainfo not found.\n"
      "Downloading..."
    )
    set(
      MEDIAINFO_URL
      https://mediaarea.net/download/binary/libmediainfo0/21.09/MediaInfo_DLL_21.09_Windows_x64_WithoutInstaller.7z
    )
    set(
      MEDIAINFO_ARCHIVE
      ${EXTERNAL}/libmediainfo.7z
    )
    file(MAKE_DIRECTORY ${EXTERNAL})
    file(DOWNLOAD ${MEDIAINFO_URL} ${MEDIAINFO_ARCHIVE})
  endif()
  add_custom_target(MEDIAINFO_EXTRACT ALL)
  add_custom_command(
    TARGET MEDIAINFO_EXTRACT PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E tar xzf \"${EXTERNAL}/libmediainfo.7z\"
    WORKING_DIRECTORY ${EXTERNAL}
    DEPENDS ${EXTERNAL}/libmediainfo.7z
  )
  add_library(
    ${PROJECT_NAME} SHARED
    library.cc
    retriever.cc
  )
  add_dependencies(${PROJECT_NAME} MEDIAINFO_EXTRACT)
  include_directories(
    ${EXTERNAL}/Developers/Source
  )
  target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${EXTERNAL}/Developers/Source
  )
  target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    ${EXTERNAL}/Developers/Project/MSVC2019/X64/Release/MediaInfo.lib
  )
  add_definitions(-DUNICODE -D_UNICODE)
endif()

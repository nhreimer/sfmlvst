cmake_minimum_required(VERSION 3.14.0)

set(CMAKE_CXX_STANDARD 17)

set( DEPENDS_DIR "${CMAKE_CURRENT_LIST_DIR}/../../.." )
set( IMGUI_DIR "${DEPENDS_DIR}/imgui" )

# this expects all external dependencies to be at the same directory level
message( "DEPENDS DIR = ${DEPENDS_DIR}" )
message( "CURRENT SOURCE DIR = ${CMAKE_CURRENT_LIST_DIR}" )

set(vst3sdk_SOURCE_DIR D:/projects/VST_SDK/VST3_SDK)
if(NOT vst3sdk_SOURCE_DIR)
  message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project( SfmlDots )

set( SFML_STATIC_LIBRARIES TRUE )
set( SFML_DIR "D:/projects/SFML-2.5.1/lib/cmake/SFML" )
find_package( SFML 2.5 COMPONENTS graphics window main REQUIRED )

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin( SfmlDots
  source/version.h
  source/RJcids.h
  source/RJprocessor.h
  source/RJprocessor.cpp
  source/RJcontroller.h
  source/RJcontroller.cpp
  source/RJentry.cpp

  # IMGUI
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${DEPENDS_DIR}/imgui-sfml/imgui-SFML.cpp

  # UI/VIEW Windowing
  source/ui/view/VstPluginViewImpl.cpp
  source/ui/view/win32/Win32SfmlWindow.cpp

  # LOGGING
  source/log/PolyLogger.cpp
)

target_include_directories( ${PROJECT_NAME}
  PRIVATE
    ${DEPENDS_DIR}/spdlog/include

    ${IMGUI_DIR}
    ${DEPENDS_DIR}/imgui-sfml

    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries( ${PROJECT_NAME}
  PRIVATE
    sdk

    msimg32.lib
    Rpcrt4.lib

    sfml-graphics
    sfml-window
    sfml-main
)

if(SMTG_MAC)
  set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12)
  smtg_set_bundle(SfmlDots
      BUNDLE_IDENTIFIER org.sfmlvst.sfmldots
      INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS
      )
  smtg_set_debug_executable(SfmlDots
      "/Applications/VST3PluginTestHost.app"
      "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
      )
elseif(SMTG_WIN)
  target_sources(SfmlDots PRIVATE
      resource/win32resource.rc
      )
  if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SfmlDots)

    smtg_set_debug_executable(SfmlDots
        "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
        "--pluginfolder \"$(OutDir)/\""
        )
  endif()
endif()

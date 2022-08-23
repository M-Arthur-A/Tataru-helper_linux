#pragma once

#include <yaml-cpp/node/detail/iterator_fwd.h>
#include <yaml-cpp/yaml.h>
#include <string>

class Config {
public:
  Config (const char* configPath) {
    YAML::Node config = YAML::LoadFile(configPath)["config"];
    for (YAML::const_iterator it = config.begin(); it != config.end(); ++it) {
      std::string key   = it->first.as<std::string>();
      auto value = it->second;
      if (key == "Screenshot") {
        x = value["position_coords"]["position_x"].as<uint>();
        y = value["position_coords"]["position_y"].as<uint>();
        width  = value["screenshot_size"]["width"].as<uint>();
        height = value["screenshot_size"]["height"].as<uint>();
      } else if (key == "ColorizeThresholds") {
        colorThresholdBot = value["bot"].as<uint>();
        colorThresholdTop = value["top"].as<uint>();
      } else if (key == "CuttingRectangles") {
        xTextAreaPosBot  = value["rectangle_coords"]["bot_x"].as<uint>();
        yTextAreaPosBot  = value["rectangle_coords"]["bot_y"].as<uint>();
        xTextAreaPosTop  = value["rectangle_coords"]["top_x"].as<uint>();
        yTextAreaPosTop  = value["rectangle_coords"]["top_y"].as<uint>();
        xTextAreaSizeBot = value["rectangle_size"]["bot_x"].as<uint>();
        yTextAreaSizeBot = value["rectangle_size"]["bot_y"].as<uint>();
        xTextAreaSizeTop = value["rectangle_size"]["top_x"].as<uint>();
        yTextAreaSizeTop = value["rectangle_size"]["top_y"].as<uint>();
      } else if (key == "DialogFrameDetection") {
        pixelXcoords = value["pixel_coords_x"].as<uint>();
        pixelYcoords = value["pixel_coords_y"].as<uint>();
      } else if (key == "CuttingCorrection") {
        videoDialogShift = value["shift_y"].as<uint>();
        needDemo = value["debug"].as<bool>();
      } else {
        translateLangFrom = value["from"].as<std::string>();
        translateLangTo   = value["to"].as<std::string>();
        translator        = value["translator"].as<std::string>();
      }
    }
  }

  uint x, y, width, height;
  uint colorThresholdBot, colorThresholdTop;
  uint xTextAreaPosBot, yTextAreaPosBot, xTextAreaPosTop, yTextAreaPosTop;
  uint xTextAreaSizeBot, yTextAreaSizeBot, xTextAreaSizeTop, yTextAreaSizeTop;
  uint pixelXcoords, pixelYcoords;
  uint videoDialogShift;
  bool needDemo;
  std::string translateLangFrom, translateLangTo, translator;
};

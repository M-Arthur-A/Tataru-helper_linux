#include "get-screen.h"
#include "get-ocr.h"
#include "get-translate.h"
#include "get-config.cpp"

#include <chrono>
#include <iostream>
#include <string>
#include <fstream>
#include <thread>


int main(int argc, char** argv) {
  const char* configPath;
  if(argv[1]) {
    configPath = argv[1];
  } else {
    configPath = "/home/arthur/Project/mySoft/Cpp/tataru-ocr/CONFIG.yaml";
  }
  if ( !std::ifstream(configPath).good() ) {
    std::cout << BOLDRED << "Please, send an argument with your yaml-config path!\n" << RESET << std::endl;
    exit(1);
  }

  std::string ENGtext(" "), previousENGtext(" "), RUtext(" ");
  cv::Mat img;
  Config config(configPath);

  for(;;) {
    screenshot(img, config);

    // check if img is not dialog: screenshot return img with 1 channel (grayscale)
    if (img.channels() != 1) {
      std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(1));
      continue;
    }

    OCRfromImg(ENGtext, img);
    if (config.needDemo) {
      std::cout << ENGtext << std::endl;
    }

    translate(ENGtext, previousENGtext, config);

    // get little delay
    std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(1));
  };
  return 0;
}

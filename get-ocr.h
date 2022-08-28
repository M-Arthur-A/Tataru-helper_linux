#pragma once
#include "get-screen.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <string>

void OCRfromImg(std::string &outText, const cv::Mat &matImg);

#pragma once
#include "get-screen.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <string>


// Pix* mat8ToPix(cv::Mat *mat8);

void OCRfromImg(std::string &outText, cv::Mat &matImg);

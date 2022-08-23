#include "get-ocr.h"
#include "get-screen.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <string>

// https://tesseract-ocr.github.io/tessdoc/Examples_C++.html

void OCRfromImg(std::string& outText, cv::Mat &matImg) {
  if (matImg.empty()) {
    printf("there is no cv::Mat image!!!!");
    exit(1);
  }

  tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
  // Initialize tesseract-ocr with English, without specifying tessdata path
  if (api->Init(NULL, "eng")) {
    fprintf(stderr, "Could not initialize tesseract.\n");
    exit(1);
  }

  api->SetImage(matImg.data, matImg.cols, matImg.rows, 4, matImg.step);
  // Get OCR result
  outText = std::string(api->GetUTF8Text());

  // Destroy used object and release memory
  api->End();
  delete api;
}

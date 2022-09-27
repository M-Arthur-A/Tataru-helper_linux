#pragma once

#include "get-config.cpp"
#include <opencv4/opencv2/opencv.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <iostream>

#define FPS(start) (CLOCKS_PER_SEC / (clock()-start))

struct ScreenShot {
  Display* display;
  Window root;
  XWindowAttributes window_attributes;
  Screen* screen;
  XImage* ximg;
  uint videoDialogShift;

  ScreenShot(const Config &config);

  void operator() (cv::Mat& cv_img);

  void cleaner(cv::Mat& cv_img);

  ~ScreenShot();
};

void findPxlToCheck(cv::Mat &img, const Config &config);

void screenshot(cv::Mat &img, const Config &config);

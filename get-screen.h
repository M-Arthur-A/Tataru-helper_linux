#pragma once

#include "get-config.cpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <opencv4/opencv2/opencv.hpp>
#include <time.h>
#include <iostream>

struct ScreenShot {
  Display* display;
  Window root;
  XWindowAttributes window_attributes;
  Screen* screen;
  XImage* ximg;
  uint videoDialogShift;

  ScreenShot(Config &config);

  void operator() (cv::Mat& cv_img);

  void cleaner(cv::Mat& cv_img);

  ~ScreenShot();
};

void screenshot(cv::Mat &img, Config &config);

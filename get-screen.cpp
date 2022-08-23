#include "get-config.cpp"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <cstddef>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <opencv4/opencv2/opencv.hpp>
#include <time.h>
/*
 * https://stackoverflow.com/a/39781697
 */

#define FPS(start) (CLOCKS_PER_SEC / (clock()-start))


struct ScreenShot{
  ScreenShot(Config &config): config(config) {
    display = XOpenDisplay(nullptr);
    root = DefaultRootWindow(display);
    XGetWindowAttributes(display, root, &window_attributes);
    screen = window_attributes.screen;
    ximg = XGetImage(display, root, config.x, config.y + videoDialogShift, config.width, config.height, AllPlanes, ZPixmap);
  }

    void operator() (cv::Mat& cv_img){
    int BitsPerPixel = ximg->bits_per_pixel;
    std::vector<uint8_t> pixels;
    pixels.resize(config.width * config.height * 4);
    memcpy(&pixels[0], ximg->data, pixels.size());
    cv_img = cv::Mat(config.height, config.width, CV_8UC4, ximg->data);
  }

  void cleaner(cv::Mat& cv_img) {
    // Convert RGBA data to grayscale data
    cv::cvtColor(cv_img, cv_img, cv::COLOR_RGBA2GRAY);

    // masking
    cv::Mat botMask, upMask;
    cv::Mat cropBotMask, cropUpMask;
    cv::Mat croppedBot, croppedUp;
    cv::threshold(cv_img, botMask, config.colorThresholdBot, 255, cv::THRESH_BINARY_INV);
    cv::threshold(cv_img, upMask,  config.colorThresholdTop, 255, cv::THRESH_BINARY);

    cropBotMask = cv::Mat::zeros(cv_img.rows, cv_img.cols, CV_8U);
    cropBotMask(cv::Rect(config.xTextAreaPosBot,
                         config.yTextAreaPosBot + videoDialogShift,
                         config.xTextAreaSizeBot,
                         config.yTextAreaPosBot)) = 1;
    botMask.copyTo(croppedBot, cropBotMask);

    cropUpMask = cv::Mat::zeros(cv_img.rows, cv_img.cols, CV_8U);
    cropUpMask(cv::Rect(config.xTextAreaPosTop,
                        config.yTextAreaPosTop + videoDialogShift,
                        config.xTextAreaSizeTop,
                        config.yTextAreaSizeTop)) = 1;
    upMask.copyTo(croppedUp, cropUpMask);

    // Blend together
    // Convert Mat to float data type
    croppedUp.convertTo(croppedUp, CV_32FC3);
    croppedBot.convertTo(croppedBot, CV_32FC3);

    // Normalize the alpha mask to keep intensity between 0 and 1
    cropUpMask.convertTo(cropUpMask, CV_32FC3, 1.0/255); //

    // Storage for output image
    cv_img = cv::Mat::zeros(croppedUp.size(), croppedUp.type());

    // Multiply the foreground with the alpha matte
    cv::multiply(cropUpMask, croppedUp, croppedUp);

    // Multiply the background with ( 1 - alpha )
    cv::multiply(cv::Scalar::all(1.0)-cropUpMask, croppedBot, croppedBot);

    // Add the masked foreground and background.
    cv::add(croppedUp, croppedBot, cv_img);
  }

  ~ScreenShot(){
    XDestroyImage(ximg);
    XCloseDisplay(display);
  }

  Config &config;
  Display* display;
  Window root;
  XWindowAttributes window_attributes;
  Screen* screen;
  XImage* ximg;
  uint videoDialogShift = 0;
};


void screenshot(cv::Mat &img, Config &config) {
  ScreenShot screen(config);

  for(uint i;; ++i) {
    double start = clock();

    screen(img);

    /* find pixel with certain color in dialog frame
    cv::Vec3b color = img.at<cv::Vec3b>(cv::Point(20,150));
    std::cout << color << std::endl;
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
    img.at<cv::Vec3b>(cv::Point(20,150)) = color;
    imshow("Result", img);
    cv::waitKey();
    */

    if(!(i & 0b111111)) {
      if(config.needDemo) {
        printf("fps %4.f  spf %.4f\n", FPS(start), 1 / FPS(start));
      }
      break;
    }
  }

  // detect dialog / interactive dialog (without GUI)
  cv::Vec3b color = img.at<cv::Vec3b>(cv::Point(config.pixelXcoords, config.pixelYcoords));
  if (!((color[0] == 201) && (color[1] == 217) && (color[2] == 226)) &&
      !((color[0] == 202) && (color[1] == 219) && (color[2] == 228)) &&
      !((color[0] == 199) && (color[1] == 216) && (color[2] == 231)) &&
      !((color[0] == 105) && (color[1] == 113) && (color[2] == 115))) {
    // std::cout << color << std::endl;
    // img.setTo(cv::Scalar::all(0)); // make all black
    return;
  }
  if (((color[0] == 202) && (color[1] == 219) && (color[2] == 228)) |
      ((color[0] == 199) && (color[1] == 216) && (color[2] == 231)) |
      ((color[0] == 105) && (color[1] == 113) && (color[2] == 115))) {
    screen.videoDialogShift = config.videoDialogShift;
  }

  screen.cleaner(img);

  if(config.needDemo) {
    // demontrate capture window
    cv::imshow("img", img);
    cv::waitKey(0);
  }
}

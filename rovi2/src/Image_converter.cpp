#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <cv_bridge/cv_bridge.h>
using namespace cv;
static const std::string OPENCV_WINDOW = "Image window";
static const std::string OPENCV_TRACKBAR = "HSV Trackbar";
static const std::string OPENCV_THRESHOLD = "Thresholded image";
static int iLowH = 20;
static int iHighH = 0;
static int iLowS = 0;
static int iHighS = 30;
static int iLowV = 255;
static int iHighV = 255;


void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
  cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);

  
  
  Mat cv_HSV;
  cv::cvtColor(cv_bridge::toCvShare(msg, "bgr8")->image,cv_HSV,COLOR_RGB2HSV);
  cv::inRange(cv_HSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), cv_HSV); //Threshold the image
  cv::imshow(OPENCV_THRESHOLD,cv_HSV);
  cv::waitKey(3);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
  cv::namedWindow("view",WINDOW_NORMAL);
 
  cv::namedWindow(OPENCV_THRESHOLD,WINDOW_NORMAL);
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("stereo_camera/left/image_raw", 1, imageCallback);
  ros::spin();
  cv::destroyWindow("view");
  
  cv::destroyWindow(OPENCV_THRESHOLD);
}

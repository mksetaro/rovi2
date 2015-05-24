#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

static const std::string OPENCV_WINDOW_RIGHT = "Right camera";
static const std::string OPENCV_WINDOW_LEFT = "Left camera";
class ImageConverter
{
  ros::NodeHandle nh_right;
  image_transport::ImageTransport it_right;
  image_transport::Subscriber image_sub_right;
  image_transport::Publisher image_pub_right;
  
  //ros::NodeHandle nh_left;
  //image_transport::ImageTransport it_left;
  //image_transport::Subscriber image_sub_left;
  //image_transport::Publisher image_pub_left;
  
public:
  //right camera
  ImageConverter(): it_right(nh_right)
  {
    // change here the topic to subscribe
    image_sub_right = it_right.subscribe("/stereo_camera/right/image_raw", 1, &ImageConverter::imageManipulation, this);
    image_pub_right = it_right.advertise("/image_converter/right/output_video", 1);
    
    cv::namedWindow(OPENCV_WINDOW_RIGHT);
    
  }

 //left camera
  //ImageConverter(): it_left(nh_left)
  //{
    // change here the topic to subscribe
 
    //image_sub_left = it_left.subscribe("/stereo_camera/left/image_raw", 1, &ImageConverter::imageManipulation, this);
    //image_pub_left = it_left.advertise("/image_converter/left/output_video", 1);

   
    //cv::namedWindow(OPENCV_WINDOW_LEFT);
  //}

  ~ImageConverter()
  {
     cv::destroyWindow(OPENCV_WINDOW_RIGHT);
    //cv::destroyWindow(OPENCV_WINDOW_LEFT);
  }

  void imageManipulation(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr_right;
    //cv_bridge::CvImagePtr cv_ptr_left;
    try
    {
      cv_ptr_right = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
     //cv_ptr_left = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

//implement here your own code,to access to the image you have to use the cv_bridge::obj->image

    // Update GUI Window
    cv::imshow(OPENCV_WINDOW_RIGHT, cv_ptr_right->image);
    //cv::imshow(OPENCV_WINDOW_LEFT, cv_ptr_left->image);
    cv::waitKey(0);
    
    // Output modified video stream
    image_pub_right.publish(cv_ptr_right->toImageMsg());
    //image_pub_left.publish(cv_ptr_left->toImageMsg());
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}

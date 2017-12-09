//contours.cpp
//矩形切りだし
//bokete用

#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(){
  int filenum=0;

  char str[]="/Users/matsugijitsuhiro/python/images2";//入力
  char str1[]=".jpg";


  char str2[]="/Users/matsugijitsuhiro/python/image2-change";//出力

  while(3078>filenum){

  char filename[256]="";//入力
  char filename1[256]="";//出力
  sprintf(filename,"%s/%d%s",str,filenum,str1);//入力動画用
  sprintf(filename1,"%s/%d%s",str2,filenum,str1);//入力動画用
  //入力
  cv::Mat mat=cv::imread(filename);//入力する用
  //出力用
  cv::Mat mat2=mat;

  //二値化
  cv::Mat gray; // グレースケール出力用
  cv::cvtColor(mat,gray,CV_BGR2GRAY);

  cv::threshold(gray, gray, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

  //cv::threshold(gray, gray, 200, 255, CV_THRESH_TOZERO_INV );
  //cv::bitwise_not(gray, gray); // 白黒の反転
  //cv::threshold(gray, gray, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

  //cv::imshow("test",gray);
  //輪郭抽出
  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;
  cv::findContours(gray, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_TC89_L1);

  std::vector<std::vector<cv::Point> > tmpContours=contours;

  double maxsize=0;;
  int max_level = 0;

  cv::Mat output;

for(int i = 0; i < contours.size(); i++) {
  if(maxsize<contourArea(contours[i],false))
    maxsize = contourArea(contours[i],false);
  }
  //std::cout<<maxsize<<std::endl;

  for(int i = 0; i < contours.size(); i++) {
    // ある程度の面積が有るものだけに絞る
    double a = contourArea(contours[i],false);
    if((a > (mat.cols*mat.rows*0.3))&&(maxsize>a)){
        std::cout<<i<<std::endl;
        //輪郭を直線近似する
        std::vector<cv::Point> approx;
        cv::approxPolyDP(cv::Mat(contours[i]), approx, 0.01 * cv::arcLength(contours[i], true), true);
        // 矩形のみ取得
        if (approx.size() == 4) {
          cv::Rect brect = cv::boundingRect(cv::Mat(approx).reshape(2));
          output = cv::Mat(mat2,brect);
          //cv::drawContours(InputOutputArray image, InputArrayOfArrays contours, int contourIdx, const Scalar& color, int thickness=1, int lineType=8, InputArray hierarchy=noArray(), int maxLevel=INT_MAX, Point offset=Point())
          //cv::drawContours(mat2, tmpContours, i, cv::Scalar(255, 0, 0), 3, CV_AA, hierarchy, max_level);

        }
    }
  }

cv::imwrite(filename1,output);
//cv::imshow("test",output);
//cv::waitKey(0);
filenum++;
}
  return 0;
}

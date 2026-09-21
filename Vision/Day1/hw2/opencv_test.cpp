#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat image3;

// 마우스 클릭
void onMouse(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN) { 
        Vec3b val = image3.at<Vec3b>(y, x);
        cout << " | H: " << (int)val[0] << ", S: " << (int)val[1] << ", V: " << (int)val[2] << std::endl;
    }
}

int main(int ac, char** av) {

	Mat image = imread("../hw2.png");

    if(image.empty()){
        cout <<"이미지 확인";
        return -1;
    }

    // 이미지 사이즈
    Mat image2;
    resize(image, image2, Size(420,236));
    // Mat image3;
    // BGR -> HSV
    cvtColor(image2, image3, COLOR_BGR2HSV);
    // 5*5 사이즈 타원형 커널 생성
    Mat Kernel = getStructuringElement(MORPH_ELLIPSE,Size(5,5));

    
    Mat mask;
    inRange(image3,Scalar(0,100,70), Scalar(10,255,255), mask);
    //imshow("mask", mask);

    Mat mask2;  
    inRange(image3, Scalar(170,100,70), Scalar(179,255,255), mask2);
    //imshow("mask2", mask2);

    Mat mask1_mask2;
    bitwise_or(mask, mask2, mask1_mask2);
    //imshow("mask1_mask2",mask1_mask2);

    Mat opening;
    morphologyEx(mask1_mask2, opening, MORPH_OPEN, Kernel);
    //imshow("red_opening", opening);

    Mat closing;
    morphologyEx(mask1_mask2, closing, MORPH_CLOSE, Kernel);
    //imshow("red_closing", closing);

    //Mat red_mask;
    //bitwise_or(opening, closing, red_mask);
    //imshow("RED_MASK", red_mask);
    // 검은 배경에 빨간색 띄우기

    Mat red;
    bitwise_and(image2,image2, red, closing);
    //imshow("RED", red);
    // 바운딩 박스 출력
    vector<vector<Point>> contours;
    findContours(closing, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    Rect box = boundingRect(contours[0]);
    rectangle(red, box, Scalar(0, 0, 255), 2);
    //imshow("RED_RESULT", red);

    // 파란색 마스크 만들기
    Mat mask3, mask3_opening, mask3_closing, blue_mask, blue;
    inRange(image3,Scalar(100,100,70), Scalar(110,255,255), mask3);
    //imshow("mask3", mask3);

    morphologyEx(mask3, mask3_opening, MORPH_OPEN, Kernel);
    //imshow("blue_opening", mask3_opening);

    morphologyEx(mask3, mask3_closing, MORPH_CLOSE, Kernel);
    //imshow("blue_closing", mask3_closing);

    bitwise_or(mask3_closing, mask3_opening, blue_mask);
    // 파란색 마스크 출력
    //imshow("BLUE_MASK", blue_mask);

    // 검은 배경에 원본 파란색 띄우기
    bitwise_and(image2,image2, blue, blue_mask);
    //imshow("BLUE", blue);

    // 파란색 바운딩 박스 출력
    vector<vector<Point>> blue_contours;
    findContours(blue_mask, blue_contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    Rect blue_box = boundingRect(blue_contours[0]);
    rectangle(blue, blue_box, Scalar(255, 0, 0), 2);
    //imshow("BLUE_RESULT", blue);




    //초록색 마스크
    Mat mask4, mask4_opening, mask4_closing, green_mask, green;
    inRange(image3,Scalar(50,100,30), Scalar(70,255,255), mask4);
    imshow("mask4", mask4);

    morphologyEx(mask4, mask4_opening, MORPH_OPEN, Kernel);
    imshow("mask4_opening", mask4_opening);

    morphologyEx(mask4, mask4_closing, MORPH_CLOSE, Kernel);
    imshow("mask4_closing", mask4_closing);

    bitwise_or(mask4_closing, mask4_opening, green_mask);
    //초록색 마스크 출력
    imshow("GREEN_MASK", green_mask);

    // 검은 배경에 원본 초록색 띄우기
    bitwise_and(image2, image2, green, green_mask);
    imshow("GREEN", green);

    // 초록색 바운딩 박스 출력
    vector<vector<Point>> green_contours;
    findContours(green_mask, green_contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    Rect green_box = boundingRect(green_contours[0]);
    rectangle(green, green_box, Scalar(0, 255, 0), 2);
    imshow("GREEN_RESULT", green);





    // 이미지 합치기
    Mat red_blue, red_blue_green;
    bitwise_or(red, blue, red_blue);
    imshow("red_blue", red_blue);
    bitwise_or(red_blue, green, red_blue_green);
    imshow("red_blue_green", red_blue_green);

    //원본 이미지와 합친 이미지
    Mat final;
    bitwise_or(red_blue_green, image2, final);
    imshow("final", final);

    // 원본 이미지
	imshow("BGR", image2);
    setMouseCallback("BGR", onMouse);

    waitKey(0);

	return 0;
}
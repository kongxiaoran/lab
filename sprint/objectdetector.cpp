#include "objectdetector.h"



#include <iostream>

#include <vector>

#include <algorithm>

#include <cassert>



cv::VideoCapture ObjectDetector::capture(0);

const double ObjectDetector::cap_width = capture.get(CV_CAP_PROP_FRAME_WIDTH) ;

const double ObjectDetector::cap_height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);



ObjectDetector::ObjectDetector(const std::string & obj_name, int imshow_flag, bool mixed_hue)

	:obj_name(obj_name), show_flag(imshow_flag), mixed_flag(mixed_hue),

	lower_color(0, 0, 0, 0), upper_color(180, 180, 255, 255),

	rect(cap_width / 2, cap_height / 2, 0, 0)
{

	assert(capture.isOpened());

	load();

}



ObjectDetector::~ObjectDetector()

{

	save();

}



void ObjectDetector::load()

{

	cv::FileStorage fs(obj_name, cv::FileStorage::READ);

	if (!fs.isOpened()) {

		std::clog << "Cannot read file: " << obj_name << std::endl;

		return;

	}

	cv::FileNode fn_lower = fs["lower_color"],

		fn_upper = fs["upper_color"];

	cv::FileNodeIterator it_lower = fn_lower.begin(),

		it_upper = fn_upper.begin();

	for (int i = 0; i != 4; ++i, ++it_lower, ++it_upper) {

		if (fn_lower.size() == 3 && i == 1) {

			--it_lower;

			--it_upper;

			continue;

		}

		lower_color[i] = static_cast<int>(*it_lower);

		upper_color[i] = static_cast<int>(*it_upper);

	}

	fs.release();

}



void ObjectDetector::save()

{

	cv::FileStorage fs(obj_name, cv::FileStorage::WRITE);

	fs << "lower_color" << "[" << lower_color[0] << lower_color[1] << lower_color[2] << lower_color[3] << "]";

	fs << "upper_color" << "[" << upper_color[0] << upper_color[1] << upper_color[2] << upper_color[3] << "]";

	fs.release();

}



void ObjectDetector::refresh_capture(int times)

{

	cv::Mat temp_image;

	for (int i = 0; i != times; ++i) {

		capture >> temp_image;

	}

}



inline bool rect_area_cmp(cv::Rect& lhs, const cv::Rect& rhs)

{

	return lhs.area() < rhs.area();

}

std::string ObjectDetector::getTime(){

        time_t timep;
        time(&timep);
        char tmp[64];
        strftime(tmp,sizeof(tmp),"%H%M%S",localtime(&timep));
        return tmp;

}

int ObjectDetector::process_by_color(int wait_msec, int rect_filter)

{

	using namespace cv;

	Mat source_image1;
	capture >> source_image1;
	if (source_image1.empty()) {
		std::cerr << "The source image is empty" << std::endl;
		return -1;
	}
        std::string time=getTime();
            String name="./jpg/"+time+".jpg";
        imwrite(name,source_image1);
	Mat source_image;
	resize(source_image1, source_image, Size(source_image1.cols * 0.5, source_image1.rows * 0.5));
 

	GaussianBlur(source_image, source_image, Size(5, 5), 0, 0);


	Mat hsv_image;

	cvtColor(source_image, hsv_image, CV_BGR2HSV);	//ת��ΪHSVɫ�ʿռ�

	Mat binary_image;
        //imshow("hsv",hsv_image);

	inRange(hsv_image, get_lower_color(), get_upper_color(), binary_image);	//����Χ��ֵ��



	if (mixed_flag) {

		Mat binary_image2;

		inRange(hsv_image, get_lower_color(true), get_upper_color(true), binary_image2);

		addWeighted(binary_image2, 1, binary_image, 1, 0, binary_image);

	}



	morphologyEx(binary_image, binary_image, CV_MOP_OPEN,

		getStructuringElement(MORPH_RECT, Size(10, 10)));	//����������
        //imshow("open",binary_image);

	morphologyEx(binary_image, binary_image, CV_MOP_OPEN,

		getStructuringElement(MORPH_RECT, Size(10, 10)));	//�ղ�������


        //imshow("close",binary_image);

	using std::vector;



	vector<vector<Point> > all_contours;

	vector<Vec4i> hierarchy;

	Mat contours_image = binary_image.clone();

	findContours(contours_image, all_contours, hierarchy,

		RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);		//��������

	vector<Rect> all_bound;

	for (vector<vector<Point> >::iterator i = all_contours.begin();

		i != all_contours.end(); ++i) {

		Rect temp = boundingRect(Mat(*i));	//��������ȡ����

		if (rect_filter & Vertical) {		//���˵����������ŵ�

			if (temp.width > temp.height)

				continue;

		}

		all_bound.push_back(temp);

	}

	rect = Rect(cap_width / 2, cap_height / 2, 0, 0);

	vector<cv::Rect>::iterator biggest_rect_iter

		= std::max_element(all_bound.begin(), all_bound.end(), rect_area_cmp);	//��ȡ������ľ��εĵ�����

	if (biggest_rect_iter != all_bound.end()) {		//��Ϊ.end()��û�����ģ���ͼ����û������

		rect = *biggest_rect_iter;

	}



	//��ʾ�Ĵ���

	if (show_flag != NotShow) {

		Mat show_image = Mat::zeros(source_image.size(), source_image.type());

		if (show_flag & ShowSource) {	//��ʾԭͼƬ

			show_image = source_image.clone();

		}

		if (show_flag & ShowBinary) {	//��ʾ��ֵ������

			Mat bgr_binary_image;

			cvtColor(binary_image, bgr_binary_image, CV_GRAY2BGR);	//ת��Ϊ��ͨ�����Ժϲ�

			const double alpha = 1, beta = 0.5, gamma = 0.0;

			addWeighted(show_image, alpha, bgr_binary_image, beta, gamma, show_image);	//���Ե���

		}

		if (show_flag & ShowDrawing) {	//���ߺ;���

			line(show_image, cv::Point(show_image.cols / 2, 0),

				Point(show_image.cols / 2, show_image.rows),

				Scalar(255, 0, 0), 2);		//��ͼ��������

			for (vector<Rect>::iterator i = all_bound.begin(); i != all_bound.end(); ++i) {	//�����о���

				rectangle(show_image, *i, cv::Scalar(0, 0, 0), 1);

			}

			line(show_image,

				Point(rect.x + rect.width / 2, rect.y),

				Point(rect.x + rect.width / 2, rect.y + rect.height),

				Scalar(0, 0, 255), 2);		//������������

			rectangle(show_image, rect, cv::Scalar(255, 255, 255), 2);	//��������

		}
		
		
		//cv::imshow(obj_name, show_image);

	}

	return waitKey(wait_msec);

}



void ObjectDetector::adjust_color()

{

	using cv::createTrackbar;



	const std::string adjust_window_name = obj_name + " - Adjust Color";

	//cv::namedWindow(adjust_window_name, cv::WINDOW_AUTOSIZE);

	createTrackbar("Low H", adjust_window_name, &lower_color[0], 180);

	createTrackbar("Up H", adjust_window_name, &upper_color[0], 180);

	if (mixed_flag) {

		createTrackbar("Low H 2", adjust_window_name, &lower_color[1], 180);

		createTrackbar("Up H 2", adjust_window_name, &upper_color[1], 180);

	}

	createTrackbar("Low S", adjust_window_name, &lower_color[2], 255);

	createTrackbar("Up S", adjust_window_name, &upper_color[2], 255);

	createTrackbar("Low V", adjust_window_name, &lower_color[3], 255);

	createTrackbar("Up V", adjust_window_name, &upper_color[3], 255);

}



cv::Scalar ObjectDetector::get_lower_color(bool second_hue)

{

	return cv::Scalar(lower_color[second_hue], lower_color[2], lower_color[3]);

}



cv::Scalar ObjectDetector::get_upper_color(bool second_hue)

{

	return cv::Scalar(upper_color[second_hue], upper_color[2], upper_color[3]);

}



int ObjectDetector::width() const

{

	return rect.width *5;

}



int ObjectDetector::height() const

{

	return rect.height*5;

}



int ObjectDetector::postion() const

{

	return x_axis() *5;

}



int ObjectDetector::x_axis() const

{

	return rect.x + (rect.width / 2);

}



int ObjectDetector::y_axis() const

{

	return rect.y + (rect.height / 2);

}



std::pair<int, int> ObjectDetector::central_point() const

{

	return std::pair<int, int>(x_axis(), y_axis());

}



bool ObjectDetector::empty() const

{

	return rect.area() == 0;

}



int & ObjectDetector::imshow_flag()

{

	return show_flag;

}



const std::string ObjectDetector::object_name() const

{

	return obj_name;

}

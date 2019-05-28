#pragma once



#include <opencv/cv.h>

#include <opencv2/highgui/highgui.hpp>

#include <opencv2/video/tracking.hpp>

#include <opencv2/core/core.hpp>



#include <string>

#include <utility>



//��ʾ���ڱ�ʶ

enum ShowImage {

	NotShow = 0,

	ShowSource = 1,

	ShowBinary = 2,

	ShowDrawing = 4,

	ShowAll = ShowSource | ShowBinary | ShowDrawing

};



//���ι�����ѡ��

enum RectFilter {

	NotFilter = 0,

	Vertical = 1,

};



class ObjectDetector

{

public:

	//���캯��������Ϊ���ơ���ʾ���ڱ�ʶ

	ObjectDetector(const std::string & obj_name, int imshow_flag = NotShow, bool blend_color = false);

	//�����������ᱣ����ɫ��ֵΪ�ļ�

	~ObjectDetector();



	//ˢ����Ƶ���棬����Ϊˢ�´���

	void refresh_capture(int times = 2);



	//��Ƶ��ȡ��߶�

	static const double cap_width;

	static const double cap_height;



	//������ɫ�������������Զ�����

	void adjust_color();

	//����ɫ����ͼ�񣬲���Ϊ������ʱ�䣨���룩

	int process_by_color(int wait_msec, int rect_filter = NotFilter);



	//��������Ŀ�ȡ��߶ȡ�λ�á��Ƿ����

	int width() const;

	int height() const;

	int postion() const;

	std::pair<int, int> central_point() const;

	bool empty() const;



	//��ȡ��ʾ���ڱ�ʶ��ͬʱҲ����ֱ�����ã�obj.imshow_flag() = ShowBinary)

	int & imshow_flag();

	//����������

	const std::string object_name() const;



private:

	void load();

	void save();



	int x_axis() const;

	int y_axis() const;



	cv::Scalar get_lower_color(bool second_hue = false);

	cv::Scalar get_upper_color(bool second_hue = false);



	static cv::VideoCapture capture;



	const std::string obj_name;

	int show_flag;

	bool mixed_flag;
    std::string getTime();


	cv::Vec4i lower_color, upper_color;

	cv::Rect rect;



};

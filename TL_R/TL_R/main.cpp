#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <ctime>
#include <core/core.hpp>  
#include <highgui/highgui.hpp>
#include "TLR.h"


using namespace std;
using namespace cv;

clock_t t0, t1,t;
int main(void)
{
	//ifstream fin("F\\OPENCV\\3\\pics.txt");
	//Mat src;
	//string s;

	//while (fin >> s)
	//{
	//	src = imread(s, -1); //����ԭͼ ����alphaͨ��
	//	if (src.empty())
	//	{
	//		cout << "No such images" << endl;
	//		return 0;
	//	}

	//	//����ʶ�𲿷�

	//	imshow("dectect",image);
	//	waitKey(1);

	//}


	//////////////////////////////����Ϊ��������:finHSV
	//TLR result;
	//Mat src = imread("F:\\OPENCV\\3\\LightRaw000378.jpg", -1);
	//Mat mask(580, 780, CV_8UC1);
	//Mat state(580, 780, CV_8UC1);
	//Mat HSV(580, 780, CV_8UC1);


	////result.findHSV(src, mask, state);
	//cvtColor(src, HSV, CV_BGR2HSV);
	//
	////imshow("mask", mask);
	////imshow("state", state);
	//imshow("hsv", HSV);
	//waitKey(0);

	//////////////////////////////����Ϊ��������:Morph
	//TLR result;
	//Mat src = imread("F:\\΢��ͼƬ_20180412233530.jpg", -1);
	//Mat morph,morph2,morph3,dst;
	//resize(src, dst, Size(src.cols / 4, src.rows / 4), 0, 0, INTER_LINEAR);
	//Mat open_element = getStructuringElement(MORPH_ELLIPSE, Size(6, 7));
	//Mat close_element = getStructuringElement(MORPH_RECT, Size(10, 10));
	//morphologyEx(dst, morph, MORPH_ERODE, open_element);
	////morphologyEx(morph, morph2, MORPH_CLOSE, close_element);
	////int b = 5;
	////int& a = b;
	////result.Morph(dst, morph3, a, a, a, a, 1);
	//imshow("src", dst);
	//imshow("morph2", morph);
	////imshow("morph3", morph3);
	//waitKey(0);


	///////////////////////////����Ϊbulb������
	/*TLR result;


	Mat src = imread("F:\\OPENCV\\3\\LightRaw000375.jpg", -1);
	Mat dst = src.clone();
	Mat mask(src.size(), CV_8UC1, Scalar(0));
	Mat state(src.size(), CV_8UC1);

	result.findHSV(src, mask, state);
	imshow("mask", mask);

	vector<Rect> bulb;
	result.findBulb(mask, bulb);
	for (size_t i = 0; i != bulb.size(); ++i){
		rectangle(dst, bulb[i].tl(), bulb[i].br(), Scalar(0, 0, 255), 2, 8, 0);
	}

	imshow("dst", dst);
	waitKey(0);*/

	///����Ϊmain��������
	//TLR result;
	//Mat src = imread("F:\\OPENCV\\3\\LightRaw000375.jpg", -1);
	//Mat mask(580, 780, CV_8UC1,Scalar(0));
	//Mat clone = src.clone();
	//Mat dst1(580, 780, CV_8UC1, Scalar(0));
	//Mat dst2(580, 780, CV_8UC1, Scalar(0));
	//Mat state(580, 780, CV_8UC1);
	//result.findHSV(src, mask, state);
	//int opwidth = 5;
	//int opheight = 5;
	//int clwidth = 9;
	//int clheight = 9;
	////result.Morph(mask, dst1, opwidth, opheight, clwidth, clheight, 0);
	//result.Morph(mask, dst2, opwidth, opheight, clwidth, clheight, 1);
	////imshow("�յ���", dst1);
	////imshow("������", dst2);
	//vector<Rect> bulb;
	//result.findBulb(dst2, bulb);
	//for (size_t i = 0; i != bulb.size(); ++i){
	//	rectangle(clone, bulb[i].tl(), bulb[i].br(), Scalar(0, 0, 255), 2, 8, 0);
	//}
	//imshow("������", clone);
	//waitKey(0);
	////result.findHSV(src, mask, state);
	//return 0;



    //����ͼƬʵ�鲿��
    TLR result;
	////��ʼ��KNN����
	result.kNclassifer();
	////��û���ѧϰģ��
	result.getData();
	result.train();
	//result.test();
    //Mat src = imread("F:\\OPENCV\\3\\LightRaw000375.jpg", -1);
    //Mat src = imread("I:\\TL\\20170121_1143310876.bmp", -1);
	Mat src = imread("I:\\TL\\20170112_1037460885.bmp", -1);
	Mat dst(src);
	t0 = clock();
	result.Recognition(src, dst);
	t1 = clock();
	cout << t1 - t0 << "ms" << endl;
	//result.RecognitionLight(src, dst);
	imshow("dst", dst);
	waitKey(0);

	//return 0;



 //   //һȺ����ʵ�鲿��
	//ifstream input("F\\OPENCV\\3\\picss.txt");
	//Mat src;
	//string name;
	//TLR result;
	////��ʼ��KNN����
	//result.kNclassifer();
	////��û���ѧϰģ��
	//result.getData();
	//result.train();
	//
	//for (int i = 1; i <= 200;i++)
	//{
	//	stringstream ss;
	//	string str;
	//	ss << i;
	//	ss >> str;
	//	//name = "F:\\OPENCV\\�ڶ���ʵ��\\" + str + ".bmp";
	//	name = "I:\\0518_xu\\image5\\" + str + ".jpg";
	//	src = imread(name, -1); //����ԭͼ ����alphaͨ��
	//	if (src.empty())
	//	{
	//		cout << "No such images" << endl;
	//		return 1;
	//	}
	//	t0 = clock();
	//	cout << "���ڴ���" << name << endl;
	//	Mat dst(src);
	//	result.Recognition(src,dst);
	//	result.RecognitionLight(src, dst);  //ȥ�����������û�п�Ƶ�С��
 //   	imshow("dectect",dst);
	//	t1 = clock();
	//	t = t + t1 - t0;
	//	cout << t1-t0 <<" ms"<< endl;
	//	waitKey(0);  //���waitkey��0�Ļ��������޵ȴ�
	//	//system("pause");
	//}
	////cout << "average time:" << t / 458 << endl;
	//waitKey(0);
	//return 0;
 ////   



    //////////////////dataset
  /*  TLR result;
	result.dataset();

	return 0;*/



	////findBOX
	//TLR result;
	//int opWidth = 5;
	//int opHeight = 5;
	//int clWidth = 9;
	//int clHeight = 9;
	////Mat src = imread("I:\\TL\\20170121_1143310876.bmp", -1); 
	//Mat src = imread("F:\\OPENCV\\3\\LightRaw000375.jpg", -1);
	//Mat dst = src.clone();
	//Mat box_map(src.size(), CV_8UC1, Scalar(0));
	//Mat morph_map(src.size(), CV_8UC1, Scalar(0));
	//vector<Rect> bulb;
	//vector<candidate> roi,cand;
	//result.findBox(src, box_map);
	//result.Morph(box_map, morph_map, opWidth, opHeight, clWidth, clHeight, 0);
	////imshow("hahaha", morph_map);
	//result.findBoxShape(dst,morph_map, bulb);
	////imshow("ayhaha", dst);
	////waitKey(0);
	//result.roiPic(dst, roi, bulb);
	//result.candJud(roi,cand);




//kNclassifer result;
//ifstream document("F:\\biaozhu\\�ɹ�ͼ\\6\\pics.txt");
//string s;
//TLR result;
//while (document.peek() != EOF)
//{
//	getline(document, s);
//	Mat src = imread(s, -1);
//	Mat mask(src.size(), CV_8UC1, Scalar(0));
//	Mat state(src.size(), CV_8UC1, Scalar(0));
//	result.findHSV(src, mask, state);
//	imwrite(s, mask);
//}
//return 0;
	


////һȺ����ʵ�鲿��
//ifstream input("F\\OPENCV\\3\\picss.txt");
//Mat src;
//string name;
//TLR result;
//kNclassifer ML;
//
//
//for (int i = 1; i <= 458; i++)
//{
//	stringstream ss;
//	string str;
//	ss << i;
//	ss >> str;
//	name = "F:\\OPENCV\\images\\" + str + ".jpg";
//	src = imread(name, -1); //����ԭͼ ����alphaͨ��
//	if (src.empty())
//	{
//		cout << "No such images" << endl;
//		return 1;
//	}
//	t0 = clock();
//	cout << "���ڴ���" << name << endl;
//	Mat dst(src);
//	result.Recognition(src, dst);
//	
//
//	//result.RecognitionLight(src, dst);
//	imshow("dectect", dst);
//	t1 = clock();
//	cout << t1 - t0 << " ms" << endl;
//	waitKey(1);  //���waitkey��0�Ļ��������޵ȴ�
//	//system("pause");
//}
//return 0;


}



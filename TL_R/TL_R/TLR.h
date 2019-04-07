#ifndef TLR_H
#define TLR_H

#include "stdafx.h"
#include <iostream>
#include <string>
#include <ctime>
#include "imgproc/imgproc.hpp"    
#include "highgui/highgui.hpp"    
#include "opencv2/opencv.hpp"    
#include <core/core.hpp>  
#include <opencv2/ml/ml.hpp>



//����ΪHSVԼ���ĳ���
#define H_R1 10
#define H_R2 156
#define V_TH 80   //�ݶ�����ֵ
#define S_TH 110
#define H_G1 50
#define H_G2 100


//BOXԼ����ɫ��ֵ
#define BOX_TH1 50
#define BOX_TH2 5

//#define H_R1 10
//#define H_R2 156
//#define V_TH 80   //�ݶ�����ֵ
//#define S_TH 110
//#define H_G1 30
//#define H_G2 100


//����Ϊ�߿��Լ���ĳ���(��)
#define MIN_ASPECT 2
#define MAX_ASPECT 4  //ֻ�еƵĻ���1.3
#define MAX_WIDTH 25
#define MIN_WIDTH 3 //ֻ�еƵ�ʱ�������1
#define HU_TH 0.6
#define LIGHT_ASPECT 1.3
//�߿�
#define LINE_THICK1 1
#define LINE_THICK2 2

//����Ȥ����ͼ���С��һ��
#define ROI_HEIGHT 45
#define ROI_WIDTH 20

//����Ϊ��̬ѧ��������˴�С����

//#define opWidth 5
//#define opHeight 5
//#define clWidth 9
//#define clHeight 9




using namespace std;
using namespace cv;

typedef struct CANDIDATE
{
	Rect candRect;
	Mat candPics;
	Mat candMask;
	int State;
	int idx;

	CANDIDATE& operator=(CANDIDATE tempbulb)
	{
		candMask = tempbulb.candMask;
		candRect = tempbulb.candRect;
		candPics = tempbulb.candPics;
		idx = tempbulb.idx;
		State = tempbulb.State;  //1Ϊ��ƣ�2Ϊ�̵�

		return *this;
	}
}candidate;


class TLR
{
public:
	
	TLR(void);
	~TLR(void);
	void RecognitionLight(Mat& src, Mat& dst);
	void dataset(void);
	void Recognition(Mat& src, Mat& dst);
	void resizeImg(void);

	void TRL(Mat& src, Mat& dst);
//private:
	void findHSV(Mat& RGB, Mat& mask, Mat& state_map);
	void Morph(Mat& src, Mat& dst, int opWidth, int opHeight, int clWidth, int clHeight, int model);
	void findBulb(Mat& mask, vector<Rect>& bulb);
	void showRect(Mat& state_map, Mat& src, vector<Rect>& bulb);
	void findState(Mat& state_map, Rect& bulb, int& State);
	void findBox(Mat& src, Mat& state, Mat& mask, Mat& state_map);
	void findBoxShape(Mat& src,Mat& mask, vector<Rect>& bulb);
	void roiPic(Mat& src, vector<candidate>& roiPics,vector<Rect>& bulb);
	void candJud(vector<candidate>& roiPics, vector<candidate>& candPics);
	void showBox(Mat& src, vector<candidate>& cand);
	void KNN(Mat& roi, int State);


	void kNclassifer();
	int train_samples;
	int classes;
	Mat trainData;
	Mat trainClasses;
	int size_height, size_width;
	KNearest* knn;
	static const int K = 6;
	void getData();
	void train();
	void test();
	float classify(Mat& image);
	float practice(Mat& image);

	clock_t t0, t1,t;

};//����Ҫ��һ���ֺŰ�����������



//class kNclassifer
//{
//public:
//	kNclassifer();
////private:
//	int train_samples;
//	int classes;
//	Mat trainData;
//	Mat trainClasses;
//	int size_height, size_width;
//	KNearest* knn;
//	static const int K = 6;
//	void getData();
//	void train();
//	void test();
//	float classify(Mat& image);
//	float practice(Mat& src);
//
//};

#endif
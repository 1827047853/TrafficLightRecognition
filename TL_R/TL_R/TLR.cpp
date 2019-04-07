#include "stdafx.h"
#include "TLR.h"
#include <iostream>
#include <fstream>
#include <string> 

TLR::TLR(void)
{
	
}

TLR::~TLR(void)  //�൱��ֱ�ӵ������������Ӧ����������ͷ��ڴ����˼��
{
}

void TLR::findHSV(Mat& RGB, Mat& mask, Mat& state_map)
{
	Mat HSV;
	unsigned char h, s, v;
	cvtColor(RGB, HSV, CV_BGR2HSV);  //rgbԭͼת��Ϊhsvͼ

	for (int i = 0; i < HSV.rows; i++){
		for (int j = 0; j < HSV.cols; j++)
		{
			h = HSV.at<Vec3b>(i, j)[0];
			s = HSV.at<Vec3b>(i, j)[1];
			v = HSV.at<Vec3b>(i, j)[2];

			if ((h<H_R1 || h>H_R2) && s>S_TH && v>V_TH)
			{
				mask.at<uchar>(i, j) = 255;
				state_map.at<uchar>(i, j) = 255;
			}

			if (h > H_G1 && h<H_G2 && s>S_TH && v > V_TH)
			{
				mask.at<uchar>(i, j) = 255;
				state_map.at<uchar>(i, j) = 128;
			}


		}
	}
}

//������̬ѧ�����������գ����߱յ���  //����̬ѧ���������˲���ȥ������
void TLR::Morph(Mat& src, Mat& dst, int opWidth, int opHeight, int clWidth, int clHeight, int model)
{
	Mat open_element = getStructuringElement(MORPH_ELLIPSE, Size(opWidth, opHeight));  //���������Ǿ���˵Ĵ�С
	Mat close_element = getStructuringElement(MORPH_ELLIPSE, Size(clWidth, clHeight));

	Mat open, close;
	if (model==1)  //model��1���ȿ��ٱգ��������ţ�ƣ�
	{
		morphologyEx(src, open, MORPH_OPEN, open_element);
		morphologyEx(open, dst, MORPH_CLOSE, close_element);
	}
	else if (model==0)  //�ղ���
	{
		morphologyEx(src, dst, MORPH_CLOSE, close_element);
		//morphologyEx(close, dst, MORPH_OPEN, open_element);
		//imshow("kai", open);
		imshow("yahaha", dst);
		waitKey(0);
	}

	else if (model == 2)  //pengzhang
	{
		morphologyEx(src, dst, MORPH_DILATE, close_element);
	}
	else if (model == 3)
	{
	//	morphologyEx(src, open, MORPH_DILATE, close_element);
		morphologyEx(src, dst, MORPH_CROSS, close_element);
	}

}

//Ѱ���źŵ�������mask���棩
void TLR::findBulb(Mat& mask,vector<Rect>& bulb)
{
	vector<vector<Point>> contours;  //���������ҵ�������
	vector<Vec4i> hierarchy; //����㼶�����洢���ĸ�����int
	//imshow("mask1", mask);
	findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);  //ֻ�������Χ������������յ���Ϣ����ʱ��show�����Ļ����ǵ��ͼ

	vector<vector<Point>> conPoints(contours.size());  //ǳ����������������ôͼƬ�����ǲ���Ҳ����ֱ������ǳ������
	vector<Rect> boundRect(contours.size());

	int max = 0;
	int min = 0;
	float aspectRatio = .0f;

	for (size_t i = 0; i != contours.size(); ++i)  //����ÿһ������
	{
		//drawContours(mask, contours, i, 255, 1, 8, vector<Vec4i>(), 0, Point());//��ʱ��showmask��������ͼ���ѵ����Ϣ�����������Ϣ��

		boundRect[i] = boundingRect(Mat(contours[i]));   //boundingRect �ҵ�һ�����ο������������
		//rectangle(mask, cvPoint(100, 150), cvPoint(150, 500), Scalar(0, 0, 255), 2, 8, 0);


		min = MIN(boundRect[i].height, boundRect[i].width);
		max = MAX(boundRect[i].height, boundRect[i].width);

		aspectRatio =(float)max /(float)min;

		if (MIN_ASPECT<aspectRatio < MAX_ASPECT && max<MAX_WIDTH && min>MIN_WIDTH)
		{
			bulb.push_back(boundRect[i]);  //�Ѹ߿�ȷ���Ҫ��Ŀ�ӵ���bulb����ȥ
			
		}
		
	}
}

void TLR::findBoxShape(Mat& src, Mat& mask, vector<Rect>& bulb)
{
	vector<vector<Point>> contours;  //���������ҵ�������
	vector<Vec4i> hierarchy; //����㼶�����洢���ĸ�����int
	//imshow("mask1", mask);
	findContours(mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);  //ֻ�������Χ������������յ���Ϣ����ʱ��show�����Ļ����ǵ��ͼ

	vector<vector<Point>> conPoints(contours.size());  //ǳ����������������ôͼƬ�����ǲ���Ҳ����ֱ������ǳ������
	vector<Rect> boundRect(contours.size());
	vector<vector<Point>>hull(contours.size());

	int max = 0;
	int min = 0;
	float aspectRatio = .0f;
	float hullRatio = .0f;

	for (size_t i = 0; i != contours.size(); ++i)  //����ÿһ������
	{

		convexHull(Mat(contours[i]), hull[i], false);  //�ҵ�������͹��
		boundRect[i] = boundingRect(Mat(contours[i]));   //boundingRect �ҵ�һ�����ο������������
		
		

		/*min = MIN(boundRect[i].height, boundRect[i].width);
		max = MAX(boundRect[i].height, boundRect[i].width);*/
		hullRatio = ((float)contourArea(contours[i])) / ((float)contourArea(hull[i]));
		//aspectRatio = (float)max / (float)min;
		aspectRatio = (float)boundRect[i].height / (float)boundRect[i].width;

		if (MIN_ASPECT<aspectRatio && aspectRatio < MAX_ASPECT && hullRatio>HU_TH && boundRect[i].width>MIN_WIDTH)
		{
			bulb.push_back(boundRect[i]);  //�Ѹ߿�ȷ���Ҫ��Ŀ�ӵ���bulb����ȥ

		}
		/*for (size_t i = 0; i!= bulb.size(); ++i)
		{
		rectangle(src, bulb[i].tl(), bulb[i].br(), Scalar(0, 0, 255), 2, 8, 0);

		}*/
	}
	
}

//	//����Ϊ�����������
//	Mat drawing = mask.clone();
//	/*for (int i = 0; i< contours.size(); i++)
//	{
//		rectangle(drawing,boundRect[i].tl(), boundRect[i].br(), 255, 2, 8, 0);
//
//	}
//*/
//	/// ��ʾ��һ������  
	/*imshow("Contours", mask);
	waitKey(0);
*/



//��RGB��ɫ��ֵ�ҵ��źŵƵĿ�ĺ�ɫ����+HSV�ҵ���ĵƵĲ���
void TLR::findBox(Mat& src, Mat& box_mask,Mat& mask,Mat& state_map)
{
	unsigned char r, g, b;
	for (int i = 0; i < src.rows; i++){
		for (int j = 0; j < src.cols; j++)
		{
			b = src.at<Vec3b>(i, j)[0];
			g = src.at<Vec3b>(i, j)[1];
			r = src.at<Vec3b>(i, j)[2];

			if (MAX(r, g, b) < BOX_TH1)
			//if (r+g+b<BOX_TH1)
			{
				box_mask.at<uchar>(i, j) = 255;
			}
		}
	}
	Mat HSV;
	unsigned char h, s, v;
	cvtColor(src, HSV, CV_BGR2HSV);  //rgbԭͼת��Ϊhsvͼ

	for (int i = 0; i < HSV.rows; i++){
		for (int j = 0; j < HSV.cols; j++)
		{
			h = HSV.at<Vec3b>(i, j)[0];
			s = HSV.at<Vec3b>(i, j)[1];
			v = HSV.at<Vec3b>(i, j)[2];

			if ((h<H_R1 || h>H_R2) && s>S_TH && v>V_TH)
			{
				mask.at<uchar>(i, j) = 255;
				box_mask.at<uchar>(i, j) = 255;
				state_map.at<uchar>(i, j) = 255;
			}

			if (h > H_G1 && h<H_G2 && s>S_TH && v > V_TH)
			{
				mask.at<uchar>(i, j) = 255;
				box_mask.at<uchar>(i, j) = 255;
				state_map.at<uchar>(i, j) = 255;
			}


		}
	}
	
}

//�ж�����������ɫ��Ϣ
void TLR::findState(Mat& state_map, Rect& bulb, int& State)
{
	int count_red = 0, count_green = 0;
	int flag=0;
	for (int i = bulb.y; i < (bulb.y + bulb.height);++i)
	{
		for (int j = bulb.x; j < (bulb.x + bulb.width);++j)
		{
			if (state_map.at<uchar>(i, j) == 255)
			{
				State = 1;
				count_red++;
				flag = 1;
				break;
			}
			else if (state_map.at<uchar>(i, j) == 128)
			{
				State = 2;
				count_green++;
				flag = 1;
				break;
			}
		}
		if (flag == 1) break;
	}

	//if (count_red > ((bulb.height*bulb.width) /3))
	//{
	//	State = 1;  //��ɫ�趨Ϊ1
	//}
	//else if (count_green > (bulb.height*bulb.width) / 3)
	//{
	//	State = 2;
	//}
	//else State = 0;

}

void TLR::showRect(Mat& state_map,Mat& src, vector<Rect>& bulb)
{
	for (size_t i = 0; i != bulb.size(); ++i)
	{
		String color;
		int State = 0;
		findState(state_map, bulb[i],State);
		if (State == 1)//��ɫ��ʱ��
		{
			color = "RED";
			rectangle(src, bulb[i].tl(), bulb[i].br(), Scalar(0, 0, 255), LINE_THICK1, 8, 0);
		}
		else if (State == 2)
		{
			color = "GREEN";
			rectangle(src, bulb[i].tl(), bulb[i].br(), Scalar(0, 255, 0), LINE_THICK1, 8, 0);
		}
		//else
		//{
		//	color = nullptr;//��ָ��
		//}

		//putText
	}
}

void TLR::RecognitionLight(Mat& src,Mat& dst)
{
	Mat roi = src(Range(0, src.rows/2 ), Range(0, src.cols));
	Mat mask(roi.size(), CV_8UC1, Scalar(0));
	Mat state_map(roi.size(), CV_8UC1, Scalar(0));
	findHSV(roi, mask, state_map);
	int opWidth = 5;
	int opHeight = 5;
	int clWidth = 9;
	int clHeight = 9;
	Morph(mask, mask, opWidth, opHeight, clWidth, clHeight,1);

	vector<Rect> bulb;
	bulb.resize(0);
	findBulb(mask, bulb);
	//for (size_t i = 0; i != bulb.size(); ++i){
	//		rectangle(dst, bulb[i].tl(), bulb[i].br(), Scalar(0, 0, 255), 2, 8, 0);
	//	}
	//	//imshow("������", dst);

	showRect(state_map, dst, bulb);



}

//�ָ����ݼ��е��źŵ�����
void TLR::dataset(void)
{
	ifstream document("F:\\biaozhu\\label\\picss.txt");
	string s;
	int count = 0;
	int NumGC = 0;
	int NumGL = 0;
	int NumGS = 0;
	int NumGR = 0;
	int NumRC = 0;
	int NumRL = 0;
	int NumRS = 0;
	int NumRR = 0;
	while (document.peek()!=EOF)
	{
		getline(document, s);
		string Img_name=s.substr(20,19)+".bmp";//�ڶ��������������ַ����ĳ���
	    ifstream infile_feat(s); //���������ļ�  
		string feature; //�洢��ȡ��ÿ������  
		float feat_onePoint;  //�洢ÿ�а��ո�ֿ���ÿһ��float����  
		vector<float> lines; //�洢ÿ������  
		vector<vector<float>> lines_feat; //�洢��������  
		lines_feat.clear();
		int counter = 0;
		while (infile_feat.peek()!=EOF)
		{
			getline(infile_feat, feature); //һ�ζ�ȡһ������  
			stringstream stringin(feature); //ʹ�ô���ʵ�ֶ�string�������������  
			lines.clear();
			while (stringin >> feat_onePoint) {      //���ո�һ�ζ�ȡһ�����ݴ���feat_onePoint   
				lines.push_back(feat_onePoint); //�洢ÿ�а��ո�ֿ�������   
			}
			lines_feat.push_back(lines); //�洢��������  ����һ����ά��������
		}

		for (int i = 0; i < lines_feat.size(); i++)
		{
			if (lines_feat[i][0] == 3 || lines_feat[i][1] == 5 || lines_feat[i][1] == 6 || lines_feat[i][1] == 7 || lines_feat[i][2] == 2 || lines_feat[i][5]<15
				|| lines_feat[i][5]>lines_feat[i][6] || (lines_feat[i][6] / lines_feat[i][5]<2))
			{
				break;
			}
			else
			{
				//�и�ͼƬ
				count++;
				//Mat src = imread("F:\\OPENCV\\3\\LightRaw000375.jpg", -1);
				Mat src = imread("I:\\TL\\"+Img_name,-1);
				/*imshow("src", src);
				cvWaitKey(0);
				break;*/
				Mat roi = src(Range(lines_feat[i][4], lines_feat[i][4] + lines_feat[i][6]), Range(lines_feat[i][3], lines_feat[i][3] + lines_feat[i][5]));
				/*imwrite("F:\\biaozhu\\ML2\\" + to_string(count) + ".bmp", roi);
				cout <<Img_name<< ":ok" << endl;*/


				//����txt ��ע�ļ�
				string label;
				

				if (lines_feat[i][0] == 2 && lines_feat[i][1] == 1)
				{
					NumGC++;
					label = "GC";
					if (NumGC <= 190)
					imwrite("F:\\biaozhu\\ML2\\" + label +to_string(NumGC) + ".bmp", roi);
					cout << Img_name << ":ok" << endl;
				}
				else if (lines_feat[i][0] == 2 && lines_feat[i][1] == 2)
				{
					NumGL++;
					label = "GL";
					if (NumGL <= 190)
					imwrite("F:\\biaozhu\\ML2\\" + label + to_string(NumGL) + ".bmp", roi);
					cout << Img_name << ":ok" << endl;
				}
				else if (lines_feat[i][0] == 2 && lines_feat[i][1] == 4)
				{
					NumGS++;
					label = "GS";
					if (NumGS <= 190)
					imwrite("F:\\biaozhu\\ML2\\" + label + to_string(NumGS) + ".bmp", roi);
					cout << Img_name << ":ok" << endl;
				}
				else if (lines_feat[i][0] == 2 && lines_feat[i][1] == 3)
				{
					NumGR++;
					label = "GR";
					if (NumGR<=190)
					imwrite("F:\\biaozhu\\ML2\\" + label + to_string(NumGR) + ".bmp", roi);
					cout << Img_name << ":ok" << endl;
				}
				else if (lines_feat[i][0] == 1 && lines_feat[i][1] == 1)
				{
					NumRC++;
					label = "RC";
					if (NumRC <= 190)
					imwrite("F:\\biaozhu\\ML2\\" + label + to_string(NumRC) + ".bmp", roi);
					cout << Img_name << ":ok" << endl;
				}
				else if (lines_feat[i][0] == 1 && lines_feat[i][1] == 2)
				{
					NumRL++;
					label = "RL";
					if (NumRL <= 190)
					imwrite("F:\\biaozhu\\ML2\\" + label + to_string(NumRL) + ".bmp", roi);
					cout << Img_name << ":ok" << endl;
				}
				else if (lines_feat[i][0] == 1 && lines_feat[i][1] == 4)
				{
					NumRS++;
					label = "RS";
					if (NumRS <= 190)
					imwrite("F:\\biaozhu\\ML2\\" + label + to_string(NumRS) + ".bmp", roi);
					cout << Img_name << ":ok" << endl;
				}
				else if (lines_feat[i][0] == 1 && lines_feat[i][1] == 3)
				{
					NumRR++;
					label = "RR";
					if (NumRR <= 190)
					imwrite("F:\\biaozhu\\ML2\\" + label + to_string(NumRR) + ".bmp", roi);
					cout << Img_name << ":ok" << endl;
				}
				////�ӱ�ǩ���֣����ǵ���knn�����ⲿ��
				//ofstream labelfile;
				//labelfile.open("F:\\biaozhu\\ML2\\" + to_string(count) + ".txt");
				//labelfile << label;
				//labelfile.close();
				
			}
		}
		infile_feat.close();
		ofstream tol;
		tol.open("F:\\biaozhu\\ML2\\tol.txt");
		tol << "GreenCircular:" << to_string(NumGC) << '\n' << "GreenLeft:" << to_string(NumGL) << '\n' << "GreenStraight:" << to_string(NumGS) << '\n' << "GreenRight:" << to_string(NumGR) << '\n'
			<< "RedCircular:" << to_string(NumRC) << '\n' << "RedLeft:" << to_string(NumRL) << '\n' << "RedStraight:" << to_string(NumRS) << '\n' << "RedRight:" << to_string(NumRR) << endl;
		tol.close();
		

	}
}


//�и��ѡ�еĸ���Ȥ���� �����ѡ����Ľṹ����
void TLR::roiPic(Mat& src, vector<candidate>& roiPics,vector<Rect>& bulb)
{
	roiPics.resize(bulb.size());
	for (size_t i = 0; i != bulb.size(); ++i)
	{
		
		roiPics[i].candPics = Mat(src, bulb[i]);
		roiPics[i].candRect = bulb[i];

		
	}
	
}


//������ɫ�ָ��λ���������õ��źŵƵĺ�ѡ���򣬲����ж����źŵƵ�״̬��������struct���棨�죬�̣�
void TLR::candJud(vector<candidate>& roiPics, vector<candidate>& candPics)
{

	for (size_t i = 0; i != roiPics.size(); ++i){

		vector<Rect> bulb;

		Mat mask(roiPics[i].candPics.size(), CV_8UC1, Scalar(0));
		Mat state(roiPics[i].candPics.size(), CV_8UC1, Scalar(0));
		findHSV(roiPics[i].candPics, mask, state); //��HSV��Сģ���Ϸָ����ɫ(�Ѿ���С���)
		findBulb(mask, bulb);  //�ҵ����ڵĿ�
		
		if (bulb.size()==1)    
		{
			if (bulb[0].height < roiPics[i].candRect.height / 2)
			{
				//���￼�ǲ���λ����Ϣ�ж϶��Ƕ��Ž�ȥ//2-��ɫ
				/*if (bulb[0].y < roiPics[i].candRect.height / 2){
					roiPics[i].State = 2;*/
					candPics.push_back(roiPics[i]);

					
				//}
				//else{//1-��ɫ
				//	roiPics[i].State = 1;
				//	candPics.push_back(roiPics[i]);
				//}
			}
		}
		

	}
}


void TLR::Recognition(Mat& src, Mat& dst)
{
	int opWidth = 3;
	int opHeight = 3;
	int clWidth = 3;
	int clHeight = 3;
	Mat roi_img = src(Range(0, src.rows / 2), Range(0, src.cols));
	//�ҿ��mask
	Mat box_map(roi_img.size(), CV_8UC1, Scalar(0));
	Mat morph_map(roi_img.size(), CV_8UC1, Scalar(0));
	//�ҵƵ�mask
	Mat mask(roi_img.size(), CV_8UC1, Scalar(0));
	Mat state_map(roi_img.size(), CV_8UC1, Scalar(0));
	vector<Rect> bulb;
	vector<candidate> roi, cand;
	//�ҵ���
	findBox(roi_img, box_map,mask,state_map);
	//��̬ѧ����
	Morph(box_map, morph_map, opWidth, opHeight, clWidth, clHeight, 1);
	//�����ҵ���ĺ�ѡ����
	findBoxShape(dst, morph_map, bulb);
	roiPic(dst, roi, bulb);
	candJud(roi, cand);
	
		for (size_t i = 0; i < cand.size(); ++i)
		{
			Mat dst(ROI_HEIGHT, ROI_WIDTH, CV_8UC3, Scalar(0, 0, 0));
			resize(cand[i].candPics, dst, dst.size(), 0, 0);  //��һ����С
			Mat mask(dst.size(), CV_8UC1, Scalar(0));
			Mat state(dst.size(), CV_8UC1, Scalar(0));
			findHSV(dst, mask, state); //��HSV��Сģ���Ϸָ����ɫ(�Ѿ���С���)
			

			float result = practice(mask);
			if (result == 2)
			{
				cand[i].State = 2;
			}
			if (result == 1)
			{
				cand[i].State = 1;
			}
			if (result == 3)
			{
				cand[i].State = 3;
			}

			//cout << cand[i].State << endl;
		}

		showBox(dst, cand);


}


void TLR::showBox(Mat& src, vector<candidate>& cand)
{
	for (size_t i = 0; i != cand.size(); ++i)
	{
		String color;
		if (cand[i].State == 2)//��ɫ��ʱ��
		{
			color = "RED";
			rectangle(src, cand[i].candRect.tl(), cand[i].candRect.br(), Scalar(0, 0, 255), LINE_THICK2, 8, 0);
		}
		else if (cand[i].State == 1)
		{
			color = "GREEN";
			rectangle(src, cand[i].candRect.tl(), cand[i].candRect.br(), Scalar(0, 255, 0), LINE_THICK2, 8, 0);
		}
		else if (cand[i].State == 3)
		{
			color = "GREENLEFT";
			rectangle(src, cand[i].candRect.tl(), cand[i].candRect.br(), Scalar(255, 0, 0), LINE_THICK2, 8, 0);
		}
	}
}




void TLR::resizeImg(void)
{
	ifstream document("F:\\biaozhu\\ML2\\pics.txt");
	string s;
	while (document.peek() != EOF)
	{
		getline(document, s);
		string name=s.substr(18);  //��һλĬ�ϵ�����
		cout << name << endl;
		Mat dst(ROI_HEIGHT, ROI_WIDTH, CV_8UC3, Scalar(0, 0, 0));
		Mat src = imread(s, -1);
		resize(src, dst, dst.size(), 0, 0);   //Ĭ��ʹ��˫���Բ�ֵ,
		/*imshow("yahaha", dst);
		waitKey(0);*/
		imwrite("F:\\biaozhu\\�ɹ�ͼ\\"+name, dst);
	}
}




//////////////////////����ѧϰ����
//��ʼ������ѧϰ����
void TLR::kNclassifer(void)
{
	//ѵ���õ�����������
	train_samples = 9;
	classes = 3;
	//ѵ���õ�ͬһͼƬ��С
	size_height = 45;
	size_width = 20;
	//����ѵ�������ݾ���
	trainData.create(train_samples*classes, size_height*size_width, CV_32FC1);  //ȡ��Ȥ����
	//trainData.create(train_samples*classes, size_height*size_width, CV_32FC1);
	trainClasses.create(train_samples*classes, 1, CV_32FC1);

	/*printf(" ------------------------------------------------------------------------\n");
	printf("|\tʶ����\t|\t ���Ծ���\t|\t  ׼ȷ��\t|\n");
	printf(" ------------------------------------------------------------------------\n");
	getData();
	train();
	test();*/
}

void TLR::getData(void)
{
	Mat src;
	int i, j;
	for (i = 0; i < classes; i++){
		for (j = 0; j < train_samples; j++)
		{
			
			//string filename = "F:\\biaozhu\\�ɹ�ͼ\\" +  to_string(i + 1) + "\\" + to_string(j + 1) + ".bmp";
			string filename = "F:\\biaozhu\\��ͷʵ��\\" + to_string(i + 1) + "\\" + to_string(j + 1) + ".bmp";
			//cout << filename << endl;
			src = imread(filename,0);  //������0ת�Ҷ�ͼ
			if (src.empty())
			{
				cout << "Error: Cant load image"<<endl;
			}
			//Mat roi = src(Range((src.rows*2) / 3, src.rows), Range(0, src.cols));
			Mat dst;
			Mat dst1(src);
			dst1.convertTo(dst, CV_32FC1);
			float* data1 = trainData.ptr<float>(i*train_samples + j);//ָ��һ��   //ȡ������׵�ַ

			for (int k = 0; k < dst.rows; k++)
			{
				float* data2 = dst.ptr<float>(k);
				for (int l = 0; l < dst.cols; l++)
				{
					data1[k*dst.cols+l] = data2[l];
				}
			}
			
            ////����label��ֵ
			trainClasses.at<float>(i*train_samples + j, 0) = i+1;

			
			
		}
	}
}

void TLR::train(void)
{
	knn = new KNearest(trainData, trainClasses, Mat(), false, K);
}

float TLR::classify(Mat& image)
{

	float result;
	Mat tem1, tem2;
	Mat Nearest;
	Nearest.create(1, K, CV_32FC1);
	result = knn->find_nearest(image, K, tem1, Nearest, tem2);

	int accuracy = 0;
	for (int i = 0; i<K; i++)
	{
		if (Nearest.at<float>(0, i) == result)//ͶƱ��
			accuracy++;
	}
	float pre = 100 * ((float)accuracy / (float)K);

	printf("|\t    %.0f    \t| \t    %.2f%%  \t| \t %d of %d \t| \n", result, pre, accuracy, K);
	printf(" ------------------------------------------------------------------------\n");

	return result;
}

void TLR::test(void)
{
	Mat src;
	//IplImage prs_image;
	Mat row, data;
	int error = 0;
	int testCounter = 0;

	for (int i = 0; i < classes; i++){
		for (int j = 100; j < 60+train_samples; j++)
		{
			string filename = "F:\\biaozhu\\�ɹ�ͼ\\" + to_string(i + 1) + "\\" + to_string(j + 1) + ".bmp";
			src = imread(filename, 0);
			if (src.empty())
			{
				cout << "Error: Cant load image" << endl;
			}
			//Mat roi = src(Range((src.rows*2) / 3, src.rows), Range(0, src.cols));
			Mat dst;
			//Mat dst2(1, (size_height*size_width)/3, CV_32FC1);
			Mat dst2(1, size_height*size_width, CV_32FC1);
			src.convertTo(dst, CV_32FC1);

			float* data1 = dst2.ptr<float>(0);//ָ��һ��   //ȡ������׵�ַ

			for (int k = 0; k < dst.rows; k++)
			{
				float* data2 = dst.ptr<float>(k);
				for (int l = 0; l < dst.cols; l++)
				{
					data1[k*dst.cols + l] = data2[l];
				}
			}
			//cout << dst << endl;
			
			float result=classify(dst2);  //ָ��Ԥ��Ľ��
			if ((int)result != i+1)
				error++;
			testCounter++;
		}
	}

	float totalerror = 100 * (float)error / (float)testCounter;
	printf("����ϵͳ��ʶ��: %.2f%%\n", totalerror);
}


float TLR::practice(Mat& image)
{
	//cout << image << endl;
	Mat row, data;
	Mat dst;
	Mat dst2(1, size_height*size_width, CV_32FC1);
	Mat dst1(image);
	//Mat roi = image(Range((image.rows * 2) / 3, image.rows), Range(0, image.cols));
	dst1.convertTo(dst, CV_32FC1);
	float* data1 = dst2.ptr<float>(0);   //ȡ������׵�ַ

	for (int k = 0; k < dst.rows; k++)
	{
		float* data2 = dst.ptr<float>(k);
		for (int l = 0; l < dst.cols; l++)
		{
			data1[k*dst.cols + l] = data2[l];
		}
	}
	float result = classify(dst2);
	return result;
}
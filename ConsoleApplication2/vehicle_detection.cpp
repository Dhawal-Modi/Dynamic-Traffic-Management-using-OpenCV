#include "stdafx.h"
#include <windows.h>
#include <strsafe.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include "tserial.h"
#define MINSDIZE 1000

using namespace cv;

volatile float cam1_cars;
volatile float cam2_cars;
volatile float cam3_cars;
volatile float cam4_cars;

#define major_part 0.99
#define minor_part (1-major_part)


//void detect(IplImage *img);

const int KEY_SPACE = 32;
const int KEY_ESC = 27;

Tserial *com;
int nCount = 0;
using namespace std;
char recvBuff[16];

struct VehicalCount
{
	int line1;
	int line2;
	int line3;
	int line4;
};


BOOL quit = FALSE;

int time1 = 10;
int time2 = 10;
int state = 0;

void timertick(int count)
{
	char buff[64];
	_itoa_s(count, buff, 64, 10);
	//system("cls");
	cout << "Second  " << buff << "\n";
	/////receive example
	int k = com->getNbrOfBytes();
	if (k)
	{

		com->getArray(&recvBuff[0], k);
		if (recvBuff != NULL)
			cout << "Received " << recvBuff << "\n";

	}

	if (state == 0)
	{
		if (count > time1)
		{
			com->sendArray((char *)"1\r\n", strlen("1\r\n"));
			count = 0;
		}
		state++;
	}
	else if (state == 1)
	{
		if (count > time2)
		{
			com->sendArray((char *)"2\r\n", strlen("2\r\n"));
			count = 0;
		}
		state = 0;
	}
	//com->sendArray((char *)"Hello\r\n", strlen("hello\r\n"));
	//com->sendArray((char *)"1\r\n", strlen("1\r\n"));
}

DWORD WINAPI thread1(__in LPVOID lpParameter) {

	HANDLE hTimer = CreateEvent(NULL, FALSE, FALSE, NULL);
	while (!quit)
	{

		WaitForSingleObject(hTimer, 1000);
		//cout << "1 s\n";
		nCount++;
		timertick(nCount);

	}
	return 0;
}

int mymain()
{
	std::cout << unitbuf;
	DWORD threadID1;
	HANDLE h1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread1, 0, 0, &threadID1);
	//////////////////serial port

	com = new Tserial();
	const char *port = "COM1";
	com->connect(port, 9600, spNONE);
	//////////////////////////Send Example


	//com->sendArray((char *)"1\r\n", strlen("1\r\n"));
	/////////////////////////////


	///////////////////////////////
	/*getchar();
	quit = TRUE;
	CloseHandle(h1);
	Sleep(500);*/
	return 0;
}

DWORD WINAPI Thread_no_1(LPVOID lpParam)
{
	CvCapture *capture;
	IplImage  *frame;
	int input_resize_percent = 100;
	BOOL first = TRUE;

	CvHaarClassifierCascade *cascade;
	CvMemStorage            *storage;

	cascade = (CvHaarClassifierCascade*)cvLoad("cars.xml", 0, 0, 0);
	storage = cvCreateMemStorage(0);
	capture = cvCaptureFromFile("v1.mp4");
	//capture = cvCaptureFromFile("rtsp://admin:admin123@192.168.101.2:554/cam/realmonitor?channel=1&subtype=00&authbasic=YWRtaW46YWRtaW4xMjM=");
	//cvSetCaptureProperty(capture, CV_CAP_PROP_BUFFERSIZE, 3);

	assert(cascade && storage && capture);

	cvNamedWindow("video1", 1);

	IplImage* frame1 = cvQueryFrame(capture);
	frame = cvCreateImage(cvSize((int)((frame1->width*input_resize_percent) / 100), (int)((frame1->height*input_resize_percent) / 100)), frame1->depth, frame1->nChannels);

	int key = 0;
	do
	{
		frame1 = cvQueryFrame(capture);
		
		if (!frame1)
			break;
		
		cvResize(frame1, frame);

		IplImage *img = frame;
		CvSize img_size = cvGetSize(img);
		CvSeq *object = cvHaarDetectObjects(
			img,
			cascade,
			storage,
			1.2, //1.1,//1.5, //-------------------SCALE FACTOR
			1, //2        //------------------MIN NEIGHBOURS
			0, //CV_HAAR_DO_CANNY_PRUNING
			cvSize(30, 30),//cvSize( 30,30), // ------MINSIZE
			img_size //cvSize(70,70)//cvSize(640,480)  //---------MAXSIZE
		);

		
		for (int i = 0; i < (object ? object->total : 0); i++)
		{
			CvRect *r = (CvRect*)cvGetSeqElem(object, i);
			if ((r->height*r->width) > MINSDIZE)
			{
				cvRectangle(img,
					cvPoint(r->x, r->y),
					cvPoint(r->x + r->width, r->y + r->height),
					CV_RGB(255, 0, 0), 2, 8, 0);
			}
		}

		if (first) {
			first == FALSE;
			cam1_cars = object->total;
		}
		else {
			cam1_cars = cam1_cars * (major_part) + (object->total)*(minor_part);
			time1 =  (int)cam1_cars * 5;
		}

		std::cout << "Total: " << cam1_cars << " cars detected." << std::endl;

		cvShowImage("video1", img);

		key = cvWaitKey(33);

		if (key == KEY_SPACE)
			key = cvWaitKey(0);

		if (key == KEY_ESC)
			break;

	} while (1);

	cvDestroyAllWindows();
	cvReleaseImage(&frame);
	cvReleaseCapture(&capture);
	cvReleaseHaarClassifierCascade(&cascade);
	cvReleaseMemStorage(&storage);

	return 0;
}

DWORD WINAPI Thread_no_2(LPVOID lpParam)
{
	CvCapture *capture;
	IplImage  *frame;
	int input_resize_percent = 100;
	BOOL first = TRUE;

	CvHaarClassifierCascade *cascade;
	CvMemStorage            *storage;

	cascade = (CvHaarClassifierCascade*)cvLoad("cars.xml", 0, 0, 0);
	storage = cvCreateMemStorage(0);
	capture = cvCaptureFromFile("sequence2.mpg");
	//capture = cvCaptureFromFile("rtsp://admin:admin123@192.168.101.3:554/cam/realmonitor?channel=1&subtype=00&authbasic=YWRtaW46YWRtaW4xMjM=");
	//cvSetCaptureProperty(capture, CV_CAP_PROP_BUFFERSIZE, 3);

	assert(cascade && storage && capture);

	cvNamedWindow("video2", 1);

	IplImage* frame1 = cvQueryFrame(capture);
	frame = cvCreateImage(cvSize((int)((frame1->width*input_resize_percent) / 100), (int)((frame1->height*input_resize_percent) / 100)), frame1->depth, frame1->nChannels);

	int key = 0;
	do
	{
		frame1 = cvQueryFrame(capture);

		if (!frame1)
			break;

		cvResize(frame1, frame);

		//detect(frame);
		IplImage *img = frame;
		CvSize img_size = cvGetSize(img);
		CvSeq *object = cvHaarDetectObjects(
			img,
			cascade,
			storage,
			1.2, //1.1,//1.5, //-------------------SCALE FACTOR
			1, //2        //------------------MIN NEIGHBOURS
			0, //CV_HAAR_DO_CANNY_PRUNING
			cvSize(75, 75),//cvSize( 30,30), // ------MINSIZE
			img_size //cvSize(70,70)//cvSize(640,480)  //---------MAXSIZE
		);

		
		for (int i = 0; i < (object ? object->total : 0); i++)
		{
			CvRect *r = (CvRect*)cvGetSeqElem(object, i);
			cvRectangle(img,
				cvPoint(r->x, r->y),
				cvPoint(r->x + r->width, r->y + r->height),
				CV_RGB(255, 0, 0), 2, 8, 0);
		}

		if (first) {
			first == FALSE;
			cam2_cars = object->total;
		}
		else {
			cam2_cars = cam2_cars * (major_part)+(object->total)*(minor_part);
			time2 = (int)cam2_cars * 5;
		}

		std::cout << "Total: " << cam2_cars << " cars detected." << std::endl;
		cvShowImage("video2", img);

		key = cvWaitKey(33);

		if (key == KEY_SPACE)
			key = cvWaitKey(0);

		if (key == KEY_ESC)
			break;

	} while (1);

	cvDestroyAllWindows();
	cvReleaseImage(&frame);
	cvReleaseCapture(&capture);
	cvReleaseHaarClassifierCascade(&cascade);
	cvReleaseMemStorage(&storage);

	return 0;

}

DWORD WINAPI Thread_no_3(LPVOID lpParam)
{
	CvCapture *capture;
	IplImage  *frame;
	int input_resize_percent = 100;
	BOOL first = TRUE;

	CvHaarClassifierCascade *cascade;
	CvMemStorage            *storage;

	cascade = (CvHaarClassifierCascade*)cvLoad("cars.xml", 0, 0, 0);
	storage = cvCreateMemStorage(0);
	capture = cvCaptureFromFile("sequence3.mpg");
	//capture = cvCaptureFromFile("rtsp://admin:admin123@192.168.101.4:554/cam/realmonitor?channel=1&subtype=01&authbasic=YWRtaW46YWRtaW4xMjM=");
	//cvSetCaptureProperty(capture, CV_CAP_PROP_BUFFERSIZE, 3);

	assert(cascade && storage && capture);

	cvNamedWindow("video3", 1);

	IplImage* frame1 = cvQueryFrame(capture);
	frame = cvCreateImage(cvSize((int)((frame1->width*input_resize_percent) / 100), (int)((frame1->height*input_resize_percent) / 100)), frame1->depth, frame1->nChannels);

	int key = 0;
	do
	{
		frame1 = cvQueryFrame(capture);

		if (!frame1)
			break;

		cvResize(frame1, frame);

		//detect(frame);
		IplImage *img = frame;
		CvSize img_size = cvGetSize(img);
		CvSeq *object = cvHaarDetectObjects(
			img,
			cascade,
			storage,
			1.1, //1.1,//1.5, //-------------------SCALE FACTOR
			2, //2        //------------------MIN NEIGHBOURS
			0, //CV_HAAR_DO_CANNY_PRUNING
			cvSize(0, 0),//cvSize( 30,30), // ------MINSIZE
			img_size //cvSize(70,70)//cvSize(640,480)  //---------MAXSIZE
		);

		for (int i = 0; i < (object ? object->total : 0); i++)
		{
			CvRect *r = (CvRect*)cvGetSeqElem(object, i);
			cvRectangle(img,
				cvPoint(r->x, r->y),
				cvPoint(r->x + r->width, r->y + r->height),
				CV_RGB(255, 0, 0), 2, 8, 0);
		}

		if (first) {
			first == FALSE;
			cam3_cars = object->total;
		}
		else {
			cam3_cars = cam3_cars * (major_part)+(object->total)*(minor_part);
		}

		std::cout << "Total: " << cam3_cars << " cars detected." << std::endl;

		cvShowImage("video3", img);

		key = cvWaitKey(33);

		if (key == KEY_SPACE)
			key = cvWaitKey(0);

		if (key == KEY_ESC)
			break;

	} while (1);

	cvDestroyAllWindows();
	cvReleaseImage(&frame);
	cvReleaseCapture(&capture);
	cvReleaseHaarClassifierCascade(&cascade);
	cvReleaseMemStorage(&storage);

	return 0;

}

DWORD WINAPI Thread_no_4(LPVOID lpParam)
{
	CvCapture *capture;
	IplImage  *frame;
	int input_resize_percent = 100;
	BOOL first = TRUE;

	CvHaarClassifierCascade *cascade;
	CvMemStorage            *storage;

	cascade = (CvHaarClassifierCascade*)cvLoad("cars.xml", 0, 0, 0);
	storage = cvCreateMemStorage(0);
	capture = cvCaptureFromFile("sequence_high.mpg");
	//capture = cvCaptureFromFile("rtsp://admin:admin123@192.168.101.5:554/cam/realmonitor?channel=1&subtype=01&authbasic=YWRtaW46YWRtaW4xMjM=");
	//cvSetCaptureProperty(capture, CV_CAP_PROP_BUFFERSIZE, 3);

	assert(cascade && storage && capture);

	cvNamedWindow("video4", 1);

	IplImage* frame1 = cvQueryFrame(capture);
	frame = cvCreateImage(cvSize((int)((frame1->width*input_resize_percent) / 100), (int)((frame1->height*input_resize_percent) / 100)), frame1->depth, frame1->nChannels);

	int key = 0;
	do
	{
		frame1 = cvQueryFrame(capture);

		if (!frame1)
			break;

		cvResize(frame1, frame);

		//detect(frame);
		IplImage *img = frame;
		CvSize img_size = cvGetSize(img);
		CvSeq *object = cvHaarDetectObjects(
			img,
			cascade,
			storage,
			1.1, //1.1,//1.5, //-------------------SCALE FACTOR
			1, //2        //------------------MIN NEIGHBOURS
			0, //CV_HAAR_DO_CANNY_PRUNING
			cvSize(0, 0),//cvSize( 30,30), // ------MINSIZE
			img_size //cvSize(70,70)//cvSize(640,480)  //---------MAXSIZE
		);

		for (int i = 0; i < (object ? object->total : 0); i++)
		{
			CvRect *r = (CvRect*)cvGetSeqElem(object, i);
			cvRectangle(img,
				cvPoint(r->x, r->y),
				cvPoint(r->x + r->width, r->y + r->height),
				CV_RGB(255, 0, 0), 2, 8, 0);
		}

		if (first) {
			first == FALSE;
			cam4_cars = object->total;
		}
		else {
			cam4_cars = cam4_cars * (major_part)+(object->total)*(minor_part);
		}

		std::cout << "Total: " << cam4_cars << " cars detected." << std::endl;

		cvShowImage("video4", img);

		key = cvWaitKey(33);

		if (key == KEY_SPACE)
			key = cvWaitKey(0);

		if (key == KEY_ESC)
			break;

	} while (1);

	cvDestroyAllWindows();
	cvReleaseImage(&frame);
	cvReleaseCapture(&capture);
	cvReleaseHaarClassifierCascade(&cascade);
	cvReleaseMemStorage(&storage);

	return 0;

}

//////////////////////////////////////////////////////////////////////////////
int main()
{
	mymain();
	// Aray to store thread handles
	HANDLE Array_Of_Thread_Handles[4];

	HANDLE Handle_Of_Thread_1 = 0;
	int Data_Of_Thread_1 = 1;
	Handle_Of_Thread_1 = CreateThread(NULL, 0,Thread_no_1, &Data_Of_Thread_1, 0, NULL);
	if (Handle_Of_Thread_1 == NULL)
		ExitProcess(Data_Of_Thread_1);
	

	HANDLE Handle_Of_Thread_2 = 0;
	int Data_Of_Thread_2 = 1;
	Handle_Of_Thread_2 = CreateThread(NULL, 0,Thread_no_2, &Data_Of_Thread_2, 0, NULL);
	if (Handle_Of_Thread_2 == NULL)
		ExitProcess(Data_Of_Thread_2);
	
	HANDLE Handle_Of_Thread_3 = 0;
	int Data_Of_Thread_3 = 1;
	Handle_Of_Thread_3 = CreateThread(NULL, 0,Thread_no_3, &Data_Of_Thread_3, 0, NULL);
	if (Handle_Of_Thread_3 == NULL)
		ExitProcess(Data_Of_Thread_3);
	
	HANDLE Handle_Of_Thread_4 = 0;
	int Data_Of_Thread_4 = 1;
	Handle_Of_Thread_4 = CreateThread(NULL, 0,Thread_no_4, &Data_Of_Thread_4, 0, NULL);
	if (Handle_Of_Thread_4 == NULL)
		ExitProcess(Data_Of_Thread_4);


	Array_Of_Thread_Handles[0] = Handle_Of_Thread_1;
	Array_Of_Thread_Handles[1] = Handle_Of_Thread_2;
	Array_Of_Thread_Handles[2] = Handle_Of_Thread_3;
	Array_Of_Thread_Handles[3] = Handle_Of_Thread_4;

	WaitForMultipleObjects(4,Array_Of_Thread_Handles, TRUE, INFINITE);

	CloseHandle(Handle_Of_Thread_1);
	CloseHandle(Handle_Of_Thread_2);
	CloseHandle(Handle_Of_Thread_3);
	CloseHandle(Handle_Of_Thread_4);
}

//void detect(IplImage *img)
//{
//	CvSize img_size = cvGetSize(img);
//	CvSeq *object = cvHaarDetectObjects(
//		img,
//		cascade,
//		storage,
//		1.1, //1.1,//1.5, //-------------------SCALE FACTOR
//		1, //2        //------------------MIN NEIGHBOURS
//		0, //CV_HAAR_DO_CANNY_PRUNING
//		cvSize(0, 0),//cvSize( 30,30), // ------MINSIZE
//		img_size //cvSize(70,70)//cvSize(640,480)  //---------MAXSIZE
//	);
//
//	std::cout << "Total: " << object->total << " cars detected." << std::endl;
//	for (int i = 0; i < (object ? object->total : 0); i++)
//	{
//		CvRect *r = (CvRect*)cvGetSeqElem(object, i);
//		cvRectangle(img,
//			cvPoint(r->x, r->y),
//			cvPoint(r->x + r->width, r->y + r->height),
//			CV_RGB(255, 0, 0), 2, 8, 0);
//	}
//
//	cvShowImage("video", img);
//}
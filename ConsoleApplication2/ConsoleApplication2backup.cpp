// ConsoleApplication2.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <windows.h> <WINDOWS.H>
#include <strsafe.h> <STRSAFE.H>
#include <stdio.h><STDIO.H>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>

using namespace cv;



//void detect(IplImage *img);

const int KEY_SPACE = 32;
const int KEY_ESC = 27;

DWORD WINAPI Thread_no_1(LPVOID lpParam)
{
	CvCapture *capture;
	IplImage  *frame;
	int input_resize_percent = 100;

	CvHaarClassifierCascade *cascade;
	CvMemStorage            *storage;

	cascade = (CvHaarClassifierCascade*)cvLoad("cars.xml", 0, 0, 0);
	storage = cvCreateMemStorage(0);
	capture = cvCaptureFromFile("rtsp://admin:admin123@192.168.101.2:554/cam/realmonitor?channel=1&subtype=01&authbasic=YWRtaW46YWRtaW4xMjM=");

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
			1.1, //1.1,//1.5, //-------------------SCALE FACTOR
			1, //2        //------------------MIN NEIGHBOURS
			0, //CV_HAAR_DO_CANNY_PRUNING
			cvSize(0, 0),//cvSize( 30,30), // ------MINSIZE
			img_size //cvSize(70,70)//cvSize(640,480)  //---------MAXSIZE
		);

		std::cout << "Total: " << object->total << " cars detected." << std::endl;
		for (int i = 0; i < (object ? object->total : 0); i++)
		{
			CvRect *r = (CvRect*)cvGetSeqElem(object, i);
			cvRectangle(img,
				cvPoint(r->x, r->y),
				cvPoint(r->x + r->width, r->y + r->height),
				CV_RGB(255, 0, 0), 2, 8, 0);
		}

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

	CvHaarClassifierCascade *cascade;
	CvMemStorage            *storage;

	cascade = (CvHaarClassifierCascade*)cvLoad("cars.xml", 0, 0, 0);
	storage = cvCreateMemStorage(0);
	capture = cvCaptureFromFile("rtsp://admin:admin123@192.168.101.3:554/cam/realmonitor?channel=1&subtype=01&authbasic=YWRtaW46YWRtaW4xMjM=");

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
			1.1, //1.1,//1.5, //-------------------SCALE FACTOR
			1, //2        //------------------MIN NEIGHBOURS
			0, //CV_HAAR_DO_CANNY_PRUNING
			cvSize(0, 0),//cvSize( 30,30), // ------MINSIZE
			img_size //cvSize(70,70)//cvSize(640,480)  //---------MAXSIZE
		);

		std::cout << "Total: " << object->total << " cars detected." << std::endl;
		for (int i = 0; i < (object ? object->total : 0); i++)
		{
			CvRect *r = (CvRect*)cvGetSeqElem(object, i);
			cvRectangle(img,
				cvPoint(r->x, r->y),
				cvPoint(r->x + r->width, r->y + r->height),
				CV_RGB(255, 0, 0), 2, 8, 0);
		}

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

	CvHaarClassifierCascade *cascade;
	CvMemStorage            *storage;

	cascade = (CvHaarClassifierCascade*)cvLoad("cars.xml", 0, 0, 0);
	storage = cvCreateMemStorage(0);
	capture = cvCaptureFromFile("rtsp://admin:admin123@192.168.101.4:554/cam/realmonitor?channel=1&subtype=01&authbasic=YWRtaW46YWRtaW4xMjM=");

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
			1, //2        //------------------MIN NEIGHBOURS
			0, //CV_HAAR_DO_CANNY_PRUNING
			cvSize(0, 0),//cvSize( 30,30), // ------MINSIZE
			img_size //cvSize(70,70)//cvSize(640,480)  //---------MAXSIZE
		);

		std::cout << "Total: " << object->total << " cars detected." << std::endl;
		for (int i = 0; i < (object ? object->total : 0); i++)
		{
			CvRect *r = (CvRect*)cvGetSeqElem(object, i);
			cvRectangle(img,
				cvPoint(r->x, r->y),
				cvPoint(r->x + r->width, r->y + r->height),
				CV_RGB(255, 0, 0), 2, 8, 0);
		}

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

	CvHaarClassifierCascade *cascade;
	CvMemStorage            *storage;

	cascade = (CvHaarClassifierCascade*)cvLoad("cars.xml", 0, 0, 0);
	storage = cvCreateMemStorage(0);
	capture = cvCaptureFromFile("rtsp://admin:admin123@192.168.101.5:554/cam/realmonitor?channel=1&subtype=01&authbasic=YWRtaW46YWRtaW4xMjM=");

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

		std::cout << "Total: " << object->total << " cars detected." << std::endl;
		for (int i = 0; i < (object ? object->total : 0); i++)
		{
			CvRect *r = (CvRect*)cvGetSeqElem(object, i);
			cvRectangle(img,
				cvPoint(r->x, r->y),
				cvPoint(r->x + r->width, r->y + r->height),
				CV_RGB(255, 0, 0), 2, 8, 0);
		}

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

	// Aray to store thread handles 
	HANDLE Array_Of_Thread_Handles[4];

	HANDLE Handle_Of_Thread_1 = 0;
	int Data_Of_Thread_1 = 1;
	Handle_Of_Thread_1 = CreateThread(NULL, 0,Thread_no_1, &Data_Of_Thread_1, 0, NULL);
	if (Handle_Of_Thread_1 == NULL)
		ExitProcess(Data_Of_Thread_1);
	while (1);
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
	while (1) {
		Sleep(10);
	}

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




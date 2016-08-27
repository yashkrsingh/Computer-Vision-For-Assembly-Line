#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/video/tracking.hpp"
#include <stdio.h>
#include <string>
#include <conio.h>
#include <iostream>
#include <cstring>
/*
* Defining Constants for display and trackbars
*/
#define MAX_CONTOUR_LEVELS 10

#define IMG_WIDTH 320  
#define IMG_HEIGHT 240 

#define DEFAULT_TRACKBAR_VAL 128 

int main(int argc, const char * argv[]) {
	/*
	* Creating variables to store image data and related values
	*/
	char quit = 0;
	char pass[10],user[10];
	char grab_frame = 1;
	double area1,area2;
	int errcount = 0;
	int thresh1 = DEFAULT_TRACKBAR_VAL, thresh2 = DEFAULT_TRACKBAR_VAL;
	cvNamedWindow("Template", 0);
	IplImage *small_image = cvCreateImage(cvSize(IMG_WIDTH, IMG_HEIGHT), IPL_DEPTH_8U, 3); 
	IplImage *small_grey_image = cvCreateImage(cvGetSize(small_image), IPL_DEPTH_8U, 1);
	IplImage *edge_image = cvCreateImage(cvGetSize(small_image), IPL_DEPTH_8U, 1);
	IplImage *frame_template= cvCreateImage(cvSize(IMG_WIDTH, IMG_HEIGHT), IPL_DEPTH_8U, 3);

	CvMemStorage *storage = cvCreateMemStorage(0);
	CvSeq *contours = 0,*template_contours=0;
	/*
	* Loop to validate password;
	*//*
	while (true)
	{
		std::cout << "Enter Username";
		std::fgets(user, 6, stdin);
		std::fflush(stdin);
		std::cout << user << "\n";
		std::cout << "Enter Password";
		std::fgets(pass, 5, stdin);
		std::fflush(stdin);
		std::cout << pass << "\n";
		if (strcmp(pass, "pass") || strcmp(user, "admin"))
		{
			std::cout << "Incorrect Username/Password\n";
		}
		else
			break;
	}
*/
	/*
	* Loading and processing Template
	*/

	frame_template = cvLoadImage("C:\\Users\\user\\Desktop\\SE\\arrowTEMP.jpg");
	cvResize(frame_template, small_image, CV_INTER_LINEAR);

	cvCvtColor(small_image, small_grey_image, CV_RGB2GRAY);

	cvCanny(small_grey_image, edge_image, (double)thresh1, (double)thresh2, 3);

	cvDilate(edge_image, small_grey_image, 0, 1);

	cvFindContours(small_grey_image, storage, &template_contours, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

	cvDrawContours(small_image, template_contours, CV_RGB(255, 0, 0), CV_RGB(0, 255, 0), MAX_CONTOUR_LEVELS, 1, CV_AA, cvPoint(0, 0));

	cvShowImage("Template", small_image);


	//CvCapture *camera = cvCreateCameraCapture(0);
	//if (!camera) { 
	//	printf("Could not find a camera to capture from...\n");
	//	return -1;
	//}

	cvNamedWindow("Process_OutputA", 0);
	cvNamedWindow("Process_OutputB", 0);
	cvNamedWindow("Process_OutputC", 0);
	cvNamedWindow("Process_OutputD", 0);
	cvNamedWindow("Process_OutputE", 0);
	cvNamedWindow("Process_OutputF", 0);
	cvNamedWindow("Process_OutputG", 0);
	cvNamedWindow("Process_OutputH", 0);
	cvNamedWindow("Process_OutputI", 0);
	cvNamedWindow("Process_OutputJ", 0);
	cvDestroyAllWindows();

								  
	cvCreateTrackbar("Thresh1", "Process_OutputA", &thresh1, 256, 0);
	cvCreateTrackbar("Thresh2", "Process_OutputA", &thresh2, 256, 0);

	cvSetTrackbarPos("Thresh1", "Process_OutputA", DEFAULT_TRACKBAR_VAL);
	cvSetTrackbarPos("Thresh2", "Process_OutputA", DEFAULT_TRACKBAR_VAL);

	/*
	* Loading and processing Samples
	*/

	while (!quit) {
		IplImage *frame;
		int c = cvWaitKey(30);

		switch (c) {
		case 32:
			grab_frame = !grab_frame;
			break;
		case 27:
			quit = 1;
			break;
		};

		if (!grab_frame)continue;

		/*frame = cvQueryFrame(camera);*/
		frame = cvLoadImage("C:\\Users\\user\\Desktop\\SE\\arrowA.jpg");
		if (!frame)continue;

		cvResize(frame, small_image, CV_INTER_LINEAR);

		cvCvtColor(small_image, small_grey_image, CV_RGB2GRAY);

		cvCanny(small_grey_image, edge_image, (double)thresh1, (double)thresh2, 3);

		cvDilate(edge_image, small_grey_image, 0, 1);

		cvFindContours(small_grey_image, storage, &contours, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

		cvDrawContours(small_image, contours, CV_RGB(255, 0, 0), CV_RGB(0, 255, 0), MAX_CONTOUR_LEVELS, 1, CV_AA, cvPoint(0, 0));

		cvShowImage("Process_OutputA", small_image);

	}
	/*
	* Checking for defects
	*/
	area1 = fabs(cvContourArea((CvContour *)contours, CV_WHOLE_SEQ));

	area2 = fabs(cvContourArea((CvContour *)template_contours, CV_WHOLE_SEQ));
	if (area1 != area2)
	{
		printf("Improper formation of block: Dimention\n");
		errcount++;
	}
	if (contours->v_next->v_next->h_next)
	{
		printf("Improper formation of block: Defect\n");
		errcount++;
	}
	area1 = fabs(cvContourArea((CvContour *)contours->v_next->v_next, CV_WHOLE_SEQ));

	area2 = fabs(cvContourArea((CvContour *)template_contours->v_next->v_next, CV_WHOLE_SEQ));
	if (area1 != area2) 
	{
		printf("Improper formation of block: Slot Dimention\n");
		errcount++;
	}
	std::fflush(stdin);
	scanf("");
	cvDestroyAllWindows();

	/*Defect percentage calculator*/
	if(errcount/10>0.30)
		printf("WARNING: HIGH AMOUNT OF ERRORS DETECTED IN BATCH\n");

	std::fflush(stdin);

	scanf("");

	cvDestroyAllWindows();
	/*cvReleaseCapture(&camera);*/

	cvReleaseMemStorage(&storage);

	cvReleaseImage(&small_image);
	cvReleaseImage(&small_grey_image);
	cvReleaseImage(&edge_image);

	return 0;
}

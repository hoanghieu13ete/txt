#include "imageprocessing.h"
#include "HTTPDownloader.h"
#include "Log.h"

using namespace cv;
using namespace std;

//global varialbe declare
Mat dst_roi;
CascadeClassifier platenumber_cascade;
vector<Rect> boundRectPlateNumber;

//convert image into gray scale mode
void convertToGrayScale(Mat src, Mat& dst)
{
	cvtColor(src, dst, COLOR_BGR2GRAY);
}

/* define class contain information of objects which maybe char to offer filter and sort chars*/
class ContourWithData
{
public:
	// member variables ///////////////////////////////////////////////////////////////////////////
	vector<cv::Point> ptContour;           // contour
	Rect boundingRect;                     // contour bounding rect
	float fltArea;                         // contour area

	///////////////////////////////////////////////////////////////////////////////////////////////
	bool checkIfContourIsValid() {                              // this method is to check whether contour element is valid or not
		if (fltArea < MIN_CONTOUR_AREA || fltArea > MAX_CONTOUR_AREA)
			return false;//contour is invalid                                           
		return true;//contour valid                                           
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	static bool sortByBoundingRectYPosition(const ContourWithData& cwdLeft, const ContourWithData& cwdRight) {      // this function allows us to sort follow Y-axis
		return(cwdLeft.boundingRect.y < cwdRight.boundingRect.y);                                                   // the contours from left to right
	}
	static bool sortByBoundingRectXPosition(const ContourWithData& cwdLeft, const ContourWithData& cwdRight) {      // this function allows us to sort follow X-axis
		return(cwdLeft.boundingRect.x < cwdRight.boundingRect.x);                                                   // the contours from left to right
	}
};

/* Find the height largest of char detected in a list
	input: 
		validContoursWithData: char list.
	output:
		return max hight of char detected in a list.
*/
int FindMaxHightOfChars(vector<ContourWithData> validContoursWithData)
{
	int maxHightOfChars = validContoursWithData[0].boundingRect.height;
	for (size_t i = 1; i < validContoursWithData.size(); i++)
	{
		if (validContoursWithData[i].boundingRect.height > maxHightOfChars)
		{
			maxHightOfChars = validContoursWithData[i].boundingRect.height;
		}
	}
	return maxHightOfChars;
}

/* Find the width largest of char detected in a list
	input: 
		validContoursWithData: char list.
	output:
		return max width of char detected in a list.
*/
int FindMaxWidthOfChars(vector<ContourWithData> validContoursWithData)
{
	int maxWidthOfChars = validContoursWithData[0].boundingRect.width;
	for (size_t i = 1; i < validContoursWithData.size(); i++)
	{
		if (validContoursWithData[i].boundingRect.width > maxWidthOfChars)
		{
			maxWidthOfChars = validContoursWithData[i].boundingRect.width;
		}
	}
	return maxWidthOfChars;
}

/* Get min distance between chars detected in a list
	input:
		validContoursWithData: char list.
	output:
		return min distance between chars detected in a list.
*/
int GetMinDistanceCharsOfRows(vector<ContourWithData> validContoursWithData)
{
	int minDistanceCharsRows = MIN_DISTANCE_OF_CHARS_IN_ROW_DEFAULT;
	if (validContoursWithData.size() >= 2)//check if number of char larger two in a row to detect min distance
	{
		for (size_t i = 1; i < validContoursWithData.size(); i++)
		{
			if (abs(validContoursWithData[i].boundingRect.x - (validContoursWithData[i - 1].boundingRect.x + validContoursWithData[i - 1].boundingRect.width)) < minDistanceCharsRows)
				minDistanceCharsRows = abs(validContoursWithData[i].boundingRect.x - (validContoursWithData[i - 1].boundingRect.x + validContoursWithData[i - 1].boundingRect.width));

		}
	}
	return minDistanceCharsRows; //return min distance
}
/* Get max distance between chars detected in a list
	input: 
		validContoursWithData: char list.
	output:
		return max distance between chars detected in a list.
*/
int GetMaxDistanceCharsOfRows(vector<ContourWithData> validContoursWithData)
{
	int maxDistanceCharsInRows = MAX_DISTANCE_OF_CHARS_IN_ROW_DEFAULT;
	if (validContoursWithData.size() >= 2)//check if number of char larger two in a row to detect max distance
	{
		for (size_t i = 1; i < validContoursWithData.size(); i++)
		{
			if (abs((validContoursWithData[i - 1].boundingRect.x + validContoursWithData[i - 1].boundingRect.width) - validContoursWithData[i].boundingRect.x) > maxDistanceCharsInRows)
				maxDistanceCharsInRows = abs((validContoursWithData[i - 1].boundingRect.x + validContoursWithData[i - 1].boundingRect.width) - validContoursWithData[i].boundingRect.x);
		}
	}
	return maxDistanceCharsInRows;//return max distance
}
// Find chars are broken of up row
/*
	input:
		char list is detected of  row in plate number image.
	result:
		push chars to be break into validContoursWithData.
*/
void FindCharsOfUpRow(vector<ContourWithData> validContoursWithDataUpRow, vector<ContourWithData> validContoursWithDataUpRowOne,
	vector<ContourWithData> validContoursWithDataUpRowTwo, int widthaverageofimage, vector<ContourWithData>& validContoursWithData)
{
	// Find 1st and 2nd char of up row 
	if (validContoursWithDataUpRowTwo.size() == 2 && validContoursWithDataUpRowOne.size() == 0) //check if angle top-left not have char is detected
	{
		ContourWithData charinsert;
		sort(validContoursWithDataUpRowTwo.begin(), validContoursWithDataUpRowTwo.end(), ContourWithData::sortByBoundingRectXPosition);//sort chars by coordinates increase direction of X
		int MinDistanceCharsOfRows = GetMinDistanceCharsOfRows(validContoursWithDataUpRowTwo);//get distance of two char in angle top-right
		Point p1(validContoursWithDataUpRowTwo[0].boundingRect.x - (((validContoursWithDataUpRowTwo[0].boundingRect.x - widthaverageofimage) * 2) +
			validContoursWithDataUpRowTwo[0].boundingRect.width) - 1, validContoursWithDataUpRowTwo[0].boundingRect.y - 1);//determined coordinates 1st of 1st char is break
		Point p2(validContoursWithDataUpRowTwo[0].boundingRect.x - ((validContoursWithDataUpRowTwo[0].boundingRect.x - widthaverageofimage) * 2) + 1,
			validContoursWithDataUpRowTwo[0].boundingRect.y + validContoursWithDataUpRowTwo[0].boundingRect.height + 1);//determined 2nd coordinates of 1st char is break
		Point p3((p1.x - MinDistanceCharsOfRows) - (validContoursWithDataUpRowTwo[0].boundingRect.width) - 1, p1.y);//determined 1st coordinates of 2nd char is break
		Point p4((p2.x - MinDistanceCharsOfRows) - (validContoursWithDataUpRowTwo[0].boundingRect.width) + 1, p2.y);//determined 2nd coordinates of 2nd char is break 
		Rect resultOfCharOne(p1,p2);//create rectangle to push into result is found
		charinsert.boundingRect = resultOfCharOne;
		if(p1.x > 0)
		validContoursWithData.push_back(charinsert);//push result into validContoursWithData
		Rect resultOfCharTwo(p3, p4);
		charinsert.boundingRect = resultOfCharTwo;
		if(p3.x > 0)
		validContoursWithData.push_back(charinsert);//push result into validContoursWithData
	}
	// Find 3rd char of up row
	if (validContoursWithDataUpRowOne.size() == 2 && validContoursWithDataUpRowTwo.size() == 1)//check if angle top-right detected one char
	{
		ContourWithData charinsert;
		sort(validContoursWithDataUpRowOne.begin(), validContoursWithDataUpRowOne.end(), ContourWithData::sortByBoundingRectXPosition);//sort chars by increase direction coordinates X
		int MaxWidthCharsOfRows = FindMaxWidthOfChars(validContoursWithDataUpRowOne);//get width of two char in angle top-left
		int MaxDistanceCharsOfRows = GetMaxDistanceCharsOfRows(validContoursWithDataUpRowOne);//get distance of two char in angle top-left
		if (abs(widthaverageofimage - validContoursWithDataUpRowTwo[0].boundingRect.x) > abs(widthaverageofimage - validContoursWithDataUpRowOne[1].boundingRect.x))//check if angle top-right is lost one char
			//this case char 3rd of up row
		{
			Point p1(validContoursWithDataUpRowTwo[0].boundingRect.x - (MaxDistanceCharsOfRows + MaxWidthCharsOfRows), validContoursWithDataUpRowOne[1].boundingRect.y); //determined coordinates 1st of char is lost
			Point  p2(validContoursWithDataUpRowTwo[0].boundingRect.x - MaxDistanceCharsOfRows, validContoursWithDataUpRowOne[1].boundingRect.y + validContoursWithDataUpRowOne[1].boundingRect.height);//determined coordinates 2nd of char is lost
			Rect resultOfCharOne(p1, p2);//create rectangle to push into validContoursWithData
			charinsert.boundingRect = resultOfCharOne;
			if (p1.x > 0)
			validContoursWithData.push_back(charinsert);//push result into validContoursWithData

		}
	}
}

// Find chars is break of down row
/*
	input:
		char list is detected of down row in plate number image.
	result:
		push chars to be break into validContoursWithData.
*/
void FindCharsOfDownRow(vector<ContourWithData> validContoursWithDataDownRow, vector<ContourWithData> validContoursWithDataDownRowOne,
	vector<ContourWithData> validContoursWithDataDownRowTwo,vector<ContourWithData>& validContoursWithData,int widthaverageofimage)
{
	sort(validContoursWithDataDownRowOne.begin(), validContoursWithDataDownRowOne.end(), ContourWithData::sortByBoundingRectXPosition);//sort chars by coordinates increase direction of X
	sort(validContoursWithDataDownRowTwo.begin(), validContoursWithDataDownRowTwo.end(), ContourWithData::sortByBoundingRectXPosition);//sort chars by coordinates increase direction of X
	if (validContoursWithDataDownRowOne.size() >= 2 && validContoursWithDataDownRowTwo.size() == 2)//check angle bottom-left detected one char for case plate number have nine chars
	{
		int MaxDistanceCharsOfRows1 = GetMaxDistanceCharsOfRows(validContoursWithDataDownRowOne);//get distance of two char in angle bottom-left
		int MaxDistanceCharsOfRows2 = GetMaxDistanceCharsOfRows(validContoursWithDataDownRowTwo);//get distance of two char in angle bottom-right
		if (MaxDistanceCharsOfRows1 > (int)(1.3*MaxDistanceCharsOfRows2) && validContoursWithDataDownRow.size() == 4)//check if 2nd char of bottom row is lost
		{
			ContourWithData charinsert;
			//determined 1st coordinates of char is break
			Point p1(validContoursWithDataDownRowOne[0].boundingRect.x + validContoursWithDataDownRowOne[0].boundingRect.width + MaxDistanceCharsOfRows2 - 3, validContoursWithDataDownRowOne[0].boundingRect.y);
			//determined 2nd coordinates of char is break
			Point p2(validContoursWithDataDownRowOne[0].boundingRect.x + validContoursWithDataDownRowOne[0].boundingRect.width * 2 + MaxDistanceCharsOfRows2 + 1, validContoursWithDataDownRowOne[0].boundingRect.y + validContoursWithDataDownRowOne[0].boundingRect.height);
			Rect result(p1, p2);//create rectangle to push into validContoursWithData
			charinsert.boundingRect = result;
			if (p1.x > 0)
				validContoursWithData.push_back(charinsert);//push result into validContoursWithData
		}
	}
	if (validContoursWithDataDownRow.size() == 3) //check bottom row is lost two char
	{
		if (validContoursWithDataDownRowTwo.size() == 2 && validContoursWithDataDownRowOne.size() == 1)//check angle bottom-left detected two char for case plate number have nine chars
			if (abs(validContoursWithDataDownRowOne[0].boundingRect.x - widthaverageofimage) < abs(validContoursWithDataDownRowTwo[0].boundingRect.x - widthaverageofimage))
			{
				int MaxDistanceCharsOfRows2 = GetMaxDistanceCharsOfRows(validContoursWithDataDownRowTwo);
				int MaxWidthCharsOfRows2 = FindMaxWidthOfChars(validContoursWithDataDownRowTwo);
				ContourWithData charinsert;
				//determined 1st coordinates of 1st char is break
				Point p1(validContoursWithDataDownRowOne[0].boundingRect.x - (MaxDistanceCharsOfRows2 + MaxWidthCharsOfRows2) - 1, validContoursWithDataDownRowOne[0].boundingRect.y - 1);
				//determined 2nd coordinates of 1st char is break
				Point p2(validContoursWithDataDownRowOne[0].boundingRect.x - MaxDistanceCharsOfRows2 + 1, validContoursWithDataDownRowOne[0].boundingRect.y + validContoursWithDataDownRowOne[0].boundingRect.height + 1);
				Rect resultOfCharOne(p1, p2); // create rectangle to push into validContoursWithData
				charinsert.boundingRect = resultOfCharOne; 
				if(p1.x>0)
				validContoursWithData.push_back(charinsert);//push result into validContoursWithData
				//determined 1st coordinates of 2nd char is break
				Point p3(p1.x - (MaxDistanceCharsOfRows2 + MaxWidthCharsOfRows2), p1.y);
				//determined 2nd coordinates of 2nd char is break
				Point p4(p1.x - MaxDistanceCharsOfRows2 + 4, p2.y);
				Rect resultOfCharTwo(p3, p4); // create rectangle to push into validContoursWithData
				charinsert.boundingRect = resultOfCharTwo;
				if(p3.x>0)
				validContoursWithData.push_back(charinsert);//push result into validContoursWithData
			}
	}
}
/* check chars in plate number is broken
	input:
		Mat: image plate number.
		validContoursWithData: char list is detected.
	output:
		push chars to be break into list.
*/
void CheckCharsToBeBreak(Mat plateNumberImage,vector<ContourWithData>& validContoursWithData, vector<ContourWithData> validContoursWithDataUpRow, vector<ContourWithData> validContoursWithDataUpRowOne, vector<ContourWithData> validContoursWithDataUpRowTwo
	,vector<ContourWithData> validContoursWithDataDownRow, vector<ContourWithData> validContoursWithDataDownRowOne, vector<ContourWithData> validContoursWithDataDownRowTwo)
{
	int hightaverageofimage = plateNumberImage.rows / 2 - 10;
	int widthaverageofimage = plateNumberImage.cols / 2;
	for (size_t i = 0; i < validContoursWithData.size(); i++)
	{
		if (validContoursWithData[i].boundingRect.y < hightaverageofimage)
		{
			if (validContoursWithData[i].boundingRect.x < widthaverageofimage)
			{
				validContoursWithDataUpRow.push_back(validContoursWithData[i]);
				validContoursWithDataUpRowOne.push_back(validContoursWithData[i]);
			}
			else
			{
				validContoursWithDataUpRow.push_back(validContoursWithData[i]);
				validContoursWithDataUpRowTwo.push_back(validContoursWithData[i]);
			}
		}
		else
			if (validContoursWithData[i].boundingRect.x < widthaverageofimage)
			{
				validContoursWithDataDownRow.push_back(validContoursWithData[i]);
				validContoursWithDataDownRowOne.push_back(validContoursWithData[i]);
			}
			else
			{
				validContoursWithDataDownRow.push_back(validContoursWithData[i]);
				validContoursWithDataDownRowTwo.push_back(validContoursWithData[i]);
			}
	}
	if (validContoursWithDataDownRow.size() >= 3)
	{
		FindCharsOfDownRow(validContoursWithDataDownRow, validContoursWithDataDownRowOne, validContoursWithDataDownRowTwo, validContoursWithData, widthaverageofimage);
	}
	if (validContoursWithDataUpRow.size() >= 2)
	{
		FindCharsOfUpRow(validContoursWithDataUpRow, validContoursWithDataUpRowOne, validContoursWithDataUpRowTwo, widthaverageofimage, validContoursWithData);
	}
}
/* 
	input:
		plateNumberImage: a number plate.

	output:
		return a vertor vector<Mat> which contain a list of char region ( picture of  char region) .
*/
vector<Mat> DetectAndCutCharRegion(Mat plateNumberImage)
{
	vector<ContourWithData> allContoursWithData;           // declare empty vectors,
	vector<ContourWithData> validContoursWithData;         // we will fill these shortly
	vector<ContourWithData> validContoursWithDataUpRow;
	vector<ContourWithData> validContoursWithDataUpRowOne;
	vector<ContourWithData> validContoursWithDataUpRowTwo;
	vector<ContourWithData> validContoursWithDataDownRow;
	vector<ContourWithData> validContoursWithDataDownRowOne;
	vector<ContourWithData> validContoursWithDataDownRowTwo;

	//Mat plateNumberImageClone = plateNumberImage.clone();
	//name of chars
	string line;

	if (plateNumberImage.empty()) {                                // if unable to open image
		cout << "error: image not read from file\n\n";         // show error message on command line                                                  // and exit program
	}

	Mat matGrayscale;           //
	Mat matBlurred;             // declare more image variables
	Mat matThresh;              //
	Mat matThreshCopy;          //

	cvtColor(plateNumberImage, matGrayscale, CV_BGR2GRAY);         // convert to grayscale
																			// blur
	GaussianBlur(matGrayscale,              // input image
		matBlurred,                // output image
		Size(BLUR_VALUE, BLUR_VALUE),            // smoothing window width and height in pixels
		0);                        // sigma value, determines how much the image will be blurred, zero makes function choose the sigma value

								   // filter image from grayscale to black and white
	adaptiveThreshold(matBlurred,                           // input image
		matThresh,                            // output image
		MAX_THRESH,                                  // make pixels that pass the threshold full white
		ADAPTIVE_THRESH_GAUSSIAN_C,       // use gaussian rather than mean, seems to give better results
		THRESH_BINARY_INV,                // invert so foreground will be white, background will be black
		VALUE_PIXEL_NEIGHBORHOOD,                                   // size of a pixel neighborhood used to calculate threshold value
		WEIGHTED_MEAN_OF_ADAPTIVETHRESHOLD);                                   // constant subtracted from the mean or weighted mean

	matThreshCopy = matThresh.clone();              // make a copy of the thresh image, this in necessary b/c findContours modifies the image
	int plateHeight = matThreshCopy.rows;
	int plateWidth = matThreshCopy.cols;
	vector<vector<Point>> ptContours;        // declare a vector for the contours
	vector<Vec4i> v4iHierarchy;                    // declare a vector for the hierarchy (we won't use this in this program but this may be helpful for reference)

	findContours(matThreshCopy,             // input image, make sure to use a copy since the function will modify this image in the course of finding contours
		ptContours,                             // output contours
		v4iHierarchy,                           // output hierarchy
		cv::RETR_EXTERNAL,                      // retrieve the outermost contours only
		cv::CHAIN_APPROX_SIMPLE);               // compress horizontal, vertical, and diagonal segments and leave only their end points

	for (int i = 0; i < ptContours.size(); i++) {               // for each contour
		ContourWithData contourWithData;                                                    // instantiate a contour with data object
		contourWithData.ptContour = ptContours[i];                                          // assign contour to contour with data
		contourWithData.boundingRect = boundingRect(contourWithData.ptContour);         // get the bounding rect
		contourWithData.fltArea = contourArea(contourWithData.ptContour);               // calculate the contour area
		allContoursWithData.push_back(contourWithData);                                     // add contour with data object to list of all contours with data
	}

	for (int i = 0; i < allContoursWithData.size(); i++)
	{                     // for all contours
		int width = allContoursWithData[i].boundingRect.width;
		int height = allContoursWithData[i].boundingRect.height;

		if (allContoursWithData[i].checkIfContourIsValid())                    // check if valid
		{
			float ratio = width * 1.0 / height;
			if (ratio > RATIO_CHAR_PLATE_MIN && width / height < RATIO_CHAR_PLATE_MAX)
			{
				float ratioWidth = width*1.0 / plateWidth;
				if (ratioWidth > PERCENT_WIDTH - DELTA && ratioWidth < PERCENT_WIDTH + DELTA)
				{
					float ratioHeight = height*1.0 / plateHeight;
					if (ratioHeight > PERCENT_HEIGHT - DELTA && ratioHeight < PERCENT_HEIGHT + DELTA)
					{
						validContoursWithData.push_back(allContoursWithData[i]);            // if so, append to valid contour list
					}
				}
			}
		}
	}
	// check Chars to be break
	if (validContoursWithData.size() >= 6 && validContoursWithData.size() < 9)
	{
		CheckCharsToBeBreak(plateNumberImage, validContoursWithData, validContoursWithDataUpRow, validContoursWithDataUpRowOne, validContoursWithDataUpRowTwo
			, validContoursWithDataDownRow, validContoursWithDataDownRowOne, validContoursWithDataDownRowTwo);
	}
	// sort char is detected
	if(validContoursWithData.size()>=8)
	{
		sort(validContoursWithData.begin(), validContoursWithData.end(), ContourWithData::sortByBoundingRectYPosition);
		sort(validContoursWithData.begin(), validContoursWithData.begin() + 4, ContourWithData::sortByBoundingRectXPosition);
		sort(validContoursWithData.begin() + 4, validContoursWithData.end(), ContourWithData::sortByBoundingRectXPosition);
	}
	vector<Mat> output;
	for (int i = 0; i < validContoursWithData.size(); i++) {
		rectangle(plateNumberImage,	        // draw rectangle on original image
			validContoursWithData[i].boundingRect,	// rect to draw
			Scalar(0, 255, 0),                  // green
			2);
		Mat matROI = plateNumberImage(validContoursWithData[i].boundingRect);          // get ROI image of bounding rect
		output.push_back(matROI);
	}

	rectangle(plateNumberImage, Rect(0, 0, plateNumberImage.cols, plateNumberImage.rows), Scalar(0, 255, 0), 2);

	imwrite(CHAR_DETECT, plateNumberImage);
	return output;
}
/*
	input:
		src: image contain plate number.
		p1,p2:	coordinates of plate number in image.
	output: 
		Return the number plate region with Mat.
*/
Mat cut_plate(Mat src, cv::Point p1, cv::Point p2)
{
	Rect rect_roi(p1, p2);
	Mat dst_roi = src(rect_roi);
	return dst_roi;
}
/*
	input:
		src: picture frame which get from video, contain the number plate region.
		frame_gray: gray picture of src.
	output:
		Return the number plate region with vector<Mat>.
*/
vector<Mat> DetectAndCutNumberPlateRegion(Mat src, Mat frame_gray)
{
	vector<Mat> result;

	platenumber_cascade.detectMultiScale(frame_gray, boundRectPlateNumber, 1.1, 2, 4 | CASCADE_SCALE_IMAGE, Size(30, 30));
	for (size_t i = 0; i < boundRectPlateNumber.size(); i++)
	{
		cv::Point point1(boundRectPlateNumber[i].x, boundRectPlateNumber[i].y);
		cv::Point point2(boundRectPlateNumber[i].x + boundRectPlateNumber[i].width, boundRectPlateNumber[i].y + boundRectPlateNumber[i].height);
		result.push_back(cut_plate(src, point1, point2));
	}

	return result;
}
/* Load xml file for detect region plate number */
void loadCascade()
{
	if (!platenumber_cascade.empty())
	{
		//return if cascade is loaded
		return;
	}
	//if not had load yet
	if (!platenumber_cascade.load(CASCADE_PLATE_FILE))
	{
		LOG("Error: Can't loading Cascade Plate File");

	}
}
/*download cascade file from server*/
bool downloadCascadeFromServer()
{
	HTTPDownloader httpDownload;
	string cascasePlateFile = CASCADE_PLATE_FILE;
	bool result = httpDownload.downLoad(SERVER_HOST + cascasePlateFile, cascasePlateFile);
	return true;
}
/*Convert image into HSV mode and filter
	input:
		src : image in color mode
	output:
		filterWhiteColor : image in binary mode
*/
Mat ConvertIntoHSVMode(Mat src)
{
	Mat hsv_image;
	// Convert input image to HSV
	cvtColor(src, hsv_image, cv::COLOR_BGR2HSV);
	// Threshold the HSV image, keep only the white pixels
	Mat filterWhiteColor;
	inRange(hsv_image, Scalar(70, 70, 70), cv::Scalar(250, 255, 255), filterWhiteColor);
	return filterWhiteColor;
}
/*
	input:
		src: source image
	output:
		out: smooth image use blur linear filter
*/
Mat SmoothingImgaeUseBlurFilter(Mat src)
{
	Mat mat_smooth;
	//smooth the image in the src
	blur(src, mat_smooth, Size(BLUR_VALUE,BLUR_VALUE));
	return mat_smooth;
}
/*
	input:
		src: source image
	output:
		out: smooth image use GaussianBlur linear filter 
*/
Mat SmoothingImgaeUseGaussianBlurFilter(Mat src)
{
	Mat mat_smooth;
	//smooth the image in the src
	GaussianBlur(src, mat_smooth, Size(BLUR_VALUE, BLUR_VALUE), 0, 0);
	return mat_smooth;
}
/*
	input:
		src: source image
	output:
		out: smooth image use medianBlur linear filter
*/
Mat SmoothingImgaeUseMedianBlurFilter(Mat src)
{
	Mat mat_smooth;
	//smooth the image in the src 
	medianBlur(src, mat_smooth, BLUR_VALUE);
	return mat_smooth;
}
/*
	input:
		src: source image
	output:
		out: smooth image use GaussianBlur linear filter
*/
Mat SmoothingImgaeUseBilateralFilter(Mat src)
{
	Mat mat_smooth;
	//smooth the image in the src 
	bilateralFilter(src, mat_smooth, BLUR_VALUE, BLUR_VALUE, BLUR_VALUE);
	return mat_smooth;
}

//////////////////////////////////////////////////////////////////////////
bool checkSize(Rect r)
{
	if (r.width * 1.0 / r.height > 0.8 && r.width * 1.0 / r.height < 2)
	{
		if (r.width * r.height > 1500)
			return true;
	}
	return false;
}

void ConectLine(Mat &binary)
{
	Mat kernel1 = Mat::ones(Size(1, 5), CV_8UC1);
	dilate(binary, binary, kernel1);
	erode(binary, binary, kernel1);

	Mat kernel2 = Mat::ones(Size(5, 1), CV_8UC1);
	dilate(binary, binary, kernel2);
	erode(binary, binary, kernel2);

	Mat kernel3 = (Mat_<uchar>(5, 5) << 1, 1, 0, 0, 0,
		1, 1, 1, 0, 0,
		0, 1, 1, 1, 0,
		0, 0, 1, 1, 1,
		0, 0, 0, 1, 1);
	dilate(binary, binary, kernel3);
	erode(binary, binary, kernel3);

	Mat kernel4 = (Mat_<uchar>(5, 5) << 0, 0, 0, 1, 1,
		0, 0, 1, 1, 1,
		0, 1, 1, 1, 0,
		1, 1, 1, 0, 0,
		1, 1, 0, 0, 0);
	dilate(binary, binary, kernel3);
	erode(binary, binary, kernel3);
}

bool wayToSort(Rect a, Rect b)
{
	return a.width * a.height < b.width * b.height;
}

//list gray.
Rect FindBestMat(vector<Rect> list, Mat frame)
{
	vector<Rect> tmp;
	Mat binary, clone;
	vector<vector<Point>> contours;
	int i = 0;

	for (auto rect : list)
	{
		int count = 0;

		clone = frame(rect).clone();
		//cvtColor(clone, clone, CV_BGR2GRAY);

		resize(clone, clone, Size(150, 150));

		GaussianBlur(clone, clone, Size(5, 5), 0);

		adaptiveThreshold(clone, binary, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 11, 9);

		findContours(binary, contours, RETR_LIST, CV_CHAIN_APPROX_NONE);

		for (int i = 0; i < contours.size(); i++) {
			Rect r = boundingRect(contours[i]);

			if (r.width > 5 && r.width < 30 && r.height > 50 && r.height < 80)
			{
				count++;
			}
		}

		cout << count << endl;

		if (count > 4) {
			tmp.push_back(rect);
		}
		i++;
	}

	if (!tmp.empty()) {
		sort(tmp.begin(), tmp.end(), wayToSort);
		return tmp[0];
	}

	else
		return Rect();
}

//input is gray image
vector<Rect> FindLicensePlate(Mat frame)
{
	vector<Rect> list;

	Mat binary, tmp;

	tmp = frame.clone();

	//find local max
	Mat kernel = Mat::ones(Size(5, 5), CV_8UC1);
	dilate(tmp, tmp, kernel);
	//cal gradient
	tmp.convertTo(tmp, CV_32F, 1 / 255.0);
	// Calculate gradients gx, gy
	Mat gx, gy;
	Sobel(tmp, gx, CV_32F, 1, 0, 1);
	Sobel(tmp, gy, CV_32F, 0, 1, 1);
	Mat mag, angle;
	cartToPolar(gx, gy, mag, angle, 1);

	threshold(mag, binary, 0.1, 255, CV_THRESH_BINARY);

	ConectLine(binary);

	vector<vector<Point>> contours;

	binary.convertTo(binary, CV_8UC1);

	findContours(binary, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	for (int i = 0; i < contours.size(); i++)
	{
		Rect r = boundingRect(contours[i]);

		if (checkSize(r))
		{
			list.push_back(r);
		}
	}

	return list;
}

Rect FindPlate(Mat frame)
{
	vector<Rect> list = FindLicensePlate(frame);
	Rect result = FindBestMat(list, frame);
	return result;
}

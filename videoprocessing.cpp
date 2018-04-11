#include "videoprocessing.h"

using namespace cv;
using namespace std;
using namespace System::Windows::Forms;

/*
This function is to play a video file from path
Param: None
Return: None
*/
void videoprocessing::play()
{
	if (!cap.isOpened())
		cap.open(pathVideo);
}
/*
This function is to init open camera by id before use
Param: int idCamera: id of camera( from camera at 0 -> n)
Return: None
*/
void videoprocessing::init(int idCamera)
{
	if (idCamera < 0) {
		MessageBox::Show("No Camera device, Please check!", "Error");  // check if camera is installed or not
	}
	else
	{
		cap.open(idCamera);
	}
}
/*
This function is to init open camera by ip adrress before use
Param: const string & ipAddress: ip address of camera
Return: None
*/
void videoprocessing::init(const string & ipAddress)
{

	if (ipAddress == "") {
		MessageBox::Show("No Camera device, Please check!", "Error"); // check if camera is installed or not
	}
	else
	{
		cap.open(ipAddress);
	}
}

/*
This function is to get frame from video file or camera capture
Param: None
Return: frame of video to further processing
*/
Mat videoprocessing::getFrame()
{
	if (!cap.isOpened()) // check if open webcam successfully or load video file successfully
	{
		MessageBox::Show("Cannot open Camera or load video file!", "Error"); // if not, print error message
		return Mat();
	}
	
	cap.read(frameofvideo); // get frame
	if (!frameofvideo.data)
	{
		MessageBox::Show("Cannot read Frame from camera or video file!", "Error"); // check if frame is empty or not
		return Mat();
	}
	return frameofvideo;
}

/*
This function is to start IP Camera capture
Param: 
const string& IPCamAddr: IP Camera address
Return: frame of video to further processing
*/
Mat videoprocessing::StartCamProcessing()
{
	return getFrame();
}

/*
This function is to stop Camera capture
Param: None
Return: None
*/
void videoprocessing::StopCamorVideoProcessing() {
	if (cap.isOpened()) {
		cap.release();
	}
}
/* author: Sanket Sudake
 * Editor: Nisarg Patel
 *Date: 18 Aug, 2013
 */

/* Usage :
 * $ cmake .
 * $ make
 * $ ./bgsub
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
	
	/* @frame - raw frame
	 * @back  - background image
	 * @fore  - foreground mask
	 */
    Mat frame, back, fore;
	VideoCapture cap(0);	// Caputure input from camera
	// Read above description
    if(cap.isOpened() == false){
        cout << " Failed to access video camera.";
    }
	BackgroundSubtractorMOG2 bg;
	bg.set("detectShadows", false);
	bg.set("nmixtures", 3);
	bg.set("history", 30);
	// vector of vector of points
	// We connect set of detected points to contours
	// 1 vector of points = 1 contour
	// No of contours detected on fly :-)
	std::vector< std::vector<cv::Point> > contours;
    namedWindow("Frame");
	
	for( ; ;)
	{
		// Grab frame from video at store it into frame
		cap >> frame;
		// Detect foreground objects
		// and write to fore matrix
		bg.operator()(frame, fore);
		// Detect background
		// and store in back matrix
		bg.getBackgroundImage(back);
		// Noise removal, we perform two operations
		// 1. erode
		// 2. dilate
		erode(fore, fore, Mat());
		dilate(fore, fore, Mat());
		// findcountours maps vector of points to contours
		// as described above
		findContours(fore, contours, CV_RETR_EXTERNAL,
					 CV_CHAIN_APPROX_NONE );
		// Now draw those contours on frame with red color
		vector<Point2f>center( contours.size() );
		vector<float>radius( contours.size() );

		drawContours(frame, contours, -1, Scalar(0, 0, 255), 2);
		imshow("Frame", frame);
		
		if(waitKey(30) >= 0) break;
	}

    return 0;
}

/* References:
 * http://personal.ee.surrey.ac.uk/Personal/R.Bowden/
 * publications/avbs01/avbs01.pdf
 * http://mateuszstankiewicz.eu/?p=189
 * Note :
 * This is basic movement detection
 */

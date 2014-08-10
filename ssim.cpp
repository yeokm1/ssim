#include <iostream> // for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O

#define STATUS_PER_NUM_FRAMES 50

using namespace std;
using namespace cv;

Scalar getMSSIM( const Mat& I1, const Mat& I2);

int main(int argc, char *argv[])
{



    int delay = 10;
    if (argc < 5)
    {
        cout << "Not enough parameters" << endl;
        cout << "Command: ssim reference_video.avi test_video.avi x y " << endl;
        cout << "Where x is start frame of reference video and y is the start frame of test video" << endl;

        return -1;
    }


    const string sourceReference = argv[1], sourceCompareWith = argv[2];



    long refFramesToWait = strtol(argv[3],NULL, 10);
    long testFramesToWait = strtol(argv[4],NULL, 10);


    char c;

    unsigned long refFrameNum = 0;
    unsigned long testFrameNum = 0;
    unsigned long comparisonFrameNum = 0;          // Frame counter
    unsigned long long redChannelTotal = 0;
    unsigned long long blueChannelTotal = 0;
    unsigned long long greenChannelTotal = 0;

    VideoCapture captRefrnc(sourceReference), captUndTst(sourceCompareWith);

    if (!captRefrnc.isOpened())
    {
        cout  << "Could not open reference " << sourceReference << endl;
        return -1;
    }

    if (!captUndTst.isOpened())
    {
        cout  << "Could not open case test " << sourceCompareWith << endl;
        return -1;
    }

    Size refS = Size((int) captRefrnc.get(CV_CAP_PROP_FRAME_WIDTH),
                     (int) captRefrnc.get(CV_CAP_PROP_FRAME_HEIGHT)),
                uTSi = Size((int) captUndTst.get(CV_CAP_PROP_FRAME_WIDTH),
                            (int) captUndTst.get(CV_CAP_PROP_FRAME_HEIGHT));

    if (refS != uTSi)
    {
        cout << "Inputs have different size!!! Closing." << endl;
        return -1;
    }

    const char* WIN_UT = "Under Test";
    const char* WIN_RF = "Reference";

    // Windows
    namedWindow(WIN_RF, CV_WINDOW_AUTOSIZE);
    namedWindow(WIN_UT, CV_WINDOW_AUTOSIZE);
    cvMoveWindow(WIN_RF, 400       , 0);         //750,  2 (bernat =0)
    cvMoveWindow(WIN_UT, refS.width, 0);         //1500, 2

    cout << "Reference frame resolution: Width=" << refS.width << "  Height=" << refS.height
         << " of nr#: " << captRefrnc.get(CV_CAP_PROP_FRAME_COUNT) << endl;


    Mat frameReference, frameUnderTest;
    double psnrV;
    Scalar mssimV;

    for(;;) //Show the image captured in the window and repeat
    {
        //Skip frames until both streams have synchronised on their starting frames
        if(refFrameNum <= refFramesToWait || testFrameNum <= testFramesToWait)
        {
            if(refFrameNum <= refFramesToWait){
                captRefrnc >> frameReference;
                refFrameNum++;
            }

            if(testFrameNum <= testFramesToWait){
                captUndTst >> frameUnderTest;
                testFrameNum++;
            }



            continue;
        }


        captRefrnc >> frameReference;
        captUndTst >> frameUnderTest;

        if (frameReference.empty() || frameUnderTest.empty())
        {
            cout << "Program ended" << endl;
            break;
        }


        mssimV = getMSSIM(frameReference, frameUnderTest);

        ++comparisonFrameNum;

        redChannelTotal += mssimV.val[2] * 10000; //To store up to 2 decimal places
        greenChannelTotal += mssimV.val[1] * 10000; //To store up to 2 decimal places
        blueChannelTotal += mssimV.val[0] * 10000; //To store up to 2 decimal places

        if((comparisonFrameNum % STATUS_PER_NUM_FRAMES) == 0){
            cout << "Finished " << setfill('0') << setw(6) << comparisonFrameNum << " frames,";

            float redSimilarity = (redChannelTotal / comparisonFrameNum) / 100.0;
            float greenSimilarity = (greenChannelTotal / comparisonFrameNum) / 100.0;
            float blueSimilarity = (blueChannelTotal / comparisonFrameNum) / 100.0;

            cout << " Similarity so far: "
                 << " R "  << setiosflags(ios::fixed) << setprecision(2) << redSimilarity << "%"
                 << " G "  << setiosflags(ios::fixed) << setprecision(2) << greenSimilarity << "%"
                 << " B "  << setiosflags(ios::fixed) << setprecision(2) << blueSimilarity << "%";

            cout << endl;

        }
        ////////////////////////////////// Show Image /////////////////////////////////////////////
        imshow(WIN_RF, frameReference);
        imshow(WIN_UT, frameUnderTest);

        c = (char)cvWaitKey(delay);
        if (c == 27) break;
    }


    float redSimilarity = (redChannelTotal / comparisonFrameNum) / 100.0;
    float greenSimilarity = (greenChannelTotal / comparisonFrameNum) / 100.0;
    float blueSimilarity = (blueChannelTotal / comparisonFrameNum) / 100.0;

    cout << "Ref start frame: " << refFramesToWait << ", Test start frame: " << testFramesToWait << endl;
    cout << "Final Results" << endl;

    cout << "R "  << setiosflags(ios::fixed) << setprecision(2) << redSimilarity << "%"
        << " G "  << setiosflags(ios::fixed) << setprecision(2) << greenSimilarity << "%"
        << " B "  << setiosflags(ios::fixed) << setprecision(2) << blueSimilarity << "%";

    cout << endl;

    float average = (redSimilarity + greenSimilarity + blueSimilarity) / 3;

    cout << "Similarity = " <<  average << "%" << endl;

    return 0;
}


Scalar getMSSIM( const Mat& i1, const Mat& i2)
{
    const double C1 = 6.5025, C2 = 58.5225;
    /***************************** INITS **********************************/
    int d = CV_32F;

    Mat I1, I2;
    i1.convertTo(I1, d);            // cannot calculate on one byte large values
    i2.convertTo(I2, d);

    Mat I2_2   = I2.mul(I2);        // I2^2
    Mat I1_2   = I1.mul(I1);        // I1^2
    Mat I1_I2  = I1.mul(I2);        // I1 * I2

    /*************************** END INITS **********************************/

    Mat mu1, mu2;                   // PRELIMINARY COMPUTING
    GaussianBlur(I1, mu1, Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, Size(11, 11), 1.5);

    Mat mu1_2   =   mu1.mul(mu1);
    Mat mu2_2   =   mu2.mul(mu2);
    Mat mu1_mu2 =   mu1.mul(mu2);

    Mat sigma1_2, sigma2_2, sigma12;

    GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;

    GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;

    GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;

    ///////////////////////////////// FORMULA ////////////////////////////////
    Mat t1, t2, t3;

    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);                 // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);                 // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

    Mat ssim_map;
    divide(t3, t1, ssim_map);        // ssim_map =  t3./t1;

    Scalar mssim = mean(ssim_map);   // mssim = average of ssim map
    return mssim;
}

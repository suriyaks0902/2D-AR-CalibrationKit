/*
  Suriya Kasiyalan Siva
  Spring 2024
  03/14/2024
  CS 5330 Computer Vision
*/
// Code Description: 
//                  improve a camera calibration and target detection project. First, they find the camera's position using calibration data. 
//                  Then, they visualize the target's orientation by projecting 3D points or displaying axes. Lastly, they create and 
//                  project a virtual 3D object onto the image, enhancing spatial tracking and visualization of virtual objects in the real world.



#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::FileStorage fs("camera_calibration.yml", cv::FileStorage::READ);
    cv::Mat cameraMatrix, distCoeffs;
    fs["cameraMatrix"] >> cameraMatrix;
    fs["distortionCoefficients"] >> distCoeffs;
    fs.release();

    cv::VideoCapture cap(0); // Open the video stream
    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open video stream" << std::endl;
        return -1;
    }

    cv::Size boardSize(9, 6); // Adjust according to your checkerboard size
    std::vector<cv::Point3f> objectPoints;
    float squareSize = 1.0; // Size of each square in the checkerboard

    for (int i = 0; i < boardSize.height; ++i) {
        for (int j = 0; j < boardSize.width; ++j) {
            objectPoints.push_back(cv::Point3f(j * squareSize, i * squareSize, 0));
        }
    }

    // Define 3D points for the virtual object (in world space)
    std::vector<cv::Point3f> objectLines;
    objectLines.push_back(cv::Point3f(2.0, 3.0, 0.0));  // Line start point
    objectLines.push_back(cv::Point3f(5.0, 3.0, 0.0));  // Line end point
    objectLines.push_back(cv::Point3f(5.0, 6.0, 0.0));  // Line start point
    objectLines.push_back(cv::Point3f(2.0, 6.0, 0.0));  // Line end point
    objectLines.push_back(cv::Point3f(2.0, 3.0, 0.0));

    objectLines.push_back(cv::Point3f(2.0, 3.0, -3.0));  // Line start point
    objectLines.push_back(cv::Point3f(5.0, 3.0, -3.0));  // Line end point
    objectLines.push_back(cv::Point3f(5.0, 6.0, -3.0));
    objectLines.push_back(cv::Point3f(2.0, 6.0, -3.0));
    objectLines.push_back(cv::Point3f(2.0, 3.0, -3.0));  // Line start point
    
    objectLines.push_back(cv::Point3f(2.0, 3.0, 0.0));
    objectLines.push_back(cv::Point3f(5.0, 3.0, 0.0));
    objectLines.push_back(cv::Point3f(5.0, 3.0, -3.0));
    objectLines.push_back(cv::Point3f(5.0, 6.0, -3.0));
    objectLines.push_back(cv::Point3f(5.0, 6.0, 0.0));
    objectLines.push_back(cv::Point3f(2.0, 6.0, 0.0));
    objectLines.push_back(cv::Point3f(2.0, 6.0, -3.0));
    objectLines.push_back(cv::Point3f(2.0, 3.0, -3.0));

    // Define 3D points for the virtual object (in world space) - Pyramid base
    
    objectLines.push_back(cv::Point3f(2.0, 3.0, -3.0));  // Line start point
    objectLines.push_back(cv::Point3f(5.0, 3.0, -3.0));  // Line end point
    objectLines.push_back(cv::Point3f(5.0, 6.0, -3.0));  // Line start point
    objectLines.push_back(cv::Point3f(2.0, 6.0, -3.0));  // Line end point
    objectLines.push_back(cv::Point3f(2.0, 3.0, -3.0));  // Closing the loop

    // Define 3D points for the virtual object (in world space) - Pyramid apex
    objectLines.push_back(cv::Point3f(3.5, 4.5, -3.0));  // Apex point

    // Connect apex to each corner of the base to form the pyramid sides
    objectLines.push_back(cv::Point3f(3.5, 4.5, -6.0));  // Connect to first base corner
    objectLines.push_back(cv::Point3f(2.0, 3.0, -3.0));  
    objectLines.push_back(cv::Point3f(5.0, 3.0, -3.0));  // Connect to second base corner
    objectLines.push_back(cv::Point3f(3.5, 4.5, -6.0));  
    objectLines.push_back(cv::Point3f(5.0, 6.0, -3.0));  // Connect to third base corner
    objectLines.push_back(cv::Point3f(3.5, 4.5, -6.0));
    objectLines.push_back(cv::Point3f(5.0, 3.0, -3.0));  // Connect to fourth base corner
    objectLines.push_back(cv::Point3f(3.5, 4.5, -3.0));
    objectLines.push_back(cv::Point3f(2.0, 6.0, -3.0));
    objectLines.push_back(cv::Point3f(3.5, 4.5, -6.0));
    objectLines.push_back(cv::Point3f(3.5, 4.5, -3.0));
    objectLines.push_back(cv::Point3f(5.0, 6.0, -3.0));


    while (true) {
        cv::Mat frame;
        cap >> frame; // Capture frame-by-frame
        if (frame.empty()) {
            std::cerr << "Error: Frame is empty" << std::endl;
            break;
        }

        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        std::vector<cv::Point2f> corners;
        bool found = cv::findChessboardCorners(gray, boardSize, corners);

        if (found) {
            cv::Mat rvec, tvec;
            cv::solvePnP(objectPoints, corners, cameraMatrix, distCoeffs, rvec, tvec);
            // Project 3D points onto the image plane
            std::vector<cv::Point3f> axisPoints;
            axisPoints.push_back(cv::Point3f(0, 0, 0));
            axisPoints.push_back(cv::Point3f(3, 0, 0));
            axisPoints.push_back(cv::Point3f(0, -3, 0));
            axisPoints.push_back(cv::Point3f(0, 0, -3));
            std::vector<cv::Point2f> imagePoints;
            cv::projectPoints(axisPoints, rvec, tvec, cameraMatrix, distCoeffs, imagePoints);

            // Draw projected points or axes
            cv::line(frame, imagePoints[0], imagePoints[1], cv::Scalar(255, 0, 0), 2); // X-axis (blue)
            cv::line(frame, imagePoints[0], imagePoints[2], cv::Scalar(0, 255, 0), 2); // Y-axis (green)
            cv::line(frame, imagePoints[0], imagePoints[3], cv::Scalar(0, 0, 255), 2); // Z-axis (red)
            // Project endpoints of lines into image space
            //std::vector<cv::Point2f> imagePoints;
            cv::projectPoints(objectLines, rvec, tvec, cameraMatrix, distCoeffs, imagePoints);

            // Draw lines between projected points
            for (size_t i = 0; i < imagePoints.size() - 1; ++i) {
                cv::line(frame, imagePoints[i], imagePoints[i + 1], cv::Scalar(0, 0, 255), 2); 
            }

            // Print translation vector
            std::cout << "Translation vector (tvec):" << std::endl << tvec << std::endl;
        }

        cv::imshow("Frame", frame);
        char key = cv::waitKey(1);
        if (key == 'q') { // Break the loop if the 'q' key is pressed
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}

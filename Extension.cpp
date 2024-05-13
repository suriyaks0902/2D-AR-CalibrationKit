/*
  Suriya Kasiyalan Siva
  Spring 2024
  03/18/2024
  CS 5330 Computer Vision
  This is the extension code where I execute each extension in this file
*/

/////////////////////// Get your system working with multiple targets in the scene ///////////////////////////

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

int main() {
   cv::VideoCapture cap(0); // Open the video stream
   if (!cap.isOpened()) {
       std::cerr << "Error: Cannot open video stream" << std::endl;
       return -1;
   }

   cv::Mat frame;
   std::vector<cv::Size> boardSizes = { cv::Size(9, 6), cv::Size(6, 7) };

   while (true) {
       cap >> frame; // Capture frame-by-frame
       if (frame.empty()) {
           std::cerr << "Error: Frame is empty" << std::endl;
           break;
       }

       cv::Mat gray;
       cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

       for (size_t i = 0; i < boardSizes.size(); ++i) {
           std::vector<cv::Point2f> corners_set;
           bool found = false;
           try {
               found = cv::findChessboardCorners(gray, boardSizes[i], corners_set);
           }
           catch (const cv::Exception& e) {
               std::cerr << "OpenCV Exception: " << e.what() << std::endl;
           }

           if (found) {
               cv::cornerSubPix(gray, corners_set, cv::Size(11, 11), cv::Size(-1, -1),
                   cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));
               cv::drawChessboardCorners(frame, boardSizes[i], corners_set, found);
               cv::putText(frame, std::to_string(corners_set.size()), corners_set[0], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
               // Print number of corners and coordinates of the first corner
               std::cout << "Number of corners: " << corners_set.size() << std::endl;
               std::cout << "Coordinates of the first corner: " << corners_set[0] << std::endl;
           }
       }

       cv::imshow("Frame", frame);
       char key = cv::waitKey(1);
       if (key == 'q') { // Break the loop if the 'q' key is pressed
           break;
       }
   }

   cap.release(); // Release the video capture object
   cv::destroyAllWindows();
   return 0;
}

//////////////////////////////////// Enable your system to use static images or pre-captured video sequences with targets and demonstrate inserting virtual objects into the scenes/////////////////////////////////////////

#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Load pre-recorded video
    cv::VideoCapture cap("pre_recorded_video.mp4");
    if (!cap.isOpened()) {
        std::cerr << "Error: Unable to open video file." << std::endl;
        return -1;
    }

    // Load camera calibration parameters
    cv::Mat cameraMatrix, distCoeffs;
    cv::FileStorage fs("camera_calibration.yml", cv::FileStorage::READ);
    fs["cameraMatrix"] >> cameraMatrix;
    fs["distCoeffs"] >> distCoeffs;
    fs.release();

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
    //objectLines.push_back(cv::Point3f(2.0, 3.0, 0.0));  // Line start point
    //objectLines.push_back(cv::Point3f(5.0, 3.0, 0.0));  // Line end point
    //objectLines.push_back(cv::Point3f(5.0, 6.0, 0.0));  // Line start point
    //objectLines.push_back(cv::Point3f(2.0, 6.0, 0.0));  // Line end point
    //objectLines.push_back(cv::Point3f(2.0, 3.0, 0.0));

    //objectLines.push_back(cv::Point3f(2.0, 3.0, -3.0));  // Line start point
    //objectLines.push_back(cv::Point3f(5.0, 3.0, -3.0));  // Line end point
    //objectLines.push_back(cv::Point3f(5.0, 6.0, -3.0));
    //objectLines.push_back(cv::Point3f(2.0, 6.0, -3.0));
    //objectLines.push_back(cv::Point3f(2.0, 3.0, -3.0));  // Line start point

    //objectLines.push_back(cv::Point3f(2.0, 3.0, 0.0));
    //objectLines.push_back(cv::Point3f(5.0, 3.0, 0.0));
    //objectLines.push_back(cv::Point3f(5.0, 3.0, -3.0));
    //objectLines.push_back(cv::Point3f(5.0, 6.0, -3.0));
    //objectLines.push_back(cv::Point3f(5.0, 6.0, 0.0));
    //objectLines.push_back(cv::Point3f(2.0, 6.0, 0.0));
    //objectLines.push_back(cv::Point3f(2.0, 6.0, -3.0));
    //objectLines.push_back(cv::Point3f(2.0, 3.0, -3.0));

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

    // Define the codec and create VideoWriter object
    int codec = cv::VideoWriter::fourcc('m', 'p', '4', 'v'); // MP4V codec
    double fps = cap.get(cv::CAP_PROP_FPS);
    cv::Size frameSize(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    cv::VideoWriter videoWriter("output_video.mp4", codec, fps, frameSize);

    // Main loop for processing video frames
    while (true) {
        cv::Mat frame;
        cap >> frame; // Capture frame from video
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

            // Project endpoints of lines into image space
            std::vector<cv::Point2f> imagePoints;
            cv::projectPoints(objectLines, rvec, tvec, cameraMatrix, distCoeffs, imagePoints);

            // Draw lines between projected points
            for (size_t i = 0; i < imagePoints.size() - 1; ++i) {
                cv::line(frame, imagePoints[i], imagePoints[i + 1], cv::Scalar(0, 0, 255), 2);
            }

            // Print translation vector
            std::cout << "Translation vector (tvec):" << std::endl << tvec << std::endl;
        }
        videoWriter.write(frame);

        // Display the frame with inserted virtual object
        cv::imshow("Frame", frame);

        // Break the loop if 'q' is pressed
        if (cv::waitKey(25) == 'q')
            break;
    }

    // Release video capture and close windows
    cap.release();
    videoWriter.release();
    cv::destroyAllWindows();

    return 0;
}

//////////////////////// Not only add a virtual object but also do something to the target to make it not look like a target anymore ///////////////////////////

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

    //Define 3D points for the pyramid
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

    bool overlayEnabled = false; // Flag to track whether overlay is enabled or not

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
            // Create bounding rectangle around the chessboard pattern
            cv::Rect roi = cv::boundingRect(corners);

            // Overlay an image if the overlay is enabled
            if (overlayEnabled) {
                cv::Mat overlay = cv::imread("batman.jpg");
                if (!overlay.empty()) {
                    // Resize overlay image if needed
                    if (overlay.cols > roi.width || overlay.rows > roi.height) {
                        cv::resize(overlay, overlay, cv::Size(roi.width, roi.height));
                    }

                    // Place the overlay onto the frame within the ROI
                    cv::Mat roiFrame = frame(roi);
                    overlay.copyTo(roiFrame);
                }
            }

            // Project 3D points of the virtual object (pyramid) onto the image plane
            std::vector<cv::Point2f> imagePoints;
            cv::Mat rvec, tvec;
            cv::solvePnP(objectPoints, corners, cameraMatrix, distCoeffs, rvec, tvec);
            cv::projectPoints(objectLines, rvec, tvec, cameraMatrix, distCoeffs, imagePoints);

            // Draw lines between projected points to render the virtual object (pyramid)
            for (size_t i = 0; i < imagePoints.size() - 1; ++i) {
                cv::line(frame, imagePoints[i], imagePoints[i + 1], cv::Scalar(0, 0, 255), 2);
            }

            // Print translation vector (if needed)
            std::cout << "Translation vector (tvec):" << std::endl << tvec << std::endl;
        }

        cv::imshow("Frame", frame);

        // Handle key press events
        char key = cv::waitKey(1);
        if (key == 'q') { // Break the loop if the 'q' key is pressed
            break;
        }
        else if (key == 'm') { // Toggle overlay when 'm' key is pressed
            overlayEnabled = !overlayEnabled;
        }
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}

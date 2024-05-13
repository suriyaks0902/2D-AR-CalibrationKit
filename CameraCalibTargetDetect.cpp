/*
  Suriya Kasiyalan Siva
  Spring 2024
  03/11/2024
  CS 5330 Computer Vision
*/
// Code Description:
//                  aim to establish an effective system for camera calibration and target detection. Initially, 
//                  the system identifies and extracts corners of a specified target, enabling precise localization within images or video streams. 
//                  Subsequently, users can select calibration images to store corner locations and corresponding 3D world points for subsequent calibration. 
//                  Finally, utilizing the collected data, the system calibrates the camera, estimating parameters like camera matrix and distortion coefficients. 
//                  This process ensures accurate mapping between 3D object points and 2D image points, essential for various computer vision applications.

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <direct.h>
#include <errno.h>
#include <cstring>
#include <vector>
#include <string>
#include <opencv2/calib3d.hpp>

#ifdef _WIN32
#define mkdir _mkdir
#endif

int main() {
    cv::VideoCapture cap(0); // Open the video stream
    if (!cap.isOpened()) {
        std::cerr << "Error: Cannot open video stream" << std::endl;
        return -1;
    }
    cv::Mat frame;
    std::vector<cv::Vec3f> point_set; // 3D world point set
    std::vector<std::vector<cv::Vec3f>> point_list;
    std::vector<std::vector<cv::Point2f>> corner_list;
    std::vector<cv::Mat> images;
    std::vector<cv::Size> boardSize = { cv::Size(9, 6), cv::Size(6, 7) };
    int num_calibration_frames = 0;
    double total_reprojection_error = 0.0;
    bool calibration_done = false;

    while (true) {
        cap >> frame; // Capture frame-by-frame
        if (frame.empty()) {
            std::cerr << "Error: Frame is empty" << std::endl;
            break;
        }
        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        /*cv::Size boardSize(9, 6);*/ // Adjust according to your checkerboard size
        
        float squareSize = 1.0; // Size of each square in the checkerboard
        std::vector<cv::Point2f> corners_set; // Vector to store detected corners
        bool found = cv::findChessboardCorners(gray, boardSize[i], corners_set); // Detect the corners of the checkerboard

        if (found) {
            cv::cornerSubPix(gray, corners_set, cv::Size(11, 11), cv::Size(-1, -1),
                cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1)); // Refine corner locations
            cv::drawChessboardCorners(frame, boardSize[i], corners_set, found); // Draw corners on the frame
            // Print number of corners and coordinates of the first corner
            std::cout << "Number of corners: " << corners_set.size() << std::endl;
            std::cout << "Coordinates of the first corner: " << corners_set[0] << std::endl;

            point_set.clear();
            for (int i = 0; i < boardSize.height; ++i) {
                for (int j = 0; j < boardSize.width; ++j) {
                    point_set.push_back(cv::Vec3f(j * squareSize, i * squareSize, 0));
                }
            }
            corner_list.push_back(corners_set);
            point_list.push_back(point_set);
            images.push_back(frame.clone());

            num_calibration_frames++;
            // Perform camera calibration if enough frames are collected
            if (corner_list.size() >= 5 && !calibration_done) {
                std::cout << "entering camera caliberation..." << std::endl;
                cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
                cv::Mat distCoeffs = cv::Mat::zeros(8, 1, CV_64F);
                std::vector<cv::Mat> rvecs, tvecs;
                double reprojectionError = cv::calibrateCamera(point_list, corner_list, frame.size(), cameraMatrix, distCoeffs, rvecs, tvecs);

                std::cout << "Camera Matrix:" << std::endl << cameraMatrix << std::endl;
                std::cout << "Distortion Coefficients:" << std::endl << distCoeffs << std::endl;
                std::cout << "Reprojection Error: " << reprojectionError << std::endl;

                cv::FileStorage fs("camera_calibration.yml", cv::FileStorage::WRITE);
                fs << "cameraMatrix" << cameraMatrix;
                fs << "distortionCoefficients" << distCoeffs;
                fs.release();
                calibration_done = true;
            }
        }
        cv::imshow("Frame", frame);
        char key = cv::waitKey(1);
        /*std::cout << "Calibration done. Press 's' to save, 'q' to quit." << std::endl;*/
        if (calibration_done) {
                       
            if (key == 's') { // Save the corner locations and corresponding 3D world points
                std::ofstream file("calibration_data.txt");
                if (file.is_open()) {
                    for (size_t i = 0; i < corner_list.size(); ++i) {
                        file << "Image " << i + 1 << ":\n";
                        for (size_t j = 0; j < corner_list[i].size(); ++j) {
                            file << "Corner " << j + 1 << ": (" << corner_list[i][j].x << ", " << corner_list[i][j].y << ")\n";
                            file << "3D World Point " << j + 1 << ": (" << point_list[i][j][0] << ", " << point_list[i][j][1] << ", " << point_list[i][j][2] << ")\n";
                        }
                        file << "\n";
                    }
                    std::cout << "Calibration data saved to calibration_data.txt" << std::endl;
                    file.close();
                    // Create a directory for images if it doesn't exist
                    if (mkdir("calibration_images") != 0) {
                        std::cerr << "Error: Unable to create directory" << std::endl;
                    }
                    else {
                        // Save images to the directory
                        for (size_t i = 0; i < images.size(); ++i) {
                            std::string filename = "calibration_images/calibration_image_" + std::to_string(i + 1) + ".jpg";
                            cv::imwrite(filename, images[i]);
                        }
                        std::cout << "Calibration images saved to calibration_images directory" << std::endl;
                    }
                }
                else {
                    std::cerr << "Error: Unable to open calibration_data.txt for writing" << std::endl;
                }

            }
            else if (key == 'q') { // Break the loop if the 'q' key is pressed
                break;
            }
        }
        
    }
    cap.release(); // Release the video capture object
    cv::destroyAllWindows();
    return 0;
}

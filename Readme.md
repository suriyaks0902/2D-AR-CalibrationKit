# Calibration and Augmented Reality

## Overview
This repository contains the source code and documentation for a project on Calibration and Augmented Reality developed as part of CS 5330 - Pattern Recognition and Computer Vision in Spring 2024. The project aims to develop a system capable of calibrating a camera, detecting a target pattern (such as a checkerboard), and accurately placing virtual objects within the scene in real-time using computer vision techniques.

## Introduction
The project's primary goal is to seamlessly integrate virtual objects into real-world scenes captured by a camera. It involves several key steps, including:
- Detecting and Extracting Target Corners
- Selecting Calibration Images
- Calibrating the Camera
- Calculating the Current Position of the Camera
- Projecting Outside Corners or 3D Axes
- Creating Virtual Objects
- Detecting Robust Features

## Project Structure
The project is organized into several modules, each focusing on a specific task. Here's a brief overview:

1. **Detection and Extraction of Target Corners**: This module focuses on detecting and extracting corners of a predefined pattern, such as a checkerboard, within a video stream or series of images. It utilizes OpenCV's capabilities for corner detection and refinement.

2. **Selection of Calibration Images**: This module enables users to specify calibration images and saves corner locations along with corresponding 3D world points.

3. **Camera Calibration**: This module performs camera calibration using collected calibration frames, ensuring accurate intrinsic camera parameters and distortion coefficients.

4. **Calculating the Current Position of the Camera**: This module estimates the camera's position in real-time using pose estimation techniques, projecting 3D points onto the image plane.

5. **Projection of Outside Corners or 3D Axes**: This module enhances the system to project 3D points or axes onto the image plane in real-time.

6. **Creation of Virtual Objects**: This module extends the system to create and project virtual objects in 3D space onto the image, maintaining correct orientation as the camera or target moves.

7. **Detection of Robust Features**: This module explores feature detection techniques, particularly the Harris corner detection algorithm, for identifying salient features in real-time video streams.

## Extensions
The project includes extensions to enhance its functionality:
- Support for multiple targets in the scene
- Comparison of camera calibrations and results for different cameras
- Integration of static images or pre-captured video sequences with virtual objects
- Manipulation of detected targets to make them look like something else

## Video Demonstration
https://github.com/suriyaks0902/2D-AR-CalibrationKit/assets/90970879/983ec71a-46e6-4758-8e78-88b6e1f10832

## Install Dependencies

To run the project, you need to set up Visual Studio and configure OpenCV. Follow these steps for installation:

1. **Install Visual Studio:**
    - Download and install Visual Studio from the official website: [Visual Studio](https://visualstudio.microsoft.com/downloads/).

2. **Install OpenCV:**
    - Download OpenCV library from the official website: [OpenCV](https://opencv.org/releases/).
    - Follow the installation instructions provided on the OpenCV documentation for your operating system.

3. **Configure OpenCV in Visual Studio:**
    - Open Visual Studio.
    - Create a new project or open an existing one.
    - Go to Project Properties > Configuration Properties > VC++ Directories.
    - Add the path to OpenCV include directory to "Include Directories".
    - Add the path to OpenCV library directory to "Library Directories".
    - Go to Linker > Input and add the names of the OpenCV libraries you are using to "Additional Dependencies".
    - Make sure to include necessary header files and link the required libraries in your project.

4. **Build and Run:**
    - Build the project in Visual Studio.
    - Run the executable file generated after successful build.
      
## Usage

To use the program, follow these steps:

1. Start debugging or run the program from Visual Studio.
2. Press keys to activate different functionalities:
    - Press 's' to enter save mode.
    - Press 'm' to apply an overlay image.
    - Press 'q' to exit the program.

**Key Functions:**

- **'s':** Enter save mode.
  - Saves all the corner points and corresponding 3D world points.
  - Stores the corresponding images.

- **'m':** Apply an overlay image.
  - Makes the chessboard pattern look like something else.

- **'q':** Exit the program.

## Learning Reflection
The project provided valuable hands-on experience in computer vision, augmented reality, and related techniques. It deepened understanding of fundamental concepts and practical applications, equipping with skills applicable across various domains.

## Acknowledgement
The project heavily relies on OpenCV documentation and resources, supplemented by online tutorials and forums for further understanding and implementation techniques.

## Contact for More Information

For more information or assistance regarding this project, feel free to contact:

Suriya Kasiyalan Siva

Email: k.s.suriya0902@gmail.com


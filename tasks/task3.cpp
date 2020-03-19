#include <iostream>
#include <timer.h>
#include <thread>
#include <future>
#include <mutex>
#include <opencv2/opencv.hpp>

/**
 * Write a program which colorizes an image with 3 horizontal stripes:
 * - red in the upper part
 * - green in the middle part
 * - blue in the bottom part
 * Each stripe must be drawn for a given number of times and each of the color must be drawn simultaneously.
 * Every next drawn stripe make the color be more visible.
 *
 * Hint:
 * Make use of the visual_multithreading.cpp example!
 *
 * @return
 */

int red_stripes = 2;
int green_stripes = 3;
int blue_stripes = 4;

void colorize_multi_thread(cv::Mat image) {

}

int main() {
    auto timer = Timer();

    auto image = cv::imread("../assets/landscape.jpg");
    colorize_multi_thread(image.clone());

    return 0;
}
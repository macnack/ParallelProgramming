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

void colorize_pixel(cv::Mat &image, unsigned char r, unsigned char g, unsigned char b, int x, int y)
{
    auto &pixel = image.at<cv::Vec3b>(x, y);

    pixel[0] = (unsigned char)(pixel[0] * 0.8f + b * 0.2f);
    pixel[1] = (unsigned char)(pixel[1] * 0.8f + g * 0.2f);
    pixel[2] = (unsigned char)(pixel[2] * 0.8f + r * 0.2f);
}
void change_pixel(cv::Mat& image, char r, char g, char b, int x, int y) {
    auto& pixel = image.at<cv::Vec3b>(x, y);
    pixel[0] = b;
    pixel[1] = g;
    pixel[2] = r;
}

void change_rows(cv::Mat &image, unsigned char r, unsigned char g, unsigned char b, int start_row, int end_row)
{
    int rows = image.rows;
    int cols = image.cols;
    int k = 0;
    for (int i = start_row; i < end_row; i++)
    {
        for (int j = 0; j < cols; j++, k++)
        {
            change_pixel(image, r, g, b, i, j);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void colorize_multi_thread(cv::Mat image)
{
    int threads = std::thread::hardware_concurrency();
    int stripes = threads / 3;
    int rows = image.rows;
    int step = rows / stripes;
    int cols = image.cols;
    // cv::namedWindow("Muklti-thread colorizing");
    std::vector<std::future<void>> futures;
    for (int i = 0; i < stripes; i++)
    {
        auto red_thread = std::async(std::launch::async, change_rows, std::ref(image), i*51.6 + 100, 0, 0, i * rows / 3 * 1 / stripes, (i + 1) * rows / 3 * 1 / stripes);
        auto green_thread = std::async(std::launch::async, change_rows, std::ref(image), 0, i*51.6 + 100, 0, rows / 3 + i * rows / 3 * 1 / stripes, rows / 3 + (i + 1) * rows / 3 * 1 / stripes);
        auto blue_thread = std::async(std::launch::async, change_rows, std::ref(image), 0, 0, i*51.6 + 100, 2 * rows / 3 + i * rows / 3 * 1 / stripes, 2 * rows / 3 + (i + 1) * rows / 3 * 1 / stripes);
        futures.push_back(std::move(red_thread));
        futures.push_back(std::move(green_thread));
        futures.push_back(std::move(blue_thread));
    }
    while (true)
    {
        bool pending_threads = 0;
        for (auto f = futures.begin(); f != futures.end(); f++)
        {
            if (f->wait_for(std::chrono::milliseconds(0)) != std::future_status::ready)
            {
                pending_threads++;
            }
        }

        if (pending_threads == 0)
        {
            break;
        }

        cv::imshow("Multi-threaded colorizing", image);
        cv::waitKey(10);
    }
    cv::imshow("Multi-threaded Final", image);
    cv::waitKey(0);
}

int main()
{
    auto timer = Timer();

    auto image = cv::imread("../assets/landscape.jpg");
    colorize_multi_thread(image.clone());

    return 0;
}
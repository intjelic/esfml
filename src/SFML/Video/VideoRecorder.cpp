////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Video/VideoRecorder.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Err.hpp>
#include <iostream>


namespace
{
    // Capture device and its counter
    cv::VideoCapture* captureDevice = NULL;
    unsigned int count = 0;
}

namespace sf
{
////////////////////////////////////////////////////////////
VideoRecorder::VideoRecorder() :
m_thread     (&VideoRecorder::record, this),
m_frame      (),
m_frameRate  (0),
m_isCapturing(false)
{
    if (!captureDevice)
    {
        // Device from which we capture images
        captureDevice = new cv::VideoCapture(0);
    }

    count++;
}


////////////////////////////////////////////////////////////
VideoRecorder::~VideoRecorder()
{
    count--;

    if (count == 0)
    {
        captureDevice->release();
        delete captureDevice;
    }
}


////////////////////////////////////////////////////////////
void VideoRecorder::start(unsigned int frameRate)
{
    // Check if the device can do video capture
    if (!isAvailable())
    {
        err() << "Failed to start capture : your system cannot capture video data (call VideoRecorder::isAvailable to check it)" << std::endl;
        return;
    }

    // Clear the frame buffer
    Vector2u size;
    m_frame.create(size.x, size.y, Color::Black);

    // Store the frame rate
    m_frameRate = frameRate;

    // Notify derived class
    if (onStart())
    {
        // Start the capture in a new thread, to avoid blocking the main thread
        m_isCapturing = true;
        m_thread.launch();
    }
}


////////////////////////////////////////////////////////////
void VideoRecorder::stop()
{
    // Stop the capturing thread
    m_isCapturing = false;
    m_thread.wait();
}


////////////////////////////////////////////////////////////
unsigned int VideoRecorder::getFrameRate() const
{
    return m_frameRate;
}


////////////////////////////////////////////////////////////
Vector2u VideoRecorder::getVideoSize() const
{
    double width = captureDevice->get(CV_CAP_PROP_FRAME_WIDTH);
    double height = captureDevice->get(CV_CAP_PROP_FRAME_HEIGHT);

    return Vector2u(width, height);
}


////////////////////////////////////////////////////////////
bool VideoRecorder::isAvailable()
{
    // Attempt to open a temporary device to know if it's available.
    if (!captureDevice)
    {
        bool isAvailable = false;

        captureDevice = new cv::VideoCapture(0);
        isAvailable = captureDevice->isOpened();
        captureDevice->release();
        delete captureDevice;

        return isAvailable;
    }

    // Or simply check the availability with the current one.
    return captureDevice->isOpened();
}


////////////////////////////////////////////////////////////
bool VideoRecorder::onStart()
{
    // Nothing to do
    return true;
}


////////////////////////////////////////////////////////////
void VideoRecorder::onStop()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
void VideoRecorder::record()
{
    // TODO: the following assumes capturing an image takes no time, which is
    // wrong.

    // Compute how long this thread must sleep to match the frame rate.
    Time time = milliseconds(1000/m_frameRate);

    while (m_isCapturing)
    {
        // Capture the next frame
        captureNextFrame();

        // Don't bother the CPU while waiting for more captured data
        sleep(time);
    }

    // Notify derived class
    onStop();
}


////////////////////////////////////////////////////////////
void VideoRecorder::captureNextFrame()
{
    // Grab the next frame
    cv::Mat frame;
    captureDevice->read(frame);

    // Convert to RGBA
    Uint8* data = new Uint8[frame.total()*4];
    cv::Mat continuous_data(frame.size(), CV_8UC4, data);
    cv::cvtColor(frame, continuous_data, CV_BGR2RGBA, 4);

    // Insert it into our buffer
    m_frame.create(frame.cols, frame.rows, data);

    // Forward them to the derived class
    if (!onProcessNextFrame(m_frame))
    {
        // The user wants to stop the capture
        m_isCapturing = false;
    }
}

} // namespace sf

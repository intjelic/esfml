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
#include <SFML/Video/VideoStream.hpp>

namespace sf
{
////////////////////////////////////////////////////////////
VideoStream::VideoStream() :
m_thread         (&VideoStream::streamData, this),
m_isStreaming    (false),
m_size           (0, 0),
m_framePerSecond (0),
m_loop           (false),
m_framesProcessed(0)
{

}


////////////////////////////////////////////////////////////
VideoStream::~VideoStream()
{
    // Stop the video if it was playing
    stop();
}


////////////////////////////////////////////////////////////
void VideoStream::initialize(const Vector2i& size, unsigned int framePerSecond)
{
    m_size = size;
    m_framePerSecond = framePerSecond;
}


////////////////////////////////////////////////////////////
void VideoStream::play()
{
    // If the video is already playing (probably paused), just resume it
    if (m_isStreaming)
    {
        m_player->play();
        return;
    }

    // Move to the beginning
    onSeek(Time::Zero);

    // Start updating the stream in a separate thread to avoid blocking the application
    m_framesProcessed = 0;
    m_isStreaming = true;
    m_thread.launch();
}


////////////////////////////////////////////////////////////
void VideoStream::pause()
{
    m_player->pause();
}


////////////////////////////////////////////////////////////
void VideoStream::stop()
{
    // Wait for the thread to terminate
    m_isStreaming = false;
    m_thread.wait();
}


////////////////////////////////////////////////////////////
Vector2i VideoStream::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
unsigned int VideoStream::getFramePerSecond() const
{
    return m_framePerSecond;
}


////////////////////////////////////////////////////////////
VideoSource::Status VideoStream::getStatus() const
{
    Status status = VideoSource::getStatus();

    // To compensate for the lag between play() and m_player->play()
    if ((status == Stopped) && m_isStreaming)
        status = Playing;

    return status;
}


////////////////////////////////////////////////////////////
void VideoStream::setPlayingOffset(Time timeOffset)
{
    // Stop the stream
    stop();

    // Let the derived class update the current position
    onSeek(timeOffset);

    // Restart streaming
    m_framesProcessed = static_cast<Uint64>(timeOffset.asSeconds() * m_framePerSecond);
    m_isStreaming = true;
    m_thread.launch();
}


////////////////////////////////////////////////////////////
Time VideoStream::getPlayingOffset() const
{
    if (m_framePerSecond && m_size.x > 0 && m_size.y > 0)
    {
        return seconds(static_cast<float>(m_framesProcessed) / m_framePerSecond);
    }
    else
    {
        return Time::Zero;
    }

    return Time::Zero;
}


////////////////////////////////////////////////////////////
void VideoStream::setLoop(bool loop)
{
    m_loop = loop;
}


////////////////////////////////////////////////////////////
bool VideoStream::getLoop() const
{
    return m_loop;
}


////////////////////////////////////////////////////////////
void VideoStream::streamData()
{
    // Initialize the buffers
    for (int i = 0; i < BufferCount; ++i)
        m_endBuffers[i] = false;

    // Fill the queue
    bool requestStop = fillQueue();

    // Play the sound
    m_player->play();

    while (m_isStreaming)
    {
        // The stream has been interrupted!
        if (VideoSource::getStatus() == Stopped)
        {
            if (!requestStop)
            {
                // Just continue
                m_player->play();
            }
            else
            {
                // End streaming
                m_isStreaming = false;
            }
        }

        // Get the number of buffers that have been processed (ie. ready for reuse)
        unsigned int nbProcessed = 0;
        nbProcessed = m_player->getUsedBufferCount();

        while (nbProcessed--)
        {
            // Pop the first unused buffer from the queue
            const VideoBuffer* buffer = m_player->unqueueBuffer();

            // Find its number
            unsigned int bufferNum = 0;
            for (int i = 0; i < BufferCount; ++i)
                if (&m_buffers[i] == buffer)
                {
                    bufferNum = i;
                    break;
                }

            // Retrieve its size and add it to the frame count
            if (m_endBuffers[bufferNum])
            {
                // This was the last buffer: reset the frame count
                m_framesProcessed = 0;
                m_endBuffers[bufferNum] = false;
            }
            else
            {
                m_framesProcessed += buffer->getFrameCount();
            }

            // Fill it and push it back into the playing queue
            if (!requestStop)
            {
                if (fillAndPushBuffer(bufferNum))
                    requestStop = true;
            }
        }

        // Leave some time for the other threads if the stream is still playing
        if (VideoSource::getStatus() != Stopped)
            sleep(milliseconds(10));
    }

    //// Stop the playback
    m_player->stop();

    // Clear the remaining buffers
    m_player->resetBuffers();
}


////////////////////////////////////////////////////////////
bool VideoStream::fillAndPushBuffer(unsigned int bufferNum)
{
    bool requestStop = false;

    // Acquire video data
    std::vector<Image> data;

    if (!onGetData(data))
    {
        // Mark the buffer as the last one (so that we know when to reset the playing position)
        m_endBuffers[bufferNum] = true;

        // Check if the stream must loop or stop
        if (m_loop)
        {
            // Return to the beginning of the stream source
            onSeek(Time::Zero);

            // If we previously had no data, try to fill the buffer once again
            if (!data.size())
            {
                return fillAndPushBuffer(bufferNum);
            }
        }
        else
        {
            // Not looping: request stop
            requestStop = true;
        }
    }

    // Fill the buffer if some data was returned
    if (data.size())
    {
        VideoBuffer& buffer = m_buffers[bufferNum];

        // Fill the buffer
        Vector2u size;
        size.x = m_size.x;
        size.y = m_size.y;

        buffer.loadFromFrames(&data[0], data.size(), size, m_framePerSecond);

        // Push it into the video queue
        m_player->queueBuffer(buffer);
    }

    return requestStop;
}


////////////////////////////////////////////////////////////
bool VideoStream::fillQueue()
{
    // Fill and enqueue all the available buffers
    bool requestStop = false;
    for (int i = 0; (i < BufferCount) && !requestStop; ++i)
    {
        if (fillAndPushBuffer(i))
            requestStop = true;
    }

    return requestStop;
}

} // namespace sf

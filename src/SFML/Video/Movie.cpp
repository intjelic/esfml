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
#include <SFML/Video/Movie.hpp>
#include <SFML/Video/VideoFile.hpp>
#include <SFML/System/Lock.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
Movie::Movie() :
m_file     (new priv::VideoFile),
m_duration ()
{
}


////////////////////////////////////////////////////////////
Movie::~Movie()
{
}


////////////////////////////////////////////////////////////
bool Movie::openFromFile(const std::string& filename)
{
    // First stop the movie if it was already running
    stop();

    // Open the underlying video file
    if (!m_file->openRead(filename))
        return false;

    // Perform common initializations
    initialize();

    return true;
}


////////////////////////////////////////////////////////////
bool Movie::openFromMemory(const void* data, std::size_t sizeInBytes)
{
    // First stop the movie if it was already running
    stop();

    // Open the underlying video file
    if (!m_file->openRead(data, sizeInBytes))
        return false;

    // Perform common initializations
    initialize();

    return true;
}


////////////////////////////////////////////////////////////
bool Movie::openFromStream(InputStream& stream)
{
    // First stop the movie if it was already running
    stop();

    // Open the underlying video file
    if (!m_file->openRead(stream))
        return false;

    // Perform common initializations
    initialize();

    return true;
}


////////////////////////////////////////////////////////////
Time Movie::getDuration() const
{
    return Time::Zero;
}


////////////////////////////////////////////////////////////
bool Movie::onGetData(std::vector<Image>& data)
{
    Lock lock(m_mutex);

    // Fill the chunk parameters
    unsigned int frameCount = m_file->read(data, m_file->getFramePerSecond());

    // Check if we have reached the end of the video file
    return frameCount == m_frames.size();
}


////////////////////////////////////////////////////////////
void Movie::onSeek(Time timeOffset)
{
    Lock lock(m_mutex);

    m_file->seek(timeOffset);
}


////////////////////////////////////////////////////////////
void Movie::initialize()
{
    // Compute the movie duration
    m_duration = seconds(m_file->getFrameCount() / m_file->getFramePerSecond());

    // Resize the internal buffer so that it can contain 1 second of video
    m_frames.resize(m_file->getFramePerSecond());

    // Initialize the stream
    VideoStream::initialize(m_file->getSize(), m_file->getFramePerSecond());
}

} // namespace sf

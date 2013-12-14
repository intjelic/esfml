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
#include <SFML/Video/VideoBuffer.hpp>
#include <SFML/Video/VideoFile.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
VideoBuffer::VideoBuffer() :
m_frames         (),
m_frameCount     (0),
m_framePerSecond (0),
m_duration       (),
m_videos         ()
{
	av_register_all();
}


////////////////////////////////////////////////////////////
VideoBuffer::VideoBuffer(const VideoBuffer& copy) :
m_frames         (),
m_frameCount     (0),
m_framePerSecond (0),
m_duration       (),
m_videos         ()
{
}


////////////////////////////////////////////////////////////
VideoBuffer::~VideoBuffer()
{
}


////////////////////////////////////////////////////////////
bool VideoBuffer::loadFromFile(const std::string& filename)
{
    priv::VideoFile file;

    if (!file.openRead(filename))
		return false;

	m_frameCount = file.getFrameCount();
	m_framePerSecond = file.getFramePerSecond();

	// Prepare our array to receive this amount of video frame
	m_frames.clear();
	m_frames.reserve(m_frameCount);

	// Read the frames from the provided file
	return file.read(m_frames, m_frameCount) == m_frameCount;
}


////////////////////////////////////////////////////////////
bool VideoBuffer::loadFromMemory(const void* data, std::size_t sizeInBytes)
{
	return false;
}


////////////////////////////////////////////////////////////
bool VideoBuffer::loadFromStream(InputStream& stream)
{
	return false;
}


////////////////////////////////////////////////////////////
bool VideoBuffer::loadFromFrames(const Image* frames, std::size_t frameCount, const Vector2u& size, unsigned int framePerSecond)
{
    // Copy the new video frames
    m_frames.assign(frames, frames + frameCount);

    // Copy the video properties
    m_frameCount = frameCount;
    m_framePerSecond = framePerSecond;
    m_duration = seconds(frameCount / framePerSecond);

    return true;
}


////////////////////////////////////////////////////////////
bool VideoBuffer::saveToFile(const std::string& filename) const
{
	return false;
}


////////////////////////////////////////////////////////////
const Image* VideoBuffer::getFrames() const
{
	return m_frames.empty() ? NULL : &m_frames[0];
}


////////////////////////////////////////////////////////////
std::size_t VideoBuffer::getFrameCount() const
{
	return m_frames.size();
}


////////////////////////////////////////////////////////////
unsigned int VideoBuffer::getFramePerSecond() const
{
	return m_framePerSecond;
}


////////////////////////////////////////////////////////////
Vector2i VideoBuffer::getSize() const
{
	return Vector2i(0, 0);
}


////////////////////////////////////////////////////////////
Time VideoBuffer::getDuration() const
{
    return seconds(static_cast<float>(getFrameCount()) / static_cast<float>(getFramePerSecond()));
}


////////////////////////////////////////////////////////////
VideoBuffer& VideoBuffer::operator =(const VideoBuffer& right)
{
}


////////////////////////////////////////////////////////////
void VideoBuffer::attachVideo(Video* video) const
{
    m_videos.insert(video);
}


////////////////////////////////////////////////////////////
void VideoBuffer::detachVideo(Video* video) const
{
    m_videos.erase(video);
}

} // namespace sf

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


namespace sf
{
////////////////////////////////////////////////////////////
VideoBuffer::VideoBuffer() :
m_frames  (),
m_duration(),
m_videos  ()
{
}


////////////////////////////////////////////////////////////
VideoBuffer::VideoBuffer(const VideoBuffer& copy) :
m_frames (),
m_duration(),
m_videos  ()
{
}


////////////////////////////////////////////////////////////
VideoBuffer::~VideoBuffer()
{
}


////////////////////////////////////////////////////////////
bool VideoBuffer::loadFromFile(const std::string& filename)
{
	return false;
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
bool VideoBuffer::loadFromSamples(const Image* frames, std::size_t frameCount, const Vector2i& size, unsigned int framePerSecond)
{
	return false;
}


////////////////////////////////////////////////////////////
bool VideoBuffer::saveToFile(const std::string& filename) const
{
	return false;
}


////////////////////////////////////////////////////////////
const Image* VideoBuffer::getFrames() const
{
	return NULL;
}


////////////////////////////////////////////////////////////
std::size_t VideoBuffer::getFrameCount() const
{
	return 0;
}


////////////////////////////////////////////////////////////
unsigned int VideoBuffer::getFramePerSecond() const
{
	return 0;
}


////////////////////////////////////////////////////////////
Vector2i VideoBuffer::getSize() const
{
	return Vector2i(0, 0);
}


////////////////////////////////////////////////////////////
Time VideoBuffer::getDuration() const
{
	return Time::Zero;
}


////////////////////////////////////////////////////////////
VideoBuffer& VideoBuffer::operator =(const VideoBuffer& right)
{
}


////////////////////////////////////////////////////////////
bool VideoBuffer::initialize(priv::VideoFile& file)
{
	return false;
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

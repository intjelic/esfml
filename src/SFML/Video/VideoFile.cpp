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
#include <SFML/Video/VideoFile.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
VideoFile::VideoFile() :
m_file           (NULL),
m_frameCount     (0),
m_size           (0, 0),
m_framePerSecond (0)
{

}

////////////////////////////////////////////////////////////
VideoFile::~VideoFile()
{
}

////////////////////////////////////////////////////////////
std::size_t VideoFile::getFrameCount() const
{
    return m_frameCount;
}


////////////////////////////////////////////////////////////
const Vector2i& VideoFile::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
unsigned int VideoFile::getFramePerSecond() const
{
    return m_framePerSecond;
}


////////////////////////////////////////////////////////////
bool VideoFile::openRead(const std::string& filename)
{
	return false;
}


////////////////////////////////////////////////////////////
bool VideoFile::openRead(const void* data, std::size_t sizeInBytes)
{
	return false;
}


////////////////////////////////////////////////////////////
bool VideoFile::openRead(InputStream& stream)
{
	return false;
}


////////////////////////////////////////////////////////////
bool VideoFile::openWrite(const std::string& filename, unsigned int channelCount, unsigned int sampleRate)
{
	return false;
}


////////////////////////////////////////////////////////////
std::size_t VideoFile::read(const Texture* data, std::size_t frameCount)
{
	return 0;
}


////////////////////////////////////////////////////////////
void VideoFile::write(const Texture* data, std::size_t frameCount)
{
}


////////////////////////////////////////////////////////////
void VideoFile::seek(Time timeOffset)
{
}

} // namespace priv

} // namespace sf

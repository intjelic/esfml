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
m_loop           (false)
{

}


////////////////////////////////////////////////////////////
VideoStream::~VideoStream()
{
}


////////////////////////////////////////////////////////////
void VideoStream::initialize(const Vector2i& size, unsigned int framePerSecond)
{
}


////////////////////////////////////////////////////////////
void VideoStream::play()
{
}


////////////////////////////////////////////////////////////
void VideoStream::pause()
{
}


////////////////////////////////////////////////////////////
void VideoStream::stop()
{
}

////////////////////////////////////////////////////////////
VideoStream::Status VideoStream::getStatus() const
{
}


////////////////////////////////////////////////////////////
void VideoStream::setPlayingOffset(Time timeOffset)
{
}


////////////////////////////////////////////////////////////
Time VideoStream::getPlayingOffset() const
{
}


////////////////////////////////////////////////////////////
void VideoStream::setLoop(bool loop)
{
}


////////////////////////////////////////////////////////////
bool VideoStream::getLoop() const
{
}


////////////////////////////////////////////////////////////
void VideoStream::streamData()
{
}

} // namespace sf

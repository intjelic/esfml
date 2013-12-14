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
#include <SFML/Video/Video.hpp>
#include <SFML/Video/VideoBuffer.hpp>
#include <SFML/Video/VideoPlayer.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
Video::Video() :
m_buffer(NULL)
{
}


////////////////////////////////////////////////////////////
Video::Video(const VideoBuffer& buffer) :
m_buffer(NULL)
{
    setBuffer(buffer);
}


////////////////////////////////////////////////////////////
Video::Video(const Video& copy) :
VideoSource(copy),
m_buffer   (NULL)
{
    if (copy.m_buffer)
        setBuffer(*copy.m_buffer);

    setLoop(copy.getLoop());
}


////////////////////////////////////////////////////////////
Video::~Video()
{
    stop();

    if (m_buffer)
        m_buffer->detachVideo(this);
}


////////////////////////////////////////////////////////////
void Video::play()
{
    m_player->play();
}


////////////////////////////////////////////////////////////
void Video::pause()
{
    m_player->pause();
}


////////////////////////////////////////////////////////////
void Video::stop()
{
    m_player->stop();
}


////////////////////////////////////////////////////////////
void Video::setBuffer(const VideoBuffer& buffer)
{
    // First detach from the previous buffer
    if (m_buffer)
    {
        stop();
        m_buffer->detachVideo(this);
    }

    // Assign and use the new buffer
    m_buffer = &buffer;
    m_buffer->attachVideo(this);
    m_player->setBuffer(*m_buffer);
}


////////////////////////////////////////////////////////////
void Video::setLoop(bool loop)
{
    m_player->setLoop(loop);
}


////////////////////////////////////////////////////////////
void Video::setPlayingOffset(Time timeOffset)
{
    m_player->setPlayingOffset(timeOffset);
}


////////////////////////////////////////////////////////////
const VideoBuffer* Video::getBuffer() const
{
    return m_buffer;
}


////////////////////////////////////////////////////////////
bool Video::getLoop() const
{
    return m_player->getLoop();
}


////////////////////////////////////////////////////////////
Time Video::getPlayingOffset() const
{
    return m_player->getPlayingOffset();
}


////////////////////////////////////////////////////////////
Video::Status Video::getStatus() const
{
    return VideoSource::getStatus();
}


////////////////////////////////////////////////////////////
Video& Video::operator =(const Video& right)
{
    // Here we don't use the copy-and-swap idiom, because it would mess up
    // the list of frames instances contained in the buffers

    // Detach the video instance from the previous buffer (if any)
    if (m_buffer)
    {
        stop();
        m_buffer->detachVideo(this);
        m_buffer = NULL;
    }

    // Copy the video attributes
    if (right.m_buffer)
        setBuffer(*right.m_buffer);
    setLoop(right.getLoop());

    return *this;
}


////////////////////////////////////////////////////////////
void Video::resetBuffer()
{
    // First stop the video in case it is playing
    stop();

    // Detach the buffer
    m_player->resetBuffer();
    m_buffer = NULL;
}

} // namespace sf

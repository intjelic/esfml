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
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


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
	// Update the video status and restart the clock
	if (VideoSource::getStatus() != Playing)
	{
		setStatus(Playing);
		m_clock.restart();
	}
}


////////////////////////////////////////////////////////////
void Video::pause()
{
	// Update the time buffer and the video status
	if (VideoSource::getStatus() == Playing)
	{
		setStatus(Paused);
		m_timeBuffer += m_clock.getElapsedTime();
	}
}


////////////////////////////////////////////////////////////
void Video::stop()
{
	// Reset the time buffer to zero and update the video status
	if (VideoSource::getStatus() != Stopped)
	{
		setStatus(Stopped);
		m_timeBuffer = Time::Zero;
	}
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
}


////////////////////////////////////////////////////////////
void Video::setLoop(bool loop)
{
	m_loop = loop;
}


////////////////////////////////////////////////////////////
void Video::setPlayingOffset(Time timeOffset)
{
	m_timeBuffer = timeOffset;
	m_clock.restart();
}


////////////////////////////////////////////////////////////
const VideoBuffer* Video::getBuffer() const
{
    return m_buffer;
}


////////////////////////////////////////////////////////////
bool Video::getLoop() const
{
	return m_loop;
}


////////////////////////////////////////////////////////////
Time Video::getPlayingOffset() const
{
	// If playing, the playing offset is the result of the time buffer plus the
	// elapsed time since the last time the time buffer was updated.
	if (VideoSource::getStatus() == Playing)
		return m_timeBuffer + m_clock.getElapsedTime();
	else
		return m_timeBuffer;
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
    m_buffer = NULL;
}


////////////////////////////////////////////////////////////
void Video::update()
{
	// Retrieve FPS
	unsigned int fps = m_buffer->getFramePerSecond();

	// Retrieve the playing offset
	Time playingOffset;

	if (VideoSource::getStatus() == Playing)
		playingOffset = m_timeBuffer + m_clock.getElapsedTime();
	else
		playingOffset = m_timeBuffer;

	// Compute the frame index
	unsigned int index = playingOffset.asSeconds() * fps;

	// Update the current image
	m_texture.loadFromImage(m_buffer->getFrames()[index]);
}


////////////////////////////////////////////////////////////
void Video::draw(RenderTarget& target, RenderStates states) const
{
	// Apply the video render states
	states.transform *= getTransform();

	// Draw the current frame
	target.draw(Sprite(m_texture), states);
}

} // namespace sf

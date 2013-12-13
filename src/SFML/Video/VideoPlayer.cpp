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
#include <SFML/Video/VideoPlayer.hpp>
#include <SFML/System/Sleep.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
VideoPlayer::VideoPlayer() :
m_thread    (&VideoPlayer::update, this),
m_isPlaying (false),
m_isPaused  (false),
m_buffer    (NULL),
m_loop      (false)
{
}

////////////////////////////////////////////////////////////
void VideoPlayer::play()
{
    // Ignore if already playing
    if (m_isPlaying)
        return;

	// Play only if some frames are available
    if (m_buffer)
    {
		// Restart the clock
		m_clock.restart();

		// Start updating the video in a separate thread
		m_isPlaying = true;
		m_thread.launch();

		// Update the status
		m_isPaused = false;
	}
}

////////////////////////////////////////////////////////////
void VideoPlayer::pause()
{
    // Ignore if already paused
    if (m_isPaused)
        return;

	if (m_isPlaying)
	{
		// Stop the background tasks
		m_isPlaying = false;
		m_thread.wait();

		// Update the time buffer and the status
		m_timeBuffer += m_clock.getElapsedTime();
		m_isPaused = true;
	}
}

////////////////////////////////////////////////////////////
void VideoPlayer::stop()
{
	// Stop the background tasks
	pause();

	// Reset the time buffer to zero and update the status
	m_timeBuffer = Time::Zero;
	m_isPaused = false;
}

////////////////////////////////////////////////////////////
unsigned int VideoPlayer::getStatus() const
{
	// Compute the status
	if (m_isPaused)
		return PAUSED;
	else
		return m_isPlaying ? PLAYING : STOPPED;
}

////////////////////////////////////////////////////////////
void VideoPlayer::setPlayingOffset(Time timeOffset)
{
	m_timeBuffer = timeOffset;
	m_clock.restart();
}

////////////////////////////////////////////////////////////
Time VideoPlayer::getPlayingOffset() const
{
	// If playing, the playing offset is the result of the time buffer plus the
	// elapsed time since the last time the buffer was updated.
	if (m_isPlaying)
		return m_timeBuffer + m_clock.getElapsedTime();
	else
		return m_timeBuffer;
}

////////////////////////////////////////////////////////////
void VideoPlayer::setLoop(bool loop)
{
	m_loop = loop;
}

////////////////////////////////////////////////////////////
bool VideoPlayer::getLoop() const
{
	return m_loop;
}

////////////////////////////////////////////////////////////
void VideoPlayer::setBuffer(const VideoBuffer& buffer)
{
	resetBuffer();
	m_buffer = &buffer;
}

////////////////////////////////////////////////////////////
void VideoPlayer::resetBuffer()
{
	stop();
	m_buffer = NULL;
}

////////////////////////////////////////////////////////////
void VideoPlayer::update()
{
    while (m_isPlaying)
    {
		// Compute the new frame index and update the internal texture
		computeNewFrame();

        // Leave some time for the other threads if the video is playing
		sleep(milliseconds(10));
	}
}

////////////////////////////////////////////////////////////
void VideoPlayer::computeNewFrame()
{
	// Retrieve FPS
	unsigned int fps = m_buffer->getFramePerSecond();

	// Compute the playing offset
	Time playingOffset;
	playingOffset = m_timeBuffer + m_clock.getElapsedTime();

	// Retrieve duration
	Time duration = m_buffer->getDuration();

	if (playingOffset >= duration)
	{
		if (m_loop)
		{
			m_timeBuffer = playingOffset - duration;
			m_clock.restart();

			computeNewFrame();
		}
		else
		{
			m_isPlaying = false;
			m_timeBuffer = Time::Zero;
		}

		return;
	}

	// Compute the frame index
	unsigned int index = playingOffset.asSeconds() * fps;

	// Update the current image
	m_currentFrame.loadFromImage(m_buffer->getFrames()[index]);
}

////////////////////////////////////////////////////////////
const Texture& VideoPlayer::getCurrentFrame() const
{
	return m_currentFrame;
}

} // namepsace priv
} // namepsace sf

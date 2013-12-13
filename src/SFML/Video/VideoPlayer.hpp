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

#ifndef SFML_VIDEOPLAYER_HPP
#define SFML_VIDEOPLAYER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Video/VideoBuffer.hpp>
#include <SFML/Video/VideoSource.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>


namespace sf
{
namespace priv
{
class VideoPlayer
{
public :

    VideoPlayer();

    void play();
    void pause();
    void stop();

    VideoSource::Status getStatus() const;

    void setPlayingOffset(Time timeOffset);
    Time getPlayingOffset() const;

    void setLoop(bool loop);
    bool getLoop() const;

	void setBuffer(const VideoBuffer&);
	void resetBuffer();

	const Texture& getCurrentFrame() const;

private :

    ////////////////////////////////////////////////////////////
    /// \brief Update the video player every 10 milliseconds
    ///
    /// This function will compute the new frame very 10 milliseconds,
    /// a short delay to avoid consuming too much CPU.
    ///
    ////////////////////////////////////////////////////////////
    void update();

    ////////////////////////////////////////////////////////////
    /// \brief Compute the new frame according to the elapsed time
    ///
    /// This function performs the computation of the frame itself and
    /// updates the internal data.
    ///
    ////////////////////////////////////////////////////////////
    void computeNewFrame();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Thread             m_thread;       ///< Thread computing frame indexes
    bool               m_isPlaying;    ///< Play state (true = playing, false = stopped || paused)
    bool               m_isPaused;     ///< Pause state (true = paused, false = playing || stopped)
    const VideoBuffer* m_buffer;       ///< The buffer containing the frames to play
    Clock              m_clock;        ///< Clock measuring elapsed time since the play()
    Time               m_timeBuffer;   ///< Time acting as buffer when the video is stopped
    bool               m_loop;         ///< Loop flag (true to loop, false to play once)
    Texture            m_currentFrame; ///< The internal texture representing the current video frame
};

} // namespace priv
} // namespace sf


#endif // SFML_VIDEOPLAYER_HPP

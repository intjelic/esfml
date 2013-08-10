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

#ifndef SFML_VIDEO_HPP
#define SFML_VIDEO_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Video/Export.hpp>
#include <SFML/Video/VideoSource.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>


namespace sf
{
class VideoBuffer;

////////////////////////////////////////////////////////////
/// \brief Regular video that can be played in the video environment
///
////////////////////////////////////////////////////////////
class SFML_VIDEO_API Video : public VideoSource
{
public :

	////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
	////////////////////////////////////////////////////////////
    Video();

	////////////////////////////////////////////////////////////
    /// \brief Construct the video with a buffer
    ///
    /// \param buffer Video buffer containing the video data to play with the video
    ///
	////////////////////////////////////////////////////////////
    explicit Video(const VideoBuffer& buffer);

	////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy Instance to copy
    ///
	////////////////////////////////////////////////////////////
    Video(const Video& copy);

	////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
	////////////////////////////////////////////////////////////
    ~Video();

	////////////////////////////////////////////////////////////
    /// \brief Start or resume playing the video
    ///
    /// This function starts the video if it was stopped, resumes
    /// it if it was paused, and restarts it from beginning if it
    /// was it already playing.
    ///
    /// \see pause, stop
    ///
	////////////////////////////////////////////////////////////
    void play();

	////////////////////////////////////////////////////////////
    /// \brief Pause the video
    ///
    /// This function pauses the video if it was playing,
    /// otherwise (video already paused or stopped) it has no effect.
    ///
    /// \see play, stop
    ///
	////////////////////////////////////////////////////////////
    void pause();

	////////////////////////////////////////////////////////////
    /// \brief stop playing the video
    ///
    /// This function stops the video if it was playing or paused,
    /// and does nothing if it was already stopped.
    /// It also resets the playing position (unlike pause()).
    ///
    /// \see play, pause
    ///
	////////////////////////////////////////////////////////////
    void stop();

	////////////////////////////////////////////////////////////
    /// \brief Set the source buffer containing the video data to play
    ///
    /// It is important to note that the video buffer is not copied,
    /// thus the sf::VideoBuffer instance must remain alive as long
    /// as it is attached to the video.
    ///
    /// \param buffer Video buffer to attach to the video
    ///
    /// \see getBuffer
    ///
	////////////////////////////////////////////////////////////
    void setBuffer(const VideoBuffer& buffer);

	////////////////////////////////////////////////////////////
    /// \brief Set whether or not the video should loop after reaching the end
    ///
    /// If set, the video will restart from beginning after
    /// reaching the end and so on, until it is stopped or
    /// setLoop(false) is called.
    /// The default looping state for video is false.
    ///
    /// \param loop True to play in loop, false to play once
    ///
    /// \see getLoop
    ///
	////////////////////////////////////////////////////////////
    void setLoop(bool loop);

	////////////////////////////////////////////////////////////
    /// \brief Change the current playing position of the video
    ///
    /// The playing position can be changed when the video is
    /// either paused or playing.
    ///
    /// \param timeOffset New playing position, from the beginning of the video
    ///
    /// \see getPlayingOffset
    ///
	////////////////////////////////////////////////////////////
    void setPlayingOffset(Time timeOffset);

	////////////////////////////////////////////////////////////
    /// \brief Get the video buffer attached to the video
    ///
    /// \return Video buffer attached to the video (can be NULL)
    ///
	////////////////////////////////////////////////////////////
    const VideoBuffer* getBuffer() const;

	////////////////////////////////////////////////////////////
    /// \brief Tell whether or not the video is in loop mode
    ///
    /// \return True if the sound is looping, false otherwise
    ///
    /// \see setLoop
    ///
	////////////////////////////////////////////////////////////
    bool getLoop() const;

	////////////////////////////////////////////////////////////
    /// \brief Get the current playing position of the video
    ///
    /// \return Current playing position, from the beginning of the video
    ///
    /// \see setPlayingOffset
    ///
	////////////////////////////////////////////////////////////
    Time getPlayingOffset() const;

	////////////////////////////////////////////////////////////
    /// \brief Get the current status of the video (stopped, paused, playing)
    ///
    /// \return Current status of the video
    ///
	////////////////////////////////////////////////////////////
    Status getStatus() const;

	////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
	////////////////////////////////////////////////////////////
    Video& operator =(const Video& right);

	////////////////////////////////////////////////////////////
    /// \brief Reset the internal buffer of the video
    ///
    /// This function is for internal use only, you don't have
    /// to use it. It is called by the sf::VideoBuffer that
    /// this video uses, when it is destroyed in order to prevent
    /// the video from using a dead buffer.
    ///
	////////////////////////////////////////////////////////////
    void resetBuffer();

	////////////////////////////////////////////////////////////
    /// \brief Update the current frame
    ///
	////////////////////////////////////////////////////////////
    void update();

private :

	////////////////////////////////////////////////////////////
    /// \brief Draw the video to a render target.
    ///
	////////////////////////////////////////////////////////////
    virtual void draw(RenderTarget& target, RenderStates states) const;

	////////////////////////////////////////////////////////////
    // Member data
	////////////////////////////////////////////////////////////
    const VideoBuffer* m_buffer;      ///< Video buffer bound to the source
    Texture            m_texture;     ///< Texture containing the current frame image
    Clock              m_clock;       ///< Clock measuring elapsed time since the play()
    Time               m_timeBuffer;  ///< Time acting as buffer when the video is stoppe
    bool               m_loop;        ///< Tell whether the video should loop after reaching the end
};

} // namespace sf

#endif // SFML_VIDEO_HPP

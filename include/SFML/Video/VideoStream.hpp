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

#ifndef SFML_VIDEOSTREAM_HPP
#define SFML_VIDEOSTREAM_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Video/Export.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>


namespace sf
{
class SFML_VIDEO_API VideoStream
{
public :

    enum Status
    {
        Stopped, ///< Video is not playing
        Paused,  ///< Video is paused
        Playing  ///< Video is playing
    };

    struct Chunk
    {
        const Image* frames;     ///< Pointer to the video frames
        std::size_t  frameCount; ///< Number of frames pointed by Samples
    };

    virtual ~VideoStream();

    void play();
    void pause();
    void stop();

    Status getStatus() const;
    void setPlayingOffset(Time timeOffset);
    Time getPlayingOffset() const;

    void setLoop(bool loop);
    bool getLoop() const;

protected :

    VideoStream();

    void initialize(const Vector2i& size, unsigned int framePerSecond);
    virtual bool onGetData(Chunk& data) = 0;
    virtual void onSeek(Time timeOffset) = 0;

private :

    void streamData();

    Thread        m_thread;         ///< Thread running the background tasks
    bool          m_isStreaming;    ///< Streaming state (true = playing, false = stopped)
    Image         m_buffers[10];    ///< Sound buffers used to store temporary audio data
    Vector2i      m_size;           ///< Number of channels (1 = mono, 2 = stereo, ...)
    unsigned int  m_framePerSecond; ///< Frequency (frames / second)
    bool          m_loop;           ///< Loop flag (true to loop, false to play once)

};

} // namespace sf


#endif // SFML_VIDEOSTREAM_HPP

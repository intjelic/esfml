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
#include <SFML/Video/VideoSource.hpp>
#include <SFML/Video/VideoBuffer.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>


namespace sf
{
class SFML_VIDEO_API VideoStream : public VideoSource
{
public :

    virtual ~VideoStream();

    void play();
    void pause();
    void stop();

    Vector2i getSize() const;
    unsigned int getFramePerSecond() const;

    Status getStatus() const;
    void setPlayingOffset(Time timeOffset);
    Time getPlayingOffset() const;

    void setLoop(bool loop);
    bool getLoop() const;

protected :

    VideoStream();

    void initialize(const Vector2i& size, unsigned int framePerSecond);
    virtual bool onGetData(std::vector<Image>& data) = 0;
    virtual void onSeek(Time timeOffset) = 0;

private :

    bool fillAndPushBuffer(unsigned int bufferNum);
    bool fillQueue();

    void streamData();

    enum
    {
        BufferCount = 3 ///< Number of video buffers used by the streaming loop
    };

    Thread        m_thread;                  ///< Thread running the background tasks
    bool          m_isStreaming;             ///< Streaming state (true = playing, false = stopped)
    VideoBuffer   m_buffers[BufferCount];    ///< Video buffers used to store temporary video data
    Vector2i      m_size;                    ///< Size of the video
    unsigned int  m_framePerSecond;          ///< Number of frame played per second
    bool          m_loop;                    ///< Loop flag (true to loop, false to play once)
    Uint64        m_framesProcessed;         ///< Number of frames processed since beginning of the stream
    bool          m_endBuffers[BufferCount]; ///< Each buffer is marked as "end buffer" or not, for proper duration calculation

};

} // namespace sf


#endif // SFML_VIDEOSTREAM_HPP

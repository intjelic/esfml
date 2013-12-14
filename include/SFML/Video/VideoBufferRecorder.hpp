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

#ifndef SFML_VIDEOBUFFERRECORDER_HPP
#define SFML_VIDEOBUFFERRECORDER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Video/Export.hpp>
#include <SFML/Video/VideoBuffer.hpp>
#include <SFML/Video/VideoRecorder.hpp>
#include <vector>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Specialized VideoRecorder which stores the captured
///        video data into a video buffer
///
////////////////////////////////////////////////////////////
class SFML_VIDEO_API VideoBufferRecorder : public VideoRecorder
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Get the video buffer containing the captured video data
    ///
    /// The video buffer is valid only after the capture has ended.
    /// This function provides a read-only access to the internal
    /// video buffer, but it can be copied if you need to
    /// make any modification to it.
    ///
    /// \return Read-only access to the video buffer
    ///
    ////////////////////////////////////////////////////////////
    const VideoBuffer& getBuffer() const;

protected:

    ////////////////////////////////////////////////////////////
    /// \brief Start capturing video data
    ///
    /// \return True to start the capture, or false to abort it
    ///
    ////////////////////////////////////////////////////////////
    virtual bool onStart();

    ////////////////////////////////////////////////////////////
    /// \brief Process the next recorded frame

    /// \param frame Reference to the next recorded frame data
    ///
    /// \return True to continue the capture, or false to stop it
    ///
    ////////////////////////////////////////////////////////////
    virtual bool onProcessNextFrame(const Image& frame);

    ////////////////////////////////////////////////////////////
    /// \brief Stop capturing video data
    ///
    ////////////////////////////////////////////////////////////
    virtual void onStop();

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::vector<Image> m_frames; ///< Temporary frame buffer to hold the recorded data
    VideoBuffer        m_buffer; ///< Video buffer that will contain the recorded data
};

} // namespace sf

#endif // SFML_VIDEOBUFFERRECORDER_HPP

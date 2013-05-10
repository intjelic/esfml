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

#ifndef SFML_VIDEOFILE_HPP
#define SFML_VIDEOFILE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>

namespace sf
{
class InputStream;

namespace priv
{
class VideoFile : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////////////////////
    VideoFile();

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////////////////////
    ~VideoFile();

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Get the total number of video frames in the file
    ///
    /// \return Number of frames
    ///
    ////////////////////////////////////////////////////////////////////////////
    std::size_t getFrameCount() const;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Get the video size
    ///
    /// \return Size of the video (width x height)
    ///
    ////////////////////////////////////////////////////////////////////////////
    const Vector2i& getSize() const;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Get the number of frame per second (FPS)
    ///
    /// \return Number of frame per second
    ///
    ////////////////////////////////////////////////////////////////////////////
    unsigned int getFramePerSecond() const;

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Open a video file for reading
    ///
    /// \param filename Path of the video file to load
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool openRead(const std::string& filename);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Open a video file in memory for reading
    ///
    /// \param data        Pointer to the file data in memory
    /// \param sizeInBytes Size of the data to load, in bytes
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool openRead(const void* data, std::size_t sizeInBytes);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Open a video file from a custom stream for reading
    ///
    /// \param stream Source stream to read from
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool openRead(InputStream& stream);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Open a video file for writing
    ///
    /// \param filename   Path of the video file to write
    /// \param frameCount Number of channels in the sound
    /// \param fps        FPS of the video
    ///
    /// \return True if the file was successfully opened
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool openWrite(const std::string& filename, unsigned int frameCount, unsigned int fps);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Read video frames from the loaded video
    ///
    /// \param data       Pointer to the frame array to fill
    /// \param frameCount Number of frame to read
    ///
    /// \return Number of samples actually read (may be less than \a sampleCount)
    ///
    ////////////////////////////////////////////////////////////////////////////
    std::size_t read(std::vector<Image>& data, std::size_t frameCount);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Write video frames to the file
    ///
    /// \param data        Pointer to the frame array to write
    /// \param sampleCount Number of frame to write
    ///
    ////////////////////////////////////////////////////////////////////////////
    void write(const Texture* data, std::size_t frameCount);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Change the current read position in the file
    ///
    /// \param timeOffset New playing position, from the beginning of the file
    ///
    ////////////////////////////////////////////////////////////////////////////
    void seek(Time timeOffset);

private :
	void close();

    ////////////////////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////////////////////
    AVFormatContext* m_formatContext;
    unsigned int     m_streamIndex;
    AVCodecContext*  m_codecContext;
    AVCodec*         m_codec;
    SwsContext*      m_swsContext;

    std::size_t  m_frameCount;     ///< Total number of frames in the file
    Vector2i     m_size;           ///< Size of the video
    unsigned int m_framePerSecond; ///< Number of frame per second
};

} // namespace priv

} // namespace sf


#endif // SFML_VIDEOFILE_HPP

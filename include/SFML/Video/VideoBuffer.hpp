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

#ifndef SFML_VIDEOBUFFER_HPP
#define SFML_VIDEOBUFFER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Video/Export.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Time.hpp>
#include <set>
#include <string>

namespace sf
{
namespace priv
{
    class VideoFile;
}

class Video;
class InputStream;

////////////////////////////////////////////////////////////
/// \brief Storage for images defining a video
///
////////////////////////////////////////////////////////////
class SFML_VIDEO_API VideoBuffer
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    VideoBuffer();

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy Instance to copy
    ///
    ////////////////////////////////////////////////////////////
    VideoBuffer(const VideoBuffer& copy);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~VideoBuffer();

    ////////////////////////////////////////////////////////////
    /// \brief Load the video buffer from a file
    ///
    /// Here is a complete list of all the supported video formats: unknown.
    ///
    /// \param filename Path of the video file to load
    ///
    /// \return True if loading succeeded, false if it failed
    ///
    /// \see loadFromMemory, loadFromStream, loadFromFrames, saveToFile
    ///
    ////////////////////////////////////////////////////////////
    bool loadFromFile(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Load the video buffer from a file in memory
    ///
    /// Here is a complete list of all the supported video formats: unknown
    ///
    /// \param data        Pointer to the file data in memory
    /// \param sizeInBytes Size of the data to load, in bytes
    ///
    /// \return True if loading succeeded, false if it failed
    ///
    /// \see loadFromFile, loadFromStream, loadFromFrames
    ///
    ////////////////////////////////////////////////////////////
    bool loadFromMemory(const void* data, std::size_t sizeInBytes);

    ////////////////////////////////////////////////////////////
    /// \brief Load the video buffer from a custom stream
    ///
    /// Here is a complete list of all the supported video formats: unknown.
    ///
    /// \param stream Source stream to read from
    ///
    /// \return True if loading succeeded, false if it failed
    ///
    /// \see loadFromFile, loadFromMemory, loadFromFrames
    ///
    ////////////////////////////////////////////////////////////
    bool loadFromStream(InputStream& stream);

    ////////////////////////////////////////////////////////////
    /// \brief Load the video buffer from an array of images
    ///
    /// \param frames         Pointer to the array of images in memory
    /// \param frameCount     Number of frames in the array
    /// \param size           Size of the video
    /// \param framePerSecond FPS (number of frames to play per second)
    ///
    /// \return True if loading succeeded, false if it failed
    ///
    /// \see loadFromFile, loadFromMemory, saveToFile
    ///
    ////////////////////////////////////////////////////////////
    bool loadFromFrames(const Image* frames, std::size_t frameCount, const Vector2u& size, unsigned int framePerSecond);

    ////////////////////////////////////////////////////////////
    /// \brief Save the video buffer to a video file
    ///
    /// Here is a complete list of all the supported video formats: unknown.
    ///
    /// \param filename Path of the video file to write
    ///
    /// \return True if saving succeeded, false if it failed
    ///
    /// \see loadFromFile, loadFromMemory, loadFromFrames
    ///
    ////////////////////////////////////////////////////////////
    bool saveToFile(const std::string& filename) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the array of images stored in the buffer
    ///
    /// \return Read-only pointer to the array of images
    ///
    /// \see getSize
    ///
    ////////////////////////////////////////////////////////////
    const Image* getFrames() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of images stored in the buffer
    ///
    /// The array of samples can be accessed with the getFrames()
    /// function.
    ///
    /// \return Number of frames
    ///
    /// \see getFrames
    ///
    ////////////////////////////////////////////////////////////
    std::size_t getFrameCount() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the number of frames per second of the video
    ///
    /// The number of frames played per second.
    /// The higher, the better the quality (for example, 26 frames/s is very a
    /// good quality.
    ///
    /// \return FPS (frames per second)
    ///
    /// \see getSize, getDuration
    ///
    ////////////////////////////////////////////////////////////
    unsigned int getFramePerSecond() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the size of the video
    ///
    /// \return Size of the video
    ///
    /// \see getFramePerSecond, getDuration
    ///
    ////////////////////////////////////////////////////////////
    Vector2i getSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the total duration of the video
    ///
    /// \return Video duration
    ///
    /// \see getFramePerSecond, getSize
    ///
    ////////////////////////////////////////////////////////////
    Time getDuration() const;

    ////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    VideoBuffer& operator =(const VideoBuffer& right);

private :

    friend class Video;

    ////////////////////////////////////////////////////////////
    /// \brief Add a video to the list of videos that use this buffer
    ///
    /// \param sound Video instance to attach
    ///
    ////////////////////////////////////////////////////////////
    void attachVideo(Video* video) const;

    ////////////////////////////////////////////////////////////
    /// \brief Remove a video from the list of videos that use this buffer
    ///
    /// \param video Video instance to detach
    ///
    ////////////////////////////////////////////////////////////
    void detachVideo(Video* video) const;

    ////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////////////////////////
    typedef std::set<Video*> VideoList; ///< Set of unique video instances

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::vector<Image> m_frames;         ///< Frame buffer
    unsigned int       m_frameCount;     ///< Number of frame in this buffer
    unsigned int       m_framePerSecond; ///< Number of frame per second
    Time               m_duration;       ///< Video duration
    mutable VideoList  m_videos;         ///< List of videos that are using this buffer
};

} // namespace sf


#endif // SFML_VIDEOBUFFER_HPP

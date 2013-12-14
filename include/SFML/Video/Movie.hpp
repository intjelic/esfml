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

#ifndef SFML_MOVIE_HPP
#define SFML_MOVIE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Video/Export.hpp>
#include <SFML/Video/VideoStream.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Time.hpp>
#include <string>
#include <vector>


namespace sf
{
namespace priv
{
    class VideoFile;
}

class InputStream;

class SFML_VIDEO_API Movie : public VideoStream
{
public :

    Movie();
    ~Movie();

    bool openFromFile(const std::string& filename);
    bool openFromMemory(const void* data, std::size_t sizeInBytes);
    bool openFromStream(InputStream& stream);

    Time getDuration() const;

protected :

    virtual bool onGetData(std::vector<Image>& data);
    virtual void onSeek(Time timeOffset);

private :

    void initialize();

    priv::VideoFile*   m_file;     ///< Video file
    Time               m_duration; ///< Video duration
    std::vector<Image> m_frames;   ///< Temporary buffer of frames
    Mutex              m_mutex;    ///< Mutex protecting the data
};

} // namespace sf


#endif // SFML_VIDEOSTREAM_HPP

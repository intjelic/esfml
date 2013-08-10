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

#ifndef SFML_VIDEOSOURCE_HPP
#define SFML_VIDEOSOURCE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Video/Export.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Base class defining a video's properties
///
////////////////////////////////////////////////////////////
class SFML_VIDEO_API VideoSource : public Drawable, public Transformable
{
public :

	////////////////////////////////////////////////////////////
    /// \brief Enumeration of the video source states
    ///
	////////////////////////////////////////////////////////////
    enum Status
    {
        Stopped, ///< Video is not playing
        Paused,  ///< Video is paused
        Playing  ///< Video is playing
    };

	////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy Instance to copy
    ///
	////////////////////////////////////////////////////////////
    VideoSource(const VideoSource& copy);

    ////////////////////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////////////////////
    virtual ~VideoSource();

protected :

	////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor is meant ot be called by derived classes only.
    ///
	////////////////////////////////////////////////////////////
    VideoSource();

	////////////////////////////////////////////////////////////
    /// \brief Get the current status of the video (stopped, paused, playing)
    ///
    /// \return Current status of the video
    ///
	////////////////////////////////////////////////////////////
    Status getStatus() const;

	////////////////////////////////////////////////////////////
    // Member data
	////////////////////////////////////////////////////////////
    Status m_status; ///< The current status of the video (stopped, paused, playing)

};

} // namespace sf


#endif // SFML_VIDEOSOURCE_HPP

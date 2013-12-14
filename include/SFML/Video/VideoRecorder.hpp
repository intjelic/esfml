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

#ifndef SFML_VIDEORECORDER_HPP
#define SFML_VIDEORECORDER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Video/Export.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Thread.hpp>
#include <vector>
#include <opencv2/opencv.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Abstract base class for capturing video data
///
////////////////////////////////////////////////////////////
class SFML_VIDEO_API VideoRecorder
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~VideoRecorder();

    ////////////////////////////////////////////////////////////
    /// \brief Start the capture
    ///
    /// The \a frameRate parameter defines the number of video frame
    /// captured per second. The higher, the better the quality
    /// (for example, 24 frames per second is standard).
    /// This function uses its own thread so that it doesn't block
    /// the rest of the program while the capture runs.
    /// Please note that only one capture can happen at the same time.
    ///
    /// \param frameRate Desired capture rate, in number of frames per second
    ///
    /// \see stop
    ///
    ////////////////////////////////////////////////////////////
    void start(unsigned int frameRate = 24);

    ////////////////////////////////////////////////////////////
    /// \brief Stop the capture
    ///
    /// \see start
    ///
    ////////////////////////////////////////////////////////////
    void stop();

    ////////////////////////////////////////////////////////////
    /// \brief Get the frame rate
    ///
    /// The frame rate defines the number of video frame
    /// captured per second. The higher, the better the quality
    /// (for example, 24 frames per second is standard).
    ///
    /// \return Frame rate, in frames per second
    ///
    ////////////////////////////////////////////////////////////
    unsigned int getFrameRate() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the size of videos captured by this device
    ///
    /// The video size defines the quality of the capture device.
    ///
    /// \return Video size, in frames per second
    ///
    ////////////////////////////////////////////////////////////
    Vector2u getVideoSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Check if the system supports video capture
    ///
    /// This function should always be called before using
    /// the video capture features. If it returns false, then
    /// any attempt to use sf::VideoRecorder or one of its derived
    /// classes will fail.
    ///
    /// \return True if video capture is supported, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isAvailable();

protected :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor is only meant to be called by derived classes.
    ///
    ////////////////////////////////////////////////////////////
    VideoRecorder();

    ////////////////////////////////////////////////////////////
    /// \brief Start capturing video data
    ///
    /// This virtual function may be overriden by a derived class
    /// if something has to be done every time a new capture
    /// starts. If not, this function can be ignored; the default
    /// implementation does nothing.
    ///
    /// \return True to start the capture, or false to abort it
    ///
    ////////////////////////////////////////////////////////////
    virtual bool onStart();

    ////////////////////////////////////////////////////////////
    /// \brief Process the next recorded frame
    ///
    /// This virtual function is called every time a new recorded
    /// frame data is available. The derived class can then do
    /// whatever it wants with it (storing it, playing it, sending
    /// it over the network, etc.).
    ///
    /// \param frame Reference to the next recorded frame data
    ///
    /// \return True to continue the capture, or false to stop it
    ///
    ////////////////////////////////////////////////////////////
    virtual bool onProcessNextFrame(const Image& frame) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Stop capturing video data
    ///
    /// This virtual function may be overriden by a derived class
    /// if something has to be done every time the capture
    /// ends. If not, this function can be ignored; the default
    /// implementation does nothing.
    ///
    ////////////////////////////////////////////////////////////
    virtual void onStop();

private :

    ////////////////////////////////////////////////////////////
    /// \brief Function called as the entry point of the thread
    ///
    /// This function starts the recording loop, and returns
    /// only when the capture is stopped.
    ///
    ////////////////////////////////////////////////////////////
    void record();

    ////////////////////////////////////////////////////////////
    /// \brief Get the next available video frame and process it
    ///
    /// This function is called continuously during the
    /// capture loop. It retrieves the captured frame and
    /// forwards it to the derived class.
    ///
    ////////////////////////////////////////////////////////////
    void captureNextFrame();

    ////////////////////////////////////////////////////////////
    /// \brief Clean up the recorder's internal resources
    ///
    /// This function is called when the capture stops.
    ///
    ////////////////////////////////////////////////////////////
    void cleanup();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Thread       m_thread;      ///< Thread running the background recording task
    Image        m_frame;       ///< Buffer to store the captured frame
    unsigned int m_frameRate ;  ///< Frame rate
    bool         m_isCapturing; ///< Capturing state
};

} // namespace sf


#endif // SFML_VIDEORECORDER_HPP

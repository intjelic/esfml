
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Video.hpp>
#include <iomanip>
#include <iostream>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Check that the device can capture video
    if (sf::VideoRecorder::isAvailable() == false)
    {
        std::cout << "Sorry, video capture is not supported by your system" << std::endl;
        return EXIT_SUCCESS;
    }

    // Choose the frame rate
    unsigned int frameRate;
    std::cout << "Please choose the frame rate for video capture (24 is standard) : ";
    std::cin  >> frameRate;
    std::cin.ignore(10000, '\n');

    // Wait for user input...
    std::cout << "Press enter to start recording video";
    std::cin.ignore(10000, '\n');

    // Here we'll use an integrated custom recorder, which saves the captured data into a VideoBuffer
    sf::VideoBufferRecorder recorder;

    // Video capture is done in a separate thread, so we can block the main thread while it is capturing
    recorder.start(frameRate);
    std::cout << "Recording... press enter to stop";
    std::cin.ignore(10000, '\n');
    recorder.stop();

    // Get the buffer containing the captured data
    const sf::VideoBuffer& buffer = recorder.getBuffer();

    // Display captured video informations
    std::cout << "Video information :" << std::endl;
    std::cout << " " << buffer.getDuration().asSeconds() << " seconds"         << std::endl;
    std::cout << " " << buffer.getFramePerSecond()       << " frames / second" << std::endl;
    std::cout << " " << buffer.getFrameCount()           << " images"          << std::endl;

    // Choose what to do with the recorded video data
    char choice;
    std::cout << "What do you want to do with captured video (p = play, s = save) ? ";
    std::cin  >> choice;
    std::cin.ignore(10000, '\n');

    if (choice == 's')
    {
        // Choose the filename
        std::string filename;
        std::cout << "Choose the file to create : ";
        std::getline(std::cin, filename);

        // Save the buffer
        buffer.saveToFile(filename);
    }
    else
    {
        // Create a video instance and play it
        sf::Video video(buffer);
        video.play();

		// Create the window to display the video
		sf::Vector2u size = recorder.getVideoSize();
		sf::RenderWindow window(sf::VideoMode(size.x, size.y), "SFML - Video Capture");
		std::cout << "Remove this print statement and you'll get a segmentation fault (O.o)" << std::endl;

        // Wait until finished
        while (video.getStatus() == sf::Video::Playing)
        {
            // Display the playing position
            std::cout << "\rPlaying... " << std::fixed << std::setprecision(2) << video.getPlayingOffset().asSeconds() << " sec";
            std::cout << std::flush;

			// Catch the close event to interupt the video if needed
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
					break;
			}

			// Draw the video on the screen
			window.clear();
			window.draw(video);
			window.display();

            // Leave some CPU time for other threads
            sf::sleep(sf::milliseconds(100));
        }
    }

    // Finished !
    std::cout << std::endl << "Done !" << std::endl;

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10000, '\n');

    return EXIT_SUCCESS;
}

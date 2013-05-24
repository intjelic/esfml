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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Video/VideoFile.hpp>
#include <SFML/System/Err.hpp>

#include <iostream>

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
VideoFile::VideoFile() :
m_formatContext  (NULL),
m_streamIndex    (-1),
m_codecContext   (NULL),
m_codec          (NULL),
m_swsContext     (NULL),
m_frameCount     (0),
m_size           (0, 0),
m_framePerSecond (0)
{
}

////////////////////////////////////////////////////////////
VideoFile::~VideoFile()
{
	close();
}

////////////////////////////////////////////////////////////
std::size_t VideoFile::getFrameCount() const
{
	return m_frameCount;
}


////////////////////////////////////////////////////////////
const Vector2i& VideoFile::getSize() const
{
	return m_size;
}


////////////////////////////////////////////////////////////
unsigned int VideoFile::getFramePerSecond() const
{
	return m_framePerSecond;
}


////////////////////////////////////////////////////////////
bool VideoFile::openRead(const std::string& filename)
{
	// If the file is already opened, first close it
	if (m_formatContext)
		close();

	// Open a multimedia file
	int ret = avformat_open_input(&m_formatContext, filename.c_str(), NULL, NULL);

	if (ret < 0)
	{
		err() << "Couldn't open this multimedia file (" << filename << ")" << std::endl;
		close();
		return false;
	}

	// Read multimedia file info
	ret = avformat_find_stream_info(m_formatContext, NULL);

	if (ret < 0)
	{
		err() << "Couldn't get stream information" << std::endl;
		close();
		return false;
	}

	// TODO: if debug mode, dump stream information

	// Find a video stream
	m_streamIndex = 0;
	ret = av_find_best_stream(m_formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, &m_codec, 0);

	if (ret < 0)
	{
		if (ret == AVERROR_DECODER_NOT_FOUND)
			err() << "Couldn't find a video stream in this multimedia file" << std::endl;
		else if (ret == AVERROR_DECODER_NOT_FOUND)
			err() << "Couldn't find a suitable codec for this video stream" << std::endl;
		else
			err() << "An unknown error occured while trying to find the video stream" << std::endl;

		close();
		return false;
	}

	m_streamIndex = ret;

	// Retrieve the codec context
	m_codecContext = m_formatContext->streams[m_streamIndex]->codec;

	// Find and load the codec
	m_codec = avcodec_find_decoder(m_codecContext->codec_id);

	if (!m_codec)
	{
		err() << "Couldn't find a suitable codec for this video stream" << std::endl;
		close();
		return false;
	}

	ret = avcodec_open2(m_codecContext, m_codec, NULL);

	if (ret < 0)
	{
		err() << "Couldn't open the suitable codec for this video stream" << std::endl;
		close();
		return false;
	}

	// Get the video size
	m_size = sf::Vector2i(m_codecContext->width, m_codecContext->height);

	// Initialize the scaling/color convertion context
	m_swsContext = sws_getContext(m_size.x, m_size.y, m_codecContext->pix_fmt,
		m_size.x, m_size.y, PIX_FMT_RGBA, SWS_FAST_BILINEAR, NULL, NULL, NULL);

	std::cout << "Remove this print statement and you'll get a segmentation fault (O.o)" << std::endl;
}


////////////////////////////////////////////////////////////
bool VideoFile::openRead(const void* data, std::size_t sizeInBytes)
{
	return false;
}


////////////////////////////////////////////////////////////
bool VideoFile::openRead(InputStream& stream)
{
	return false;
}


////////////////////////////////////////////////////////////
bool VideoFile::openWrite(const std::string& filename, unsigned int channelCount, unsigned int sampleRate)
{
	return false;
}


////////////////////////////////////////////////////////////
std::size_t VideoFile::read(std::vector<Image>& data, std::size_t frameCount)
{
	// Abord if no multimedia file is open
	if (!m_formatContext)
	{
		data.clear();
		data.reserve(0);
		return 0;
	}

	// Allocate space to store the x next images
	data.resize(frameCount);

	// Initialize the packet which will contain video frames
	AVPacket pkt;

	av_init_packet(&pkt);
	pkt.data = NULL;
	pkt.size = 0;

	// Allocate the frames which will receive video frame data
	AVFrame* rawFrame = avcodec_alloc_frame();
	AVFrame* rgbaFrame = avcodec_alloc_frame();

	if (!rawFrame || !rgbaFrame)
	{
		err() << "Couldn't allocate frames" << std::endl;
		return false;
	}

	// Allocate two buffers to perform color format convertions
	int rawSize = avpicture_get_size(m_codecContext->pix_fmt, m_size.x, m_size.y);
	int rgbaSize = avpicture_get_size(PIX_FMT_RGBA, m_size.x, m_size.y);

	uint8_t* rawPictureBuf = (uint8_t *)av_malloc(rawSize);
	uint8_t* rgbaPictureBuf = (uint8_t *)av_malloc(rgbaSize);

	avpicture_fill((AVPicture *)rawFrame, rawPictureBuf, m_codecContext->pix_fmt, m_size.x, m_size.y);
	avpicture_fill((AVPicture *)rgbaFrame, rgbaPictureBuf, PIX_FMT_RGBA, m_size.x, m_size.y);

	// Now we're decoding images one by one
	int currentFrame = 0;
	int frameDecoded;

	while (currentFrame < frameCount)
	{
		// Read the next chunk of data
		if (av_read_frame(m_formatContext, &pkt) < 0)
		{
			err() << "Couldn't read the next frame" << std::endl;
			break;
		}

		// Skip if not a video frame
		if (pkt.stream_index != m_streamIndex)
		{
			av_free_packet(&pkt);
			continue;
		}

		// Decode the video frame
		avcodec_decode_video2(m_codecContext, rawFrame, &frameDecoded, &pkt);

		// Convert color format to RGBA
		sws_scale(m_swsContext, rawFrame->data, rawFrame->linesize, 0, m_size.y, rgbaFrame->data, rgbaFrame->linesize);
		// Append our decoded and well formatted frame
		data[currentFrame].create(m_size.x, m_size.y, (const sf::Uint8*)rgbaFrame->data[0]);

		av_free_packet(&pkt);

		currentFrame++;
	}

	// Deallocate temporary resources
	av_free(rawPictureBuf);
	av_free(rgbaPictureBuf);

	av_free(rawFrame);
	av_free(rgbaFrame);

	return currentFrame + 1;
}


////////////////////////////////////////////////////////////
void VideoFile::write(const Texture* data, std::size_t frameCount)
{
}


////////////////////////////////////////////////////////////
void VideoFile::seek(Time timeOffset)
{
}

////////////////////////////////////////////////////////////////////////////////
void VideoFile::close()
{
	avcodec_close(m_codecContext);
	avformat_close_input(&m_formatContext);

	m_streamIndex = -1;
	m_size = Vector2i(0, 0);
}


} // namespace priv

} // namespace sf

/*
 * Copyright (c) 2016 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual
 * property and proprietary rights in and to this software,
 * related documentation and any modifications thereto.  Any use,
 * reproduction, disclosure or distribution of this software and
 * related documentation without an express license agreement from
 * NVIDIA Corporation is strictly prohibited.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE
 * IS PROVIDED *AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL
 * WARRANTIES, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS BE
 * LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR
 * LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF BUSINESS
 * INFORMATION, OR ANY OTHER PECUNIARY LOSS) ARISING OUT OF THE USE OF
 * OR INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS BEEN ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGES
 */
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#ifdef __cplusplus
}
#endif

#include "libnvpipecodec/nvpipecodec.h"
#include "libnvpipeutil/formatConversionCuda.h"
#include <string>

class NvPipeCodec264 : public NvPipeCodec {

public:
    virtual int encode( void* buffer,
                        size_t &size,
                        enum NVPipeImageFormat format);

    virtual int decode( void* picture, 
                        int &width, 
                        int &height, 
                        size_t &size,
                        enum NVPipeImageFormat format);

    virtual void setImageSize( int width, int height);

    virtual void setBitrate( int64_t bitrate );

    virtual void setInputFrameBuffer(   void* frame_buffer, 
                                        size_t buffer_size);

    NvPipeCodec264();
    virtual ~NvPipeCodec264();

protected:
    AVCodecContext *encoder_context_;
    AVCodec *encoder_codec_;
    AVFrame *encoder_frame_;
    AVPacket encoder_packet_;

    AVCodecContext *decoder_context_;
    AVCodec *decoder_codec_;
    AVFrame *decoder_frame_;
    AVPacket decoder_packet_;

private:

    _nvpipeMemGpu2 memgpu2_;

    int getFormatConversionEnum(
            enum NVPipeImageFormat format,
            bool encoder_flag,
            enum NVPipeImageFormatConversion &conversion_flag,
            enum AVPixelFormat &pixel_format);

    // append 2 dummy access delimiter NAL at the end.
    void appendDummyNAL(void* buffer, size_t offset);

    bool encoder_config_corrupted_;
    bool decoder_config_corrupted_;

    bool encoder_frame_buffer_corrupted_;

    // not the best implementation.
    enum NVPipeImageFormatConversion encoder_conversion_flag_;
    void* encoder_converted_image_buffer_;
    size_t encoder_converted_image_buffer_size_;

    enum NVPipeImageFormatConversion decoder_conversion_flag_;

    enum AVPixelFormat encoder_frame_pixel_format_;

    std::string getEncoderName();
    std::string getDecoderName();

    int configureEncoderContext();
};

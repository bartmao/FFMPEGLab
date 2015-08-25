//#include <stdio.h>
//#include <iostream>
//#include "windows.h"
//#pragma warning(disable: 4996)
//
//extern "C"
//{
//#include "libavcodec/avcodec.h"
//#include "libavformat/avformat.h"
//#include "libswscale/swscale.h"
//#include "SDL/SDL.h"
//#include "libavfilter/avfiltergraph.h"  
//#include "libavfilter/avcodec.h"  
//#include "libavfilter/buffersink.h"  
//#include "libavfilter/buffersrc.h"  
//#include "libavutil/error.h"
//};
//using namespace std;
//#define __STDC_CONSTANT_MACROS
////Refresh
//#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)
//
////Thread
//
//extern "C"
//{
//	SDL_Overlay* bmp;
//	bool pause = false;
//	int decodeInterval = 10;
//	int thread_exit = 0;
//
//	const char *filter_descr = "movie=1.jpg[wm];[in][wm]overlay=5:5[out]";
//
//	static AVFormatContext *pFormatCtx;
//	static AVCodecContext *pCodecCtx;
//	AVFilterContext *buffersink_ctx;
//	AVFilterContext *buffersrc_ctx;
//	AVFilterGraph *filter_graph;
//
//	static int init_filters(const char *filters_descr)
//	{
//		char args[512];
//		int ret;
//		AVFilter *buffersrc = avfilter_get_by_name("buffer");
//		AVFilter *buffersink = avfilter_get_by_name("ffbuffersink");
//		AVFilterInOut *outputs = avfilter_inout_alloc();
//		AVFilterInOut *inputs = avfilter_inout_alloc();
//		enum PixelFormat pix_fmts[] = { PIX_FMT_YUV420P, PIX_FMT_NONE };
//		AVBufferSinkParams *buffersink_params;
//
//		filter_graph = avfilter_graph_alloc();
//
//		/* buffer video source: the decoded frames from the decoder will be inserted here. */
//		_snprintf(args, sizeof(args),
//			"video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d",
//			pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
//			pCodecCtx->time_base.num, pCodecCtx->time_base.den,
//			pCodecCtx->sample_aspect_ratio.num, pCodecCtx->sample_aspect_ratio.den);
//
//		ret = avfilter_graph_create_filter(&buffersrc_ctx, buffersrc, "in",
//			args, NULL, filter_graph);
//		if (ret < 0) {
//			printf("Cannot create buffer source\n");
//			return ret;
//		}
//
//		/* buffer video sink: to terminate the filter chain. */
//		buffersink_params = av_buffersink_params_alloc();
//		buffersink_params->pixel_fmts = pix_fmts;
//		ret = avfilter_graph_create_filter(&buffersink_ctx, buffersink, "out",
//			NULL, buffersink_params, filter_graph);
//		av_free(buffersink_params);
//		if (ret < 0) {
//			printf("Cannot create buffer sink\n");
//			return ret;
//		}
//
//		/* Endpoints for the filter graph. */
//		outputs->name = av_strdup("in");
//		outputs->filter_ctx = buffersrc_ctx;
//		outputs->pad_idx = 0;
//		outputs->next = NULL;
//
//		inputs->name = av_strdup("out");
//		inputs->filter_ctx = buffersink_ctx;
//		inputs->pad_idx = 0;
//		inputs->next = NULL;
//
//		if ((ret = avfilter_graph_parse_ptr(filter_graph, filters_descr,
//			&inputs, &outputs, NULL)) < 0)
//		{
//			printf("error avfilter_graph_parse_ptr");
//			return ret;
//		}
//
//		if ((ret = avfilter_graph_config(filter_graph, NULL)) < 0)
//			return ret;
//		return 0;
//	}
//
//	int sfp_refresh_thread(void *opaque)
//	{
//		SDL_Event event;
//		while (thread_exit == 0) {
//			event.type = SFM_REFRESH_EVENT;
//			SDL_PushEvent(&event);
//			//Wait 40 ms
//			SDL_Delay(decodeInterval);
//		}
//		return 0;
//	}
//
//
//	void SaveFrame(AVFrame *pFrame, int width, int height, int index, int bpp)
//	{
//		//char buf[5] = {0};
//		int y;
//		BITMAPFILEHEADER bmpheader;
//		BITMAPINFOHEADER bmpinfo;
//		FILE *fp;
//		char szFilename[32];
//
//		// Open file
//		sprintf_s(szFilename, "frame%d.bmp", index);
//		fopen_s(&fp, szFilename, "wb");
//		if (fp == NULL)
//			return;
//		bmpheader.bfType = 0x4d42;
//		bmpheader.bfReserved1 = 0;
//		bmpheader.bfReserved2 = 0;
//		bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
//		bmpheader.bfSize = bmpheader.bfOffBits + width*height*bpp / 8;
//		bmpinfo.biSize = sizeof(BITMAPINFOHEADER);
//		bmpinfo.biWidth = width;
//		bmpinfo.biHeight = height;
//		bmpinfo.biPlanes = 1;
//		bmpinfo.biBitCount = bpp;
//		bmpinfo.biCompression = BI_RGB;
//		bmpinfo.biSizeImage = (width*bpp + 31) / 32 * 4 * height;
//		bmpinfo.biXPelsPerMeter = 100;
//		bmpinfo.biYPelsPerMeter = 100;
//		bmpinfo.biClrUsed = 0;
//		bmpinfo.biClrImportant = 0;
//		fwrite(&bmpheader, sizeof(bmpheader), 1, fp);
//		fwrite(&bmpinfo, sizeof(bmpinfo), 1, fp);
//		for (y = height - 1; y >= 0; y--)
//			fwrite(pFrame->data[0] + y*pFrame->linesize[0], 1, width * 3, fp);
//		fclose(fp);
//	}
//
//	__declspec(dllexport) const char* doSomthing1(const char* s1, char* s2, char** s3)
//	{
//		return "Bart111";
//	}
//
//	__declspec(dllexport) const void  Pause(bool pauseAction)
//	{
//		pause = pauseAction;
//	}
//
//	__declspec(dllexport) const void  FastForward(bool isFaster)
//	{
//		if (isFaster && decodeInterval > 0)
//			decodeInterval -= 10;
//		else if (!isFaster)
//			decodeInterval += 10;
//	}
//
//	__declspec(dllexport) int doSomthing(SDL_Surface * screen)
//	{
//		int				i, videoindex;
//		
//		AVCodec			*pCodec;
//		AVFrame	*pFrame, *pFrameYUV;
//		AVPacket *packet;
//		struct SwsContext *img_convert_ctx;
//		//SDL
//		int ret, got_picture;
//		int screen_w = 0, screen_h = 0;
//		//SDL_Surface *screen;
//		//SDL_Overlay *bmp;
//		SDL_Rect rect;
//		SDL_Thread *video_tid;
//		SDL_Event event;
//		decodeInterval = 20;
//		char filepath[] = "e:\\1.flv";
//		av_register_all();
//		avcodec_register_all();
//		avformat_network_init();
//		avfilter_register_all();
//
//		pFormatCtx = avformat_alloc_context();
//
//		if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0){
//			printf("Couldn't open input stream.\n");
//			return -1;
//		}
//		if (avformat_find_stream_info(pFormatCtx, NULL) < 0){
//			printf("Couldn't find stream information.\n");
//			return -1;
//		}
//		videoindex = -1;
//		for (i = 0; i < pFormatCtx->nb_streams; i++)
//		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
//			videoindex = i;
//			break;
//		}
//		if (videoindex == -1){
//			printf("Didn't find a video stream.\n");
//			return -1;
//		}
//		pCodecCtx = pFormatCtx->streams[videoindex]->codec;
//		pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
//		if (pCodec == NULL)
//		{
//			printf("Codec not found.\n");
//			return -1;
//		}
//		if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
//		{
//			printf("Could not open codec.\n");
//			return -1;
//		}
//
//		if ((ret = init_filters(filter_descr)) < 0)
//			printf("failed to init filters\n");
//
//		pFrame = av_frame_alloc();
//		pFrameYUV = av_frame_alloc();
//		AVFrame *pFrameRGB = av_frame_alloc();
//		//uint8_t *out_buffer;
//		//out_buffer = new uint8_t[avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height)];
//		//avpicture_fill((AVPicture *)pFrameRGB, out_buffer, PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
//		//------------SDL----------------
//		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
//			printf("Could not initialize SDL - %s\n", SDL_GetError());
//			return -1;
//		}
//
//		int screenW = pCodecCtx->width*2;
//		int screenH = pCodecCtx->height*2;
//		SwsContext *img_convert_ctx1;
//		if (!screen) {
//			printf("SDL: could not set video mode - exiting:%s\n", SDL_GetError());
//			return -1;
//		}
//
//		bmp = SDL_CreateYUVOverlay(screenW, screenH, SDL_YV12_OVERLAY, screen);
//
//		rect.x = 0;
//		rect.y = 0;
//		rect.w = screenW;
//		rect.h = screenH;
//
//		packet = (AVPacket *)av_malloc(sizeof(AVPacket));
//
//		printf("---------------File Information------------------\n");
//		av_dump_format(pFormatCtx, 0, filepath, 0);
//		printf("-------------------------------------------------\n");
//		AVDictionaryEntry *m = NULL;
//		while (m = av_dict_get(pFormatCtx->metadata, "", m, AV_DICT_IGNORE_SUFFIX)){
//			printf("%s, %s \r\n", m->key, m->value);
//		}
//		printf("-------------------------------------------------\n");
//
//		img_convert_ctx1 = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
//		video_tid = SDL_CreateThread(sfp_refresh_thread, NULL);
//
//		AVRational time_base = pFormatCtx->streams[videoindex]->time_base;
//		int64_t dur = pFormatCtx->duration / AV_TIME_BASE;
//		printf("video total time:%lld", dur);
//
//		int ii = 0;
//		char title[5];
//		int64_t curPts = 0;
//		int jumpflag = 0;
//		int scaleflag = 0;
//		int64_t curTime = 0;
//
//		for (;;)
//		{
//			SDL_WaitEvent(&event);
//
//			if (event.type == SDL_KEYDOWN)
//			{
//				char* k = SDL_GetKeyName(event.key.keysym.sym);
//				if (k[0] == 'w')
//				{
//					jumpflag = 1;
//				}
//				else if (k[0] == 'x')
//				{
//					screen = SDL_SetVideoMode(screenW /= 2, screenH /= 2, 32, SDL_SWSURFACE);
//					SDL_FreeYUVOverlay(bmp);
//					bmp = SDL_CreateYUVOverlay(screenW, screenH, SDL_YV12_OVERLAY, screen);
//					rect.w = screenW;
//					rect.h = screenH;
//				}
//				else if (k[0] == 'y')
//				{
//					screen = SDL_SetVideoMode(screenW *= 2, screenH *= 2, 32, SDL_SWSURFACE);
//					SDL_FreeYUVOverlay(bmp);
//					bmp = SDL_CreateYUVOverlay(screenW, screenH, SDL_YV12_OVERLAY, screen);
//					rect.w = screenW;
//					rect.h = screenH;
//				}
//				//printf("code:%s\n", k);
//			}
//			else if (event.type == SFM_REFRESH_EVENT && !pause){
//				//------------------------------
//				if (av_read_frame(pFormatCtx, packet) >= 0){
//					if (packet->stream_index == videoindex){
//						curPts = packet->pts;
//						curTime = curPts * time_base.num / (int64_t)time_base.den;
//						printf("CurTime:%d\n", curTime);
//						if (jumpflag)
//						{
//							/*AVRational r = { 1, AV_TIME_BASE };
//							curPts += 60;
//							int64_t kk = av_rescale_q(curPts, r, pFormatCtx->streams[videoindex]->time_base);
//							printf("fast-forward, curPts:%lld , dstPts: %lld \n", curPts, curPts + 60);*/
//
//							int64_t seek_target = curTime + 300;
//							int64_t DesiredFrameNumber = av_rescale(seek_target, pFormatCtx->streams[videoindex]->time_base.den, pFormatCtx->streams[videoindex]->time_base.num);
//							if (av_seek_frame(pFormatCtx, videoindex,
//								DesiredFrameNumber, AVSEEK_FLAG_BACKWARD));
//							avcodec_flush_buffers(pFormatCtx->streams[videoindex]->codec);
//							jumpflag = 0;
//							continue;
//						}
//						//printf("Packet------- pts:%lld  dts:%lld\n", packet->pts, packet->dts);
//						ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
//						if (ret < 0){
//							printf("Decode Error.\n");
//							return -1;
//						}
//						if (got_picture){
//							ii++;
//							//SDL_LockYUVOverlay(bmp);
//							//pFrameYUV->data[0] = bmp->pixels[0];
//							//pFrameYUV->data[1] = bmp->pixels[2];
//							//pFrameYUV->data[2] = bmp->pixels[1];
//							//pFrameYUV->linesize[0] = bmp->pitches[0];
//							//pFrameYUV->linesize[1] = bmp->pitches[2];
//							//pFrameYUV->linesize[2] = bmp->pitches[1];
//							//
//
//
//
//							//img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, screenW, screenH, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
//							//sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
//							//sws_freeContext(img_convert_ctx);
//
//
//							//SDL_UnlockYUVOverlay(bmp);
//							//SDL_DisplayYUVOverlay(bmp, &rect);
//							//sprintf(title, "%d--%d", ii++, packet->duration);
//							//SDL_WM_SetCaption(title, NULL);
//
//							if (av_buffersrc_add_frame(buffersrc_ctx, pFrame) < 0) {
//								printf("Error while feeding the filtergraph\n");
//								break;
//							}
//
//							AVFilterBufferRef *picref;
//							ret = av_buffersink_get_buffer_ref(buffersink_ctx, &picref, 0);
//							if (ret < 0) printf("failed av_buffersink_get_buffer_ref \n");
//
//							SDL_LockYUVOverlay(bmp);
//							int y_size = picref->video->w*picref->video->h;
//							memcpy(bmp->pixels[0], picref->data[0], y_size);   //Y  
//							memcpy(bmp->pixels[2], picref->data[1], y_size / 4); //U  
//							memcpy(bmp->pixels[1], picref->data[2], y_size / 4); //V   
//							bmp->pitches[0] = picref->linesize[0];
//							bmp->pitches[2] = picref->linesize[1];
//							bmp->pitches[1] = picref->linesize[2];
//
//
//							SDL_UnlockYUVOverlay(bmp);
//							rect.x = 0;
//							rect.y = 0;
//							rect.w = picref->video->w;
//							rect.h = picref->video->h;
//							SDL_DisplayYUVOverlay(bmp, &rect);
//							avfilter_unref_bufferp(&picref);
//
//						}
//					}
//					av_free_packet(packet);
//				}
//				else{
//					//Exit Thread
//					//thread_exit = 1;
//					//break;
//
//					if (av_seek_frame(pFormatCtx, videoindex,
//						0, AVSEEK_FLAG_BACKWARD));
//					avcodec_flush_buffers(pFormatCtx->streams[videoindex]->codec);
//				}
//			}
//		}
//
//		SDL_Quit();
//
//		//sws_freeContext(img_convert_ctx);
//		//sws_freeContext(img_convert_ctx1);
//
//		//--------------
//		//av_free(out_buffer);
//		av_free(pFrameYUV);
//		avcodec_close(pCodecCtx);
//		avformat_close_input(&pFormatCtx);
//
//		return 0;
//	}
//
//
//
//}
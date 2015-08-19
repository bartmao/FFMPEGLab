using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace FFMPEGExp
{
    class Program
    {
        [DllImport("avformat-55", EntryPoint = "av_register_all", CallingConvention = CallingConvention.Cdecl)]
        public extern static void AVRegisterAll();

        [DllImport("avformat-55", EntryPoint = "avformat_network_init", CallingConvention = CallingConvention.Cdecl)]
        public extern static int AVFormatNetworkInit();

        [DllImport("avformat-55", EntryPoint = "avformat_alloc_context", CallingConvention = CallingConvention.Cdecl)]
        public extern static IntPtr AVFormatAllocContext();

        [DllImport("avutil-52", EntryPoint = "av_frame_alloc", CallingConvention = CallingConvention.Cdecl)]
        public extern static IntPtr AVFrameAlloc();

        [DllImport("avformat-55", EntryPoint = "avformat_open_input", CallingConvention = CallingConvention.Cdecl)]
        public extern static int AVFormatOpenInput(out IntPtr context, string filePath, IntPtr format, IntPtr dict);

        [DllImport("avformat-55", EntryPoint = "avformat_find_stream_info", CallingConvention = CallingConvention.Cdecl)]
        public extern static int AVFormatFindStreamInfo(IntPtr context, IntPtr dict);

        [DllImport("avcodec-55", EntryPoint = "avcodec_find_decoder", CallingConvention = CallingConvention.Cdecl)]
        public extern static IntPtr AVCodecFindDecoder(int codecId);

        [DllImport("avcodec-55", EntryPoint = "avcodec_open2", CallingConvention = CallingConvention.Cdecl)]
        public extern static int AVCodecOpen2(IntPtr context, IntPtr codec, IntPtr options);

        [DllImport("avcodec-55", EntryPoint = "avcodec_close", CallingConvention = CallingConvention.Cdecl)]
        public extern static IntPtr AVCodecClose(IntPtr codec);

        [DllImport("avformat-55", EntryPoint = "avformat_close_input", CallingConvention = CallingConvention.Cdecl)]
        public extern static int AVFormatCloseInput(out IntPtr context);

        [DllImport("SDL", EntryPoint = "SDL_Init", CallingConvention = CallingConvention.Cdecl)]
        public extern static int SDLInit(int flags);

        [DllImport("SDL", EntryPoint = "SDL_SetVideoMode", CallingConvention = CallingConvention.Cdecl)]
        public extern static IntPtr SDLSetVideoMode(int width, int heigth, int bpp, int flags);

                [DllImport("SDL", EntryPoint = "SDL_CreateYUVOverlay", CallingConvention = CallingConvention.Cdecl)]
        public extern static IntPtr SDLCreateYUVOverlay(int width, int heigth, int format, IntPtr display);
        
        [DllImport("SDL", EntryPoint = "SDL_Quit", CallingConvention = CallingConvention.Cdecl)]
        public extern static int SDLQuit();

        static void Main(string[] args)
        {
            Console.WriteLine("Enter To Continue:");
            Console.ReadLine();
            
            new Form().Show();
            Console.ReadLine();
            
            return;
            var s = DoSomthing();
            AVRegisterAll();
            AVFormatNetworkInit();
            var context = AVFormatAllocContext();
            var rst = AVFormatOpenInput(out context, "e:\\1.mp4", IntPtr.Zero, IntPtr.Zero);
            if (rst != 0)
            {
                Console.WriteLine("Couldn't open input stream.\n");
                return;
            }
            
            rst = AVFormatFindStreamInfo(context, IntPtr.Zero);
            if (rst < 0)
            {
                Console.WriteLine("Couldn't find stream information.\n");
                return;
            }

            var codecContextTuple = FindFirstVideoCodecContext(context);
            if (codecContextTuple.Item1 == -1)
            {
                Console.WriteLine("Didn't find a video stream.\n");
                return;
            }

            var codecContext = codecContextTuple.Item2;
            var codecId = Marshal.ReadInt32(codecContext, 48);
            var codec = AVCodecFindDecoder(codecId);
            if (codec == IntPtr.Zero)
            {
                Console.WriteLine("Didn't find a video stream.\n");
                return;
            }

            rst = AVCodecOpen2(codecContext, codec, IntPtr.Zero);
            if (rst < 0)
            {
                Console.WriteLine("Could not open codec.");
                return;
            }

            var frame = AVFrameAlloc();
            var framYUV = AVFrameAlloc();
            var flags = 0x20 | 0x10 | 0x1;
            if ((rst = SDLInit(flags)) != 0)
            {
                Console.WriteLine("Could not initialize SDL");
                return;
            }

            var screenWidth = Marshal.ReadInt32(codecContext, 116);
            var screenHeight = Marshal.ReadInt32(codecContext, 120);
            var sdlSurface = SDLSetVideoMode(screenWidth, screenHeight, 0, 0);
            if (sdlSurface == IntPtr.Zero) 
            {
                Console.WriteLine("SDL: could not set video mode");
                return;
            }
            var sdlOverlay = SDLCreateYUVOverlay(screenWidth, screenHeight, 0x32315659, sdlSurface);


            SDLQuit();
            AVCodecClose(codecContext);
            AVFormatCloseInput(out context);

            Console.WriteLine("Done");


            //DoSomthing("Bart");
            //var s = DoSomthing1("Bart1");
            //Console.WriteLine(Marshal.PtrToStringAnsi(s));
            //return;
        }

        static Tuple<int, IntPtr> FindFirstVideoCodecContext(IntPtr context)
        {
            var nbStreams = Marshal.ReadInt32(context, 6 * 4);
            var streams = Marshal.ReadIntPtr(context, 6 * 4 + 4);
            for (int i = 0; i < nbStreams; ++i)
            {
                var stream = Marshal.ReadIntPtr(streams, 4 * i);
                var codecContext = Marshal.ReadIntPtr(stream, 4 * 2);
                var code_type = Marshal.ReadInt32(codecContext, 4 * 2);
                if (code_type == 0)
                    return Tuple.Create(i, codecContext);
            }

            return Tuple.Create(-1, IntPtr.Zero);
        }

        [DllImport("FFMPEG-cpp", EntryPoint = "doSomthing", CallingConvention = CallingConvention.Cdecl)]
        public extern static int DoSomthing();

        [DllImport("FFMPEG-cpp", EntryPoint = "doSomthing1", CallingConvention = CallingConvention.Cdecl)]
        public extern static IntPtr DoSomthing1(string s1, IntPtr s2, ref IntPtr s3);


        public class AVFrame
        {
            public IntPtr[] Data { get; set; }

            public int[] LineSize { get; set; }

            public IntPtr ExtendedData { get; set; }

            public int Width { get; set; }

            public int Height { get; set; }

            public int NbSamples { get; set; }

            public int Format { get; set; }

            public int KeyFrame { get; set; }

            public AVPictureType PictType { get; set; }

            public static AVFrame GetInstance(IntPtr p)
            {
                var f = new AVFrame();
                int curPos = 0;
                var data = new IntPtr[8];
                for (int i = 0; i < 8; ++i)
                {
                    data[i] = Marshal.ReadIntPtr(p, curPos);
                    curPos += 4;
                }
                f.Data = data;
                var lineSize = new int[8];
                for (int i = 0; i < 8; ++i)
                {
                    lineSize[i] = Marshal.ReadInt32(p, curPos);
                    curPos += 4;
                }
                f.LineSize = lineSize;
                f.ExtendedData = Marshal.ReadIntPtr(p, curPos);
                curPos += 4;
                f.Width = Marshal.ReadInt32(p, curPos);
                curPos += 4;
                f.Height = Marshal.ReadInt32(p, curPos);
                curPos += 4;
                f.NbSamples = Marshal.ReadInt32(p, curPos);
                curPos += 4;
                f.Format = Marshal.ReadInt32(p, curPos);
                curPos += 4;
                f.KeyFrame = Marshal.ReadInt32(p, curPos);
                curPos += 4;
                f.PictType = (AVPictureType)Marshal.ReadInt32(p, curPos);
                return f;
            }
        }

        public enum AVPictureType
        {
            AV_PICTURE_TYPE_NONE = 0, ///< Undefined
            AV_PICTURE_TYPE_I,     ///< Intra
            AV_PICTURE_TYPE_P,     ///< Predicted
            AV_PICTURE_TYPE_B,     ///< Bi-dir predicted
            AV_PICTURE_TYPE_S,     ///< S(GMC)-VOP MPEG4
            AV_PICTURE_TYPE_SI,    ///< Switching Intra
            AV_PICTURE_TYPE_SP,    ///< Switching Predicted
            AV_PICTURE_TYPE_BI,    ///< BI type
        };
    }
}

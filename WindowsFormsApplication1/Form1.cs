using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        private IntPtr m_SdlWindowHandle;
        private IntPtr m_SdlSurface;
        //private Panel m_SdlPanel;
        Button button;
        private int pos = 0;

        public Form1()
        {
            InitializeComponent();
            CreateSdlWindow();
        }

        private void CreateSdlWindow()
        {

            // Initialize SDL
            SDL_Init(0x20);
            m_SdlSurface = SDL_SetVideoMode(640, 480, 0, 0);

            // Retrieve the Handle to the SDL Window
            SDL_SysWMinfo wmInfo;
            SDL_GetWMInfo(out wmInfo);
            m_SdlWindowHandle = new IntPtr(wmInfo.window);

            // Set the Window Position to 0/0
            SetWindowPos(m_SdlWindowHandle, IntPtr.Zero, 0, 0, 0, 0, (SetWindowPosFlags.IgnoreResize | SetWindowPosFlags.ShowWindow | SetWindowPosFlags.IgnoreMove));
            int style = GetWindowLong(m_SdlWindowHandle, GWL_STYLE);
            SetWindowLong(m_SdlWindowHandle, GWL_STYLE, (style & ~WS_CAPTION));
            // Make the SDL Window the child of our Panel
            SetParent(m_SdlWindowHandle, panel1.Handle);
            ShowWindow(m_SdlWindowHandle, ShowWindowCommand.SW_SHOWNORMAL);
            this.Invalidate();
        }

        [DllImport("FFMPEG-cpp", EntryPoint = "doSomthing", CallingConvention = CallingConvention.Cdecl)]
        public extern static int DoSomthing(IntPtr surface);

        [DllImport("FFMPEG-cpp", CallingConvention = CallingConvention.Cdecl)]
        public extern static void Pause(bool pauseAction);

        [DllImport("FFMPEG-cpp", CallingConvention = CallingConvention.Cdecl)]
        public extern static void FastForward(bool isFaster);

        [System.Runtime.InteropServices.DllImport("USER32.DLL")]
        public static extern int GetWindowLong(IntPtr hWnd, int nIndex);

        [DllImport("SDL.dll", SetLastError = true)]
        static extern IntPtr SDL_RWFromFile(string path, string mode);

        [DllImport("SDL.dll", SetLastError = true)]
        static extern IntPtr SDL_LoadBMP_RW(IntPtr file, int freeSrc);

        [DllImport("SDL.dll", SetLastError = true)]
        static extern void SDL_Flip(IntPtr surface);

        [DllImport("SDL.dll", SetLastError = true)]
        static extern IntPtr SDL_UpperBlit(IntPtr src, IntPtr srcRect, IntPtr dst, IntPtr dstRect);

        [DllImport("SDL.dll", SetLastError = true)]
        static extern void SDL_Init(int mode);

        [DllImport("SDL.dll", SetLastError = true)]
        static extern IntPtr SDL_SetVideoMode(int width, int height, int bpp, int flags);

        [DllImport("SDL.dll", SetLastError = true)]
        static extern void SDL_GetWMInfo(out SDL_SysWMinfo info);

        [DllImport("SDL.dll", SetLastError = true)]
        static extern IntPtr SDL_GetVideoInfo();

        [DllImport("SDL.dll", SetLastError = true)]
        static extern int SDL_MapRGB(IntPtr fmt, int x, int y, int z);

        [DllImport("SDL.dll", SetLastError = true)]
        static extern IntPtr SDL_FillRect(IntPtr surface, ref SDL_Rect rect, int color);

        [DllImport("SDL.dll", SetLastError = true)]
        static extern IntPtr SDL_UpdateRect(IntPtr surface, int m, int n, int x, int y);

        public static int GWL_STYLE = -16;
        public static int WS_CHILD = 0x40000000; //child window
        public static int WS_BORDER = 0x00800000; //window with border
        public static int WS_DLGFRAME = 0x00400000; //window with double border but no title
        public static int WS_CAPTION = WS_BORDER | WS_DLGFRAME; //window with a title bar

        public struct SDL_SysWMinfo
        {
            public char version1;
            public char version2;
            public char version3;
            public int window;
        }

        [StructLayout(LayoutKind.Sequential)]
        public class SDL_VideoInfo
        {
            public SDL_VideoInfo()
            {
                hw_available = 1;
                wm_available = 1;
                UnusedBits1 = 6;
                UnusedBits2 = 1;
                blit_hw = 1;
                blit_hw_CC = 1;
                blit_hw_A = 1;
                blit_sw = 1;
                blit_sw_CC = 1;
                blit_fill = 1;
                blit_sw_A = 1;
                UnusedBits3 = 16;

            }

            public UInt32 hw_available;
            public UInt32 wm_available;
            public UInt32 UnusedBits1;
            public UInt32 UnusedBits2;
            public UInt32 blit_hw;
            public UInt32 blit_hw_CC;
            public UInt32 blit_hw_A;
            public UInt32 blit_sw;
            public UInt32 blit_sw_CC;
            public UInt32 blit_sw_A;
            public UInt32 blit_fill;
            public UInt32 UnusedBits3;
            public UInt32 video_mem;
            public IntPtr vfmt;
            public int current_w;
            public int current_h;
        }

        [StructLayout(LayoutKind.Sequential)]
        public class SDL_Rect
        {
            public SDL_Rect(short left, short top, int width, int height)
            {
                Left = left;
                Top = top;
                Width = width;
                Height = height;
            }

            public short Left;
            public short Top;
            public int Width;
            public int Height;
        }

        #region Native Win32 Functions
        [DllImport("user32.dll", SetLastError = true)]
        static extern IntPtr SetParent(IntPtr child, IntPtr newParent);

        [DllImport("user32.dll", SetLastError = true)]
        static extern IntPtr FindWindow(string className, string windowName);

        [DllImport("user32.dll", SetLastError = true)]
        static extern bool ShowWindow(IntPtr handle, ShowWindowCommand command);

        [DllImport("user32.dll", SetLastError = true)]
        static extern bool SetWindowPos(IntPtr handle, IntPtr handleAfter, int x, int y, int cx, int cy, SetWindowPosFlags flags);

        [DllImport("user32.dll", SetLastError = true)]
        static extern long SetWindowLong(IntPtr hwnd, int index, long newVal);

        private enum ShowWindowCommand
        {
            SW_HIDE = 0,
            SW_SHOW = 5,
            SW_SHOWNA = 8,
            SW_SHOWNORMAL = 1,
            SW_SHOWMAXIMIZED = 3
        }

        private enum SetWindowPosFlags
        {
            /// <summary>If the calling thread and the thread that owns the window are attached to different input queues, 
            /// the system posts the request to the thread that owns the window. This prevents the calling thread from 
            /// blocking its execution while other threads process the request.</summary>
            /// <remarks>SWP_ASYNCWINDOWPOS</remarks>
            AsynchronousWindowPosition = 0x4000,
            /// <summary>Prevents generation of the WM_SYNCPAINT message.</summary>
            /// <remarks>SWP_DEFERERASE</remarks>
            DeferErase = 0x2000,
            /// <summary>Draws a frame (defined in the window's class description) around the window.</summary>
            /// <remarks>SWP_DRAWFRAME</remarks>
            DrawFrame = 0x0020,
            /// <summary>Applies new frame styles set using the SetWindowLong function. Sends a WM_NCCALCSIZE message to 
            /// the window, even if the window's size is not being changed. If this flag is not specified, WM_NCCALCSIZE 
            /// is sent only when the window's size is being changed.</summary>
            /// <remarks>SWP_FRAMECHANGED</remarks>
            FrameChanged = 0x0020,
            /// <summary>Hides the window.</summary>
            /// <remarks>SWP_HIDEWINDOW</remarks>
            HideWindow = 0x0080,
            /// <summary>Does not activate the window. If this flag is not set, the window is activated and moved to the 
            /// top of either the topmost or non-topmost group (depending on the setting of the hWndInsertAfter 
            /// parameter).</summary>
            /// <remarks>SWP_NOACTIVATE</remarks>
            DoNotActivate = 0x0010,
            /// <summary>Discards the entire contents of the client area. If this flag is not specified, the valid 
            /// contents of the client area are saved and copied back into the client area after the window is sized or 
            /// repositioned.</summary>
            /// <remarks>SWP_NOCOPYBITS</remarks>
            DoNotCopyBits = 0x0100,
            /// <summary>Retains the current position (ignores X and Y parameters).</summary>
            /// <remarks>SWP_NOMOVE</remarks>
            IgnoreMove = 0x0002,
            /// <summary>Does not change the owner window's position in the Z order.</summary>
            /// <remarks>SWP_NOOWNERZORDER</remarks>
            DoNotChangeOwnerZOrder = 0x0200,
            /// <summary>Does not redraw changes. If this flag is set, no repainting of any kind occurs. This applies to 
            /// the client area, the nonclient area (including the title bar and scroll bars), and any part of the parent 
            /// window uncovered as a result of the window being moved. When this flag is set, the application must 
            /// explicitly invalidate or redraw any parts of the window and parent window that need redrawing.</summary>
            /// <remarks>SWP_NOREDRAW</remarks>
            DoNotRedraw = 0x0008,
            /// <summary>Same as the SWP_NOOWNERZORDER flag.</summary>
            /// <remarks>SWP_NOREPOSITION</remarks>
            DoNotReposition = 0x0200,
            /// <summary>Prevents the window from receiving the WM_WINDOWPOSCHANGING message.</summary>
            /// <remarks>SWP_NOSENDCHANGING</remarks>
            DoNotSendChangingEvent = 0x0400,
            /// <summary>Retains the current size (ignores the cx and cy parameters).</summary>
            /// <remarks>SWP_NOSIZE</remarks>
            IgnoreResize = 0x0001,
            /// <summary>Retains the current Z order (ignores the hWndInsertAfter parameter).</summary>
            /// <remarks>SWP_NOZORDER</remarks>
            IgnoreZOrder = 0x0004,
            /// <summary>Displays the window.</summary>
            /// <remarks>SWP_SHOWWINDOW</remarks>
            ShowWindow = 0x0040,
        }
        #endregion

        private bool isPause = false;

        private void button1_Click_2(object sender, EventArgs e)
        {
            isPause = !isPause;
            Pause(isPause);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            DoSomthing(m_SdlSurface);

        }

        private void button3_Click(object sender, EventArgs e)
        {
            FastForward(true);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            FastForward(false);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            panel1.Invalidate();
        }
    }
}

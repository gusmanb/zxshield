using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ZXDesktopLoader
{
    public partial class GIFPlayer : Panel
    {
        #region Imports and declarations
        [DllImport("kernel32.dll")]
        static extern bool CreateTimerQueueTimer(out IntPtr phNewTimer,
        IntPtr TimerQueue, WaitOrTimerDelegate Callback, IntPtr Parameter,
        uint DueTime, uint Period, uint Flags);
        [DllImport("kernel32.dll")]
        static extern bool ChangeTimerQueueTimer(IntPtr TimerQueue, IntPtr Timer,
            uint DueTime, uint Period);
        [DllImport("kernel32.dll")]
        static extern bool DeleteTimerQueueTimer(IntPtr TimerQueue,
            IntPtr Timer, IntPtr CompletionEvent);
        public delegate void WaitOrTimerDelegate(IntPtr lpParameter,
            bool TimerOrWaitFired);
        public static WaitOrTimerDelegate UpdateFn;

        public enum ExecuteFlags
        {
            /// <summary>
            /// The callback function is queued to an I/O worker thread. This flag should be used if the function should be executed in a thread that waits in an alertable state.
            /// The callback function is queued as an APC. Be sure to address reentrancy issues if the function performs an alertable wait operation.
            /// </summary>
            WT_EXECUTEINIOTHREAD = 0x00000001,
        };

        #endregion

        private Image gif;
        private int frameCount = -1;
        private UInt32[] frameIntervals;
        private int currentFrame = 0;
        private static object locker = new object();
        private volatile IntPtr timerPtr = IntPtr.Zero;
        
        [Browsable(true)]
        [EditorBrowsable(EditorBrowsableState.Always)]
        public Image GIF
        {
            get { return gif; }
            set
            {
                lock (locker)
                {
                    Stop();
                    gif = value;

                    if(gif != null)
                        LoadGif();
                }
            }
        }

        public GIFPlayer()
        {
            InitializeComponent();

            this.SetStyle(ControlStyles.AllPaintingInWmPaint |
            ControlStyles.OptimizedDoubleBuffer, true);
            // Set the timer callback
            UpdateFn = new WaitOrTimerDelegate(UpdateFrame);
        }
        private void LoadGif()
        {

            // How many frames of animation are there in total?
            frameCount = gif.GetFrameCount(FrameDimension.Time);
            // Retrieve the frame time property
            PropertyItem propItem = gif.GetPropertyItem(20736);
            int propIndex = 0;
            frameIntervals = new UInt32[frameCount];
            // Each frame can have a different timing - retrieve each of them
            for (int i = 0; i < frameCount; i++)
            {
                // NB: intervals are given in hundredths of a second, so need
                // multiplying to match the timer's millisecond interval
                frameIntervals[i] = BitConverter.ToUInt32(propItem.Value,
                    propIndex) * 10;
                // Point to the next interval stored in this property
                propIndex += 4;
            }

            // Show the first frame of the animation
            ShowFrame();
        }
        private void UpdateFrame(IntPtr lpParam, bool timerOrWaitFired)
        {
            // The timer has elapsed
            // Update the number of the frame to show next
            currentFrame = (currentFrame + 1) % frameCount;
            // Paint the frame to the panel
            ShowFrame();

            if (this.timerPtr != IntPtr.Zero)
            {
                // Re-start the timer after updating its interval to that of
                // the new frame
                ChangeTimerQueueTimer(IntPtr.Zero, this.timerPtr,
                    frameIntervals[currentFrame], 100000);
            }
        }
        private void ShowFrame()
        {
            // We need to use a lock as we cannot update the GIF at the
            // same time as it's being drawn
            lock (locker)
            {
                if(gif != null)
                    gif.SelectActiveFrame(FrameDimension.Time, currentFrame);
            }

            this.Invalidate();
        }
        public void Play()
        {
            if (timerPtr != IntPtr.Zero || gif == null)
                return;

            CreateTimerQueueTimer(out this.timerPtr, IntPtr.Zero, UpdateFn,
            IntPtr.Zero, frameIntervals[0], 100000,
            (uint)ExecuteFlags.WT_EXECUTEINIOTHREAD);
        }

        public void Stop()
        {
            if (this.timerPtr == IntPtr.Zero)
                return;

            var copy = this.timerPtr;
            this.timerPtr = IntPtr.Zero;
            DeleteTimerQueueTimer(IntPtr.Zero, copy, IntPtr.Zero);
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            base.OnPaint(pe);


            lock (locker)
            {
                if(gif != null)
                    pe.Graphics.DrawImage(gif, this.ClientRectangle);
            }
        }
        /// <summary>
        /// Limpiar los recursos que se estén usando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben desechar; false en caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                Stop();
                components.Dispose();
            }

            base.Dispose(disposing);
        }

    }
}

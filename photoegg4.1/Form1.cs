using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using pix;
namespace photoegg4._1
{
    public partial class Form1 : Form
    {
        List<Bitmap> originBitmap = new List<Bitmap>();
        pixelOperate Pixel_C = new pixelOperate();
        int Now_Bitmap = -1;
        public enum colorFunction { colorTo255, colorToGray, brightness, blurry };
        public Form1()
        {
            InitializeComponent();
        }

        private void 載入圖片ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFileDialog1.Filter = "圖片檔 (*.png;*.jpg;*.bmp;*.gif;*.tif)|*.png;*.jpg;*.bmp;*.gif;*.tif";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                Bitmap a = new Bitmap(openFileDialog1.FileName);
                originBitmap.Add(a);
                pictureBox1.Image = a;
                Now_Bitmap++;
            }
            blurry();
        }
        public void Pixel_Operate(colorFunction fun)
        {
            int func = (int)fun;
            Bitmap MyNewBmp = (Bitmap)originBitmap[Now_Bitmap];
            Rectangle MyRec = new Rectangle(0, 0, MyNewBmp.Width, MyNewBmp.Height);
            BitmapData MyBmpData = MyNewBmp.LockBits(MyRec, ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData MyBmpData2 = ((Bitmap)(MyNewBmp.Clone())).LockBits(MyRec, ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            unsafe
            {
                if (func == (int)colorFunction.colorTo255)
                    Pixel_C.colorTo255((byte*)MyBmpData.Scan0, MyNewBmp.Width, MyNewBmp.Height, 4);
                else if (func == (int)colorFunction.colorToGray)
                    Pixel_C.colorToGray((byte*)MyBmpData.Scan0, MyNewBmp.Width, MyNewBmp.Height, 4);
                else if (func == (int)colorFunction.brightness)
                    Pixel_C.brightness((byte*)MyBmpData.Scan0, MyNewBmp.Width, MyNewBmp.Height, 4, -100);
                else if (func == (int)colorFunction.blurry)
                    Pixel_C.blurry2((byte*)MyBmpData.Scan0, MyNewBmp.Width, MyNewBmp.Height, 4,30, (byte*)MyBmpData2.Scan0);
            }
            MyNewBmp.UnlockBits(MyBmpData);
        }
        private void 反向ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (Now_Bitmap < 0) return;
            Pixel_Operate(colorFunction.colorTo255);
            pictureBox1.Image = originBitmap[Now_Bitmap];
        }

        private void 詼諧ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (Now_Bitmap < 0) return;
            Pixel_Operate(colorFunction.colorToGray);
            pictureBox1.Image = originBitmap[Now_Bitmap];
        }
        private void brightness()
        {
            if (Now_Bitmap < 0) return;
            Pixel_Operate(colorFunction.brightness);
            pictureBox1.Image = originBitmap[Now_Bitmap];
        }
        private void blurry()
        {
            if (Now_Bitmap < 0) return;
            Pixel_Operate(colorFunction.blurry);
            pictureBox1.Image = originBitmap[Now_Bitmap];
        }
    }
}


  /* System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();//引用stopwatch物件
            sw.Reset();//碼表歸零
            sw.Start();//碼表開始計時
                       /////////////////////////*/

            ///////////////////////////
          /*  sw.Stop();//碼錶停止

            MessageBox.Show(sw.Elapsed.TotalMilliseconds.ToString());*/
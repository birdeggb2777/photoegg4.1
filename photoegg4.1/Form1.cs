﻿using System;
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
        /// <summary>
        /// 初始化
        /// </summary>
        public List<Bitmap> originBitmap = new List<Bitmap>();
        public pixelOperate Pixel_C = new pixelOperate();
        public int Now_Bitmap = -1;

        /// <summary>
        /// 影像操作的參數
        /// </summary>
        public int value_int_1 = 0;
        public int value_int_2 = 0;
        public int value_int_3 = 0;
        public int value_int_4 = 0;
        public int value_int_5 = 0;
        public double value_double_1 = 0;
        public double value_double_2 = 0;
        public double value_double_3 = 0;
        public double value_double_4 = 0;
        public double value_double_5 = 0;
        public bool value_bool_1 = false;
        public bool value_bool_2 = false;
        public bool value_bool_3 = false;
        public bool value_bool_4 = false;
        public bool value_bool_5 = false;

        /// <summary>
        /// 
        /// </summary>
        public bool isTemp = false;
        public enum colorFunction { colorTo255, colorToGray, brightness, blurry, HSV };
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
        }
        public void Pixel_Operate(colorFunction fun)
        {
            int func = (int)fun;
            Bitmap MyNewBmp = (Bitmap)originBitmap[Now_Bitmap];
            Bitmap MyNewBmp2 = (Bitmap)MyNewBmp.Clone();
            Rectangle MyRec = new Rectangle(0, 0, MyNewBmp.Width, MyNewBmp.Height);
            BitmapData MyBmpData = MyNewBmp.LockBits(MyRec, ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData MyBmpData2 = MyNewBmp2.LockBits(MyRec, ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            unsafe
            {
                if (func == (int)colorFunction.colorTo255)
                    Pixel_C.colorTo255((byte*)MyBmpData.Scan0, MyNewBmp.Width, MyNewBmp.Height, 4);
                else if (func == (int)colorFunction.colorToGray)
                    Pixel_C.colorToGray((byte*)MyBmpData.Scan0, MyNewBmp.Width, MyNewBmp.Height, 4);
                else if (func == (int)colorFunction.brightness)
                    Pixel_C.brightness((byte*)MyBmpData.Scan0, MyNewBmp.Width, MyNewBmp.Height, 4, value_int_1);
                else if (func == (int)colorFunction.blurry)
                    Pixel_C.blurry2((byte*)MyBmpData.Scan0, (byte*)MyBmpData2.Scan0, MyNewBmp.Width, MyNewBmp.Height, 4, value_int_1);
                else if (func == (int)colorFunction.HSV)
                    Pixel_C.ConvertHSV((byte*)MyBmpData.Scan0, MyNewBmp.Width, MyNewBmp.Height, value_int_1, value_int_2, value_int_3, 4, value_bool_1, value_int_4);
            }
            MyNewBmp.UnlockBits(MyBmpData);
            MyNewBmp2.UnlockBits(MyBmpData2);
        }
        public void Pixel_Operate_Temp(colorFunction fun)
        {
            int func = (int)fun;
            Bitmap MyNewBmp = (Bitmap)originBitmap[Now_Bitmap];
            Bitmap MyNewBmp2 = (Bitmap)MyNewBmp.Clone();
            Bitmap TempBmp = (Bitmap)MyNewBmp.Clone();
            Rectangle MyRec = new Rectangle(0, 0, MyNewBmp.Width, MyNewBmp.Height);
            BitmapData MyBmpData = MyNewBmp.LockBits(MyRec, ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData MyBmpData2 = MyNewBmp2.LockBits(MyRec, ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData MyBmpData3 = TempBmp.LockBits(MyRec, ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            unsafe
            {
                if (func == (int)colorFunction.colorTo255)
                    Pixel_C.colorTo255((byte*)MyBmpData3.Scan0, MyNewBmp.Width, MyNewBmp.Height, 4);
                else if (func == (int)colorFunction.colorToGray)
                    Pixel_C.colorToGray((byte*)MyBmpData3.Scan0, MyNewBmp.Width, MyNewBmp.Height, 4);
                else if (func == (int)colorFunction.brightness)
                    Pixel_C.brightness((byte*)MyBmpData3.Scan0, MyNewBmp.Width, MyNewBmp.Height, 4, value_int_1);
                else if (func == (int)colorFunction.blurry)
                    Pixel_C.blurry2((byte*)MyBmpData3.Scan0, (byte*)MyBmpData2.Scan0, MyNewBmp.Width, MyNewBmp.Height, 4, value_int_1);
                else if (func == (int)colorFunction.HSV)
                    Pixel_C.ConvertHSV((byte*)MyBmpData3.Scan0, MyNewBmp.Width, MyNewBmp.Height, value_int_1, value_int_2, value_int_3, 4, value_bool_1, value_int_4);
            }
            MyNewBmp.UnlockBits(MyBmpData);
            MyNewBmp2.UnlockBits(MyBmpData2);
            TempBmp.UnlockBits(MyBmpData3);
            pictureBox1.Image = TempBmp;
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
        public void brightness(bool istemp)
        {
            if (Now_Bitmap < 0) return;
            if (istemp == false)
            {
                Pixel_Operate(colorFunction.brightness);
                pictureBox1.Image = originBitmap[Now_Bitmap];
            }
            else
            {
                Pixel_Operate_Temp(colorFunction.brightness);
            }
        }
        public void blurry(bool istemp)
        {
            if (Now_Bitmap < 0) return;
            if (istemp == false)
            {
                Pixel_Operate(colorFunction.blurry);
                pictureBox1.Image = originBitmap[Now_Bitmap];
            }
            else
            {
                Pixel_Operate_Temp(colorFunction.blurry);
            }
        }
        public void HSV(bool istemp)
        {
            if (Now_Bitmap < 0) return;
            if (istemp == false)
            {
                Pixel_Operate(colorFunction.HSV);
                pictureBox1.Image = originBitmap[Now_Bitmap];
            }
            else
            {
                Pixel_Operate_Temp(colorFunction.HSV);
            }
        }

        private void 亮度ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            brightnessForm form = new brightnessForm(this);
            form.Show();
        }

        private void 模糊ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            blurryForm form = new blurryForm(this);
            form.Show();
        }

        private void HSVToolStripMenuItem_Click(object sender, EventArgs e)
        {
            HSVForm form = new HSVForm(this);
            form.Show();
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

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
        List<Bitmap> originBitmap=new List<Bitmap>();
        pixelOperate Pixel_C = new pixelOperate();
        int Now_Bitmap = -1;
        public enum colorFunction { colorTo255,colorToGray};
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
            Rectangle MyRec = new Rectangle(0, 0, MyNewBmp.Width, MyNewBmp.Height);
            BitmapData MyBmpData = MyNewBmp.LockBits(MyRec, ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);   
            unsafe
            {
                if (func==0)
                    Pixel_C.colorTo255((byte*)MyBmpData.Scan0, MyNewBmp.Width, MyNewBmp.Height,4);
                else if (func==1)
                    Pixel_C.colorToGray((byte*)MyBmpData.Scan0, MyNewBmp.Width, MyNewBmp.Height, 4);

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
    }
}

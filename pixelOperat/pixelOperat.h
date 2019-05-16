#pragma once
#include <math.h>  
using namespace System;

namespace pix {

	public ref class pixelOperate
	{

		// TODO: 請在此新增此類別的方法。
	public:
		/*	void colorTo255(unsigned char*, int, int, int);
			void BGRToHSV(unsigned char, unsigned char, unsigned char);
			void  HSVToBGR(double, double, double, unsigned char, unsigned char, unsigned char);*/
		void inline colorTo255(unsigned char* ptr, int width, int height, int channel)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					fp[y][x] = 255 - fp[y][x];
					fp[y][x + 1] = 255 - fp[y][x + 1];
					fp[y][x + 2] = 255 - fp[y][x + 2];
				}
			}
			delete[] fp;

		}

		void  inline colorToGray(unsigned char* ptr, int width, int height, int channel)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					fp[y][x] = (0.299 * fp[y][x + 2] + 0.587 * fp[y][x + 1] + 0.114 * fp[y][x]);
					fp[y][x + 1] = fp[y][x];
					fp[y][x + 2] = fp[y][x];
				}
			}
			delete[] fp;
		}
		void  inline brightness(unsigned char* ptr, int width, int height, int channel, int value)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			if (value > 0)
			{
				for (y = 0; y < height; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						fp[y][x] = fp[y][x] + value > 255 ? 255 : fp[y][x] + value;
						fp[y][x + 1] = fp[y][x + 1] + value > 255 ? 255 : fp[y][x + 1] + value;
						fp[y][x + 2] = fp[y][x + 2] + value > 255 ? 255 : fp[y][x + 2] + value;
					}
				}
			}
			else
			{
				for (y = 0; y < height; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						fp[y][x] = fp[y][x] + value < 0 ? 0 : fp[y][x] + value;
						fp[y][x + 1] = fp[y][x + 1] + value < 0 ? 0 : fp[y][x + 1] + value;
						fp[y][x + 2] = fp[y][x + 2] + value < 0 ? 0 : fp[y][x + 2] + value;
					}
				}
			}
			delete[] fp;
		}
		void  inline blurry(unsigned char* ptr, int width, int height, int channel, int value)
		{
			unsigned char** fp = new unsigned char* [height];
			const int recSize = ((value * 2 + 1) * (value * 2 + 1));
			const int recWidth = value * channel;
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					for (y2 = -value; y2 <= value; y2++)
					{
						for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							countB += fp[y + y2][x + x2];
							countG += fp[y + y2][x + x2 + 1];
							countR += fp[y + y2][x + x2 + 2];
						}
					}
					fp[y][x] = countB / recSize;
					fp[y][x + 1] = countG / recSize;
					fp[y][x + 2] = countR / recSize;
					countB = countG = countR = 0;
				}
			}
			delete[] fp;
		}

		void  inline blurry2(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int value)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			const int recSize = ((value * 2 + 1) * (value * 2 + 1));
			const int recWidth = value * channel;
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			int reduceRecSize = 0;
			int reduceRecSize1 = 0;
			int reduceRecSize2 = 0;
			int boxSize = 0;
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					if (x == 0)
					{
						for (y2 = -value; y2 <= value; y2++)
						{
							for (x2 = -recWidth; x2 <= recWidth; x2 += channel)
							{
								if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								{
									reduceRecSize2++;
									continue;
								}
								countB += fp2[y + y2][x + x2];
								countG += fp2[y + y2][x + x2 + 1];
								countR += fp2[y + y2][x + x2 + 2];
							}
						}
						fp[y][x] = countB / (recSize - reduceRecSize2);
						fp[y][x + 1] = countG / (recSize - reduceRecSize2);
						fp[y][x + 2] = countR / (recSize - reduceRecSize2);
						reduceRecSize2 = 0;
					}
					else
					{
						for (y2 = -value, x2 = -recWidth - channel; y2 <= value; y2++)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
							{
								reduceRecSize1++;
								continue;
							}
							countB -= fp2[y + y2][x + x2];
							countG -= fp2[y + y2][x + x2 + 1];
							countR -= fp2[y + y2][x + x2 + 2];
						}
						for (y2 = -value, x2 = recWidth; y2 <= value; y2++)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
							{
								reduceRecSize++;
								continue;
							}
							countB += fp2[y + y2][x + x2];
							countG += fp2[y + y2][x + x2 + 1];
							countR += fp2[y + y2][x + x2 + 2];
						}
						fp[y][x] = countB / (recSize + reduceRecSize - reduceRecSize1);
						fp[y][x + 1] = countG / (recSize + reduceRecSize - reduceRecSize1);
						fp[y][x + 2] = countR / (recSize + reduceRecSize - reduceRecSize1);
					}
					reduceRecSize = 0;
					reduceRecSize1 = 0;
				}
				countB = countG = countR = 0;
				reduceRecSize2 = 0;
			}
			delete[] fp;
		}

		void ConvertHSV(unsigned char* ptr, int width, int height, int H, int S, int V, int channel)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					BGRToHSV(H, S, V, fp[y][x], fp[y][x + 1], fp[y][x + 2]);
				}
			}
			delete[] fp;
		}
		/////////////////////////////
		//////////////////////////////

		static double HSVMin(double a, double b) {
			return a <= b ? a : b;
		}

		static double HSVMax(double a, double b) {
			return a >= b ? a : b;
		}

		void BGRToHSV(int H, int S, int V, unsigned char& colorB, unsigned char& colorG, unsigned char& colorR)
		{
			double delta, min;
			double h = 0, s, v;
			min = HSVMin(HSVMin(colorR, colorG), colorB);
			v = HSVMax(HSVMax(colorR, colorG), colorB);
			delta = v - min;
			if (v == 0.0)
				s = 0;
			else
				s = delta / v;
			if (s == 0)
				h = 0.0;
			else
			{
				if (colorR == v)
					h = (colorG - colorB) / delta;
				else if (colorG == v)
					h = 2 + (colorB - colorR) / delta;
				else if (colorB == v)
					h = 4 + (colorR - colorG) / delta;
				h *= 60;
				if (h < 0.0)
					h = h + 360;
			}
			h += H;
			s += s * S / 100;
			if (s > 1.0) s = 1.0;
			if (s < 0) s = 0;
			v += V;
			if (v > 255) v = 255;
			if (v < 0) v = 0;
			HSVToBGR(h, s, v, colorB, colorG, colorR);
		}

		void  HSVToBGR(double H, double S, double V, unsigned char& colorB, unsigned char& colorG, unsigned char& colorR)
		{
			if (S == 0)
			{
				colorR = V;
				colorG = V;
				colorB = V;
			}
			else
			{
				int i;
				double f, p, q, t;

				if (H == 360)
					H = 0;
				else
					H = H / 60;

				i = (int)trunc(H);
				f = H - i;

				p = V * (1.0 - S);
				q = V * (1.0 - (S * f));
				t = V * (1.0 - (S * (1.0 - f)));

				switch (i)
				{
				case 0:
					colorR = V;
					colorG = t;
					colorB = p;
					break;

				case 1:
					colorR = q;
					colorG = V;
					colorB = p;
					break;
				case 2:
					colorR = p;
					colorG = V;
					colorB = t;
					break;

				case 3:
					colorR = p;
					colorG = q;
					colorB = V;
					break;

				case 4:
					colorR = t;
					colorG = p;
					colorB = V;
					break;

				default:
					colorR = V;
					colorG = p;
					colorB = q;
					break;
				}
			}
		}
	};
};

/* //淺綠特效
void blurry(unsigned char* ptr, int width, int height, int channel, int value)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					for (y2 = -value; y2 <= value; y2++)
					{
						for (x2 = -value; x2 <= value; x2++)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							countB += fp[y+y2][x+x2];
							countG += fp[y+y2][x+x2+1];
							countR += fp[y+y2][x+x2+2];
						}
					}
					fp[y][x] = countB / ((value*2+1)* (value * 2 + 1));
					fp[y][x + 1] = countG / ((value * 2 + 1) * (value * 2 + 1));
					fp[y][x + 2] = countR / ((value * 2 + 1) * (value * 2 + 1));
					countB = countG = countR = 0;
				}
			}
			delete[] fp;

		}
*/
/*//墨綠特效
void blurry(unsigned char* ptr, int width, int height, int channel, int value)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					for (y2 = -value; y2 <= value; y2++)
					{
						for (x2 = -value; x2 <= value*channel; x2+=channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							countB += fp[y + y2][x + x2];
							countG += fp[y + y2][x + x2 + 1];
							countR += fp[y + y2][x + x2 + 2];
						}
					}
					fp[y][x] = countB / ((value * 2 + 1) * (value * 2 + 1));
					fp[y][x + 1] = countG / ((value * 2 + 1) * (value * 2 + 1));
					fp[y][x + 2] = countR / ((value * 2 + 1) * (value * 2 + 1));
					countB = countG = countR = 0;
				}
			}
			delete[] fp;

		}
*/
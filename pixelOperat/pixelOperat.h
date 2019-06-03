﻿#pragma once
#include <math.h> 
#define BGR2BGR 0
#define BGR2RGB 1
#define BGR2RBG 2
#define BGR2GRB 3
#define BGR2GBR 4
#define BGR2BRG 5
#include <thread>
#include <iostream>  
using namespace System;
using namespace std;
namespace pix {

	public ref class pixelOperate
	{
		const double PI = 3.141592653589793238463;
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
		void horizontalFlip(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					fp[y][x] = fp2[y][Stride - x];
					fp[y][x + 1] = fp2[y][Stride - x + 1];
					fp[y][x + 2] = fp2[y][Stride - x + 2];
				}
			}
		}
		void verticalFlip(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					fp[y][x] = fp2[(height - 1) - y][x];
					fp[y][x + 1] = fp2[(height - 1) - y][x + 1];
					fp[y][x + 2] = fp2[(height - 1) - y][x + 2];
				}
			}

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
		void pasteImage(unsigned char* ptr, unsigned char* ptr2, int width, int height, int width2, int height2, int pasteX, int pasteY, int channel)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height2];
			int Stride = width * channel, x = 0, y = 0;
			int Stride2 = width2 * channel;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height2; j++)
				fp2[j] = ptr2 + (Stride2 * j);
			int x2 = 0; int y2 = 0;
			int pasteXPoint = pasteX * channel;
			for (y = 0, y2 = 0; y < height, y2 < height2; y++, y2++)
			{
				for (x = 0, x2 = 0; x < Stride, x2 < Stride2; x += channel, x2 += channel)
				{
					if (x + pasteXPoint >= Stride || x + pasteXPoint < 0 || y + pasteY >= height || y + pasteY < 0)continue;
					if (x2 >= Stride2 || x2 < 0 || y2 >= height2 || y2 < 0)continue;
					fp[y + pasteY][x + pasteXPoint] = fp2[y2][x2];
					fp[y + pasteY][x + pasteXPoint + 1] = fp2[y2][x2 + 1];
					fp[y + pasteY][x + pasteXPoint + 2] = fp2[y2][x2 + 2];
				}
			}
			delete[] fp;
			delete[] fp2;
		}
		void tile(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int value)
		{
			int value2 = 5;
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			const int recSize = value * value;
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			int x2 = 0; int y2 = 0;
			int B = 0;
			int G = 0;
			int R = 0;
			for (y = 0; y < height; y += value)
			{
				for (x = 0; x < Stride; x += channel * value)
				{
					for (y2 = 0; y2 <= value; y2++)
					{
						for (x2 = 0; x2 <= value2 * channel; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							B = fp[y + y2][x + x2] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
							G = fp[y + y2][x + x2 + 1] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
							R = fp[y + y2][x + x2 + 2] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
							if (B > 255)B = 255;
							if (B < 0) B = 0;
							if (G > 255)G = 255;
							if (G < 0) G = 0;
							if (R > 255)R = 255;
							if (R < 0) R = 0;
							fp[y + y2][x + x2] = B;
							fp[y + y2][x + x2 + 1] = G;
							fp[y + y2][x + x2 + 2] = R;
						}
					}
					for (y2 = 0; y2 <= value2; y2++)
					{
						for (x2 = 0; x2 <= value * channel; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							B = fp[y + y2][x + x2] * (y2 ) / value2 + 255 * (value2 - y2 ) / value2;
							G = fp[y + y2][x + x2 + 1] * (y2 ) / value2 + 255 * (value2 - y2 ) / value2;
							R = fp[y + y2][x + x2 + 2] * (y2 ) / value2 + 255 * (value2 - y2 ) / value2;
							if (B > 255)B = 255;
							if (B < 0) B = 0;
							if (G > 255)G = 255;
							if (G < 0) G = 0;
							if (R > 255)R = 255;
							if (R < 0) R = 0;
							fp[y + y2][x + x2] = B;
							fp[y + y2][x + x2 + 1] = G;
							fp[y + y2][x + x2 + 2] = R;

						}
					}
					for (y2 = 0; y2 <= value; y2++)
					{
						for (x2 = (value-value2) * channel;x2<=value*channel; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							int temptra = x2 - ((value-value2) * channel);
							B = fp[y + y2][x + x2] *   (value2- temptra/channel)/value2 +  (temptra / channel) / value2;
							G = fp[y + y2][x + x2 + 1] * (value2 - temptra / channel) / value2 + (temptra / channel) / value2;
							R = fp[y + y2][x + x2 + 2] * (value2 - temptra / channel) / value2 +  (temptra / channel) / value2;
							if (B > 255)B = 255;
							if (B < 0) B = 0;
							if (G > 255)G = 255;
							if (G < 0) G = 0;
							if (R > 255)R = 255;
							if (R < 0) R = 0;
							fp[y + y2][x + x2] = B;
							fp[y + y2][x + x2 + 1] = G;
							fp[y + y2][x + x2 + 2] = R;
						}
					}
					for (y2 = (value - value2); y2 <= value; y2++)
					{
						for (x2 = 0; x2 <= value * channel; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							int temptra =( y2 - (value - value2)) * channel;
							B = fp[y + y2][x + x2] * (value2 - temptra / channel) / value2 + (temptra / channel) / value2;
							G = fp[y + y2][x + x2 + 1] * (value2 - temptra / channel) / value2 + (temptra / channel) / value2;
							R = fp[y + y2][x + x2 + 2] * (value2 - temptra / channel) / value2 + (temptra / channel) / value2;
							if (B > 255)B = 255;
							if (B < 0) B = 0;
							if (G > 255)G = 255;
							if (G < 0) G = 0;
							if (R > 255)R = 255;
							if (R < 0) R = 0;
							fp[y + y2][x + x2] = B;
							fp[y + y2][x + x2 + 1] = G;
							fp[y + y2][x + x2 + 2] = R;
						}
					}

				}
			}
		}
		void mosaic(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int value)
		{
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			const int recSize = value * value;
			//const int recWidth = value * channel;
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			int x2 = 0; int y2 = 0;
			int countB = 0;
			int countG = 0;
			int countR = 0;
			for (y = 0; y < height; y += value)
			{
				for (x = 0; x < Stride; x += channel * value)
				{
					for (y2 = 0; y2 < value; y2++)
					{
						for (x2 = 0; x2 < value * channel; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							countB += fp2[y + y2][x + x2];
							countG += fp2[y + y2][x + x2 + 1];
							countR += fp2[y + y2][x + x2 + 2];
						}
					}
					countB /= recSize;
					countG /= recSize;
					countR /= recSize;
					for (y2 = 0; y2 < value; y2++)
					{
						for (x2 = 0; x2 < value * channel; x2 += channel)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
								continue;
							fp[y + y2][x + x2] = countB;
							fp[y + y2][x + x2 + 1] = countG;
							fp[y + y2][x + x2 + 2] = countR;
						}
					}
					countB = countG = countR = 0;
				}
			}
			delete[] fp;
		}
		void emboss(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int pasteY, int pasteX, bool isgray)
		{
			//int pasteY = 5;
			//int pasteX = 5;
			unsigned char** fp = new unsigned char* [height];
			unsigned char** fp2 = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (int j = 0; j < height; j++)
				fp2[j] = ptr2 + (Stride * j);
			int x2 = 0; int y2 = 0;
			int pasteXPoint = pasteX * channel;
			unsigned char gray = 0;
			int bb = 0, gg = 0, rr = 0;
			int tempB = 0, tempG = 0, tempR = 0;
			if (isgray == false)
			{
				for (y = 0; y < height; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						if (x + pasteXPoint >= Stride || x + pasteXPoint < 0 || y + pasteY >= height || y + pasteY < 0)continue;
						gray = (fp2[y + pasteY][x + pasteXPoint] + fp2[y + pasteY][x + pasteXPoint + 1] + fp2[y + pasteY][x + pasteXPoint + 2]) / 3;
						tempB = 255 - (gray);
						tempG = 255 - (gray);
						tempR = 255 - (gray);
						bb = ((tempB)+fp[y][x]) / 2;
						gg = ((tempG)+fp[y][x + 1]) / 2;
						rr = ((tempR)+fp[y][x + 2]) / 2;
						gray = (bb + gg + rr) / 3;
						bb = bb * 2 - gray;
						gg = gg * 2 - gray;
						rr = rr * 2 - gray;
						if (rr > 255)rr = 255;
						if (gg > 255)gg = 255;
						if (bb > 255)bb = 255;
						if (rr < 0)rr = 0;
						if (gg < 0)gg = 0;
						if (bb < 0)bb = 0;
						fp[y][x] = bb;
						fp[y][x + 1] = gg;
						fp[y][x + 2] = rr;
					}
				}
			}
			else
			{
				for (y = 0; y < height; y++)
				{
					for (x = 0; x < Stride; x += channel)
					{
						if (x + pasteXPoint >= Stride || x + pasteXPoint < 0 || y + pasteY >= height || y + pasteY < 0)continue;
						fp[y + pasteY][x + pasteXPoint] = ((255 - fp2[y][x]) + fp[y + pasteY][x + pasteXPoint]) / 2;
						fp[y + pasteY][x + pasteXPoint + 1] = ((255 - fp2[y][x + 1]) + fp[y + pasteY][x + pasteXPoint + 1]) / 2;
						fp[y + pasteY][x + pasteXPoint + 2] = ((255 - fp2[y][x + 2]) + fp[y + pasteY][x + pasteXPoint + 2]) / 2;
					}
				}
			}
			delete[] fp;
			delete[] fp2;
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
		static void blurry3B(unsigned char** fp, unsigned char** fp2, int width, int height, int channel, int value)
		{
			int Stride = width * channel;
			const int recSize = ((value * 2 + 1) * (value * 2 + 1));
			const int recWidth = value * channel;
			int x = 0; int y = 0;
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
							}
						}
						fp[y][x] = countB / (recSize - reduceRecSize2);
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
						}
						for (y2 = -value, x2 = recWidth; y2 <= value; y2++)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
							{
								reduceRecSize++;
								continue;
							}
							countB += fp2[y + y2][x + x2];
						}
						fp[y][x] = countB / (recSize + reduceRecSize - reduceRecSize1);
					}
					reduceRecSize = 0;
					reduceRecSize1 = 0;
				}
				countB = countG = countR = 0;
				reduceRecSize2 = 0;
			}
		}
		static void blurry3G(unsigned char** fp, unsigned char** fp2, int width, int height, int channel, int value)
		{
			int Stride = width * channel;
			const int recSize = ((value * 2 + 1) * (value * 2 + 1));
			const int recWidth = value * channel;
			int x = 0; int y = 0;
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
								countG += fp2[y + y2][x + x2 + 1];
							}
						}
						fp[y][x + 1] = countG / (recSize - reduceRecSize2);
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
							countG -= fp2[y + y2][x + x2 + 1];
						}
						for (y2 = -value, x2 = recWidth; y2 <= value; y2++)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
							{
								reduceRecSize++;
								continue;
							}
							countG += fp2[y + y2][x + x2 + 1];
						}
						fp[y][x + 1] = countG / (recSize + reduceRecSize - reduceRecSize1);
					}
					reduceRecSize = 0;
					reduceRecSize1 = 0;
				}
				countB = countG = countR = 0;
				reduceRecSize2 = 0;
			}
		}
		static void blurry3R(unsigned char** fp, unsigned char** fp2, int width, int height, int channel, int value)
		{
			int Stride = width * channel;
			const int recSize = ((value * 2 + 1) * (value * 2 + 1));
			const int recWidth = value * channel;
			int x = 0; int y = 0;
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
								countR += fp2[y + y2][x + x2 + 2];
							}
						}
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
							countR -= fp2[y + y2][x + x2 + 2];
						}
						for (y2 = -value, x2 = recWidth; y2 <= value; y2++)
						{
							if (y + y2 < 0 || y + y2 >= height || x + x2 < 0 || x + x2 >= Stride)
							{
								reduceRecSize++;
								continue;
							}
							countR += fp2[y + y2][x + x2 + 2];
						}
						fp[y][x + 2] = countR / (recSize + reduceRecSize - reduceRecSize1);
					}
					reduceRecSize = 0;
					reduceRecSize1 = 0;
				}
				countB = countG = countR = 0;
				reduceRecSize2 = 0;
			}
		}
		void  inline blurry3(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int value)
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
			thread ThreadB(blurry3B,fp,fp2,width,height,channel,value);
			thread ThreadG(blurry3G, fp, fp2, width, height, channel, value);
			thread ThreadR(blurry3R, fp, fp2, width, height, channel, value);
			ThreadB.join();
			ThreadG.join();
			ThreadR.join();		
			delete[] fp;
			delete[] fp2;
		}
		void colorOrder(unsigned char& b, unsigned char& g, unsigned char& r, int order)
		{
			unsigned char temp;
			if (order == BGR2BGR)
			{
				return;
			}
			else if (order == BGR2BRG)
			{
				temp = g;
				g = r;
				r = temp;
			}
			else if (order == BGR2RGB)
			{
				temp = b;
				b = r;
				r = temp;
			}
			else if (order == BGR2RBG)
			{
				temp = b;
				b = g;
				g = temp;
				temp = r;
				r = g;
				g = temp;
			}
			else if (order == BGR2GRB)
			{
				temp = b;
				b = r;
				r = temp;
				temp = g;
				g = r;
				r = temp;
			}
			else if (order == BGR2GBR)
			{
				temp = b;
				b = g;
				g = temp;
			}
		}
		void ConvertHSV(unsigned char* ptr, int width, int height, int H, int S, int V, int channel, bool fix, int order)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					colorOrder(fp[y][x], fp[y][x + 1], fp[y][x + 2], order);
					BGRToHSV(H, S, V, fp[y][x], fp[y][x + 1], fp[y][x + 2], fix);
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

		void BGRToHSV(int H, int S, int V, unsigned char& colorB, unsigned char& colorG, unsigned char& colorR, bool fix)
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
			if (fix == false)
				h += H;
			else
				h = H;
			if (h < 0.0)
				h = h + 360;
			if (h >= 360.0)
				h = h - 360;
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
/*麵包版
void tile(unsigned char* ptr, unsigned char* ptr2, int width, int height, int channel, int value)
{
	int value2 = 5;
	unsigned char** fp = new unsigned char* [height];
	unsigned char** fp2 = new unsigned char* [height];
	const int recSize = value * value;
	int Stride = width * channel, x = 0, y = 0;
	for (int j = 0; j < height; j++)
		fp[j] = ptr + (Stride * j);
	for (int j = 0; j < height; j++)
		fp2[j] = ptr2 + (Stride * j);
	int x2 = 0; int y2 = 0;
	int B = 0;
	int G = 0;
	int R = 0;
	for (y = 0; y < height; y += value)
	{
		for (x = 0; x < Stride; x += channel * value)
		{
			for (y2 = 0; y2 < value; y2++)
			{
				for (x2 = 0; x2 < value2 * channel; x2 += channel)
				{
					fp[y + y2][x + x2] = fp2[y + y2][x + x2] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
					fp[y + y2][x + x2 + 1] = fp2[y + y2][x + x2 + 1] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
					fp[y + y2][x + x2 + 2] = fp2[y + y2][x + x2 + 2] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
				}
			}
			for (y2 = 0; y2 < value2; y2++)
			{
				for (x2 = 0; x2 < value * channel; x2 += channel)
				{
					B = fp2[y + y2][x + x2] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
					G = fp2[y + y2][x + x2 + 1] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
					R = fp2[y + y2][x + x2 + 2] * (x2 / channel) / value2 + 255 * (value2 - x2 / 4) / value2;
					if (B > 255)B = 255;
					if (B < 0) B = 0;
					if (G > 255)G = 255;
					if (G < 0) G = 0;
					if (R > 255)R = 255;
					if (R < 0) R = 0;
					fp[y + y2][x + x2] = B;
					fp[y + y2][x + x2 + 1] = G;
					fp[y + y2][x + x2 + 2] = R;

				}
			}
		}
	}*/
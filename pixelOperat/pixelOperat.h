#pragma once

using namespace System;

namespace pix{
	public ref class pixelOperate
	{
		// TODO: 請在此新增此類別的方法。
	public :
		void colorTo255(unsigned char* ptr, int width, int height, int channel)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel,x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride *j);	
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					fp[y][x] = 255- fp[y][x];
					fp[y][x + 1] = 255 - fp[y][x+1];
					fp[y][x + 2] = 255 - fp[y][x+2];
				}
			}
			delete[] fp;
			
		}
		
		void colorToGray(unsigned char* ptr, int width, int height, int channel)
		{
			unsigned char** fp = new unsigned char* [height];
			int Stride = width * channel, x = 0, y = 0;
			for (int j = 0; j < height; j++)
				fp[j] = ptr + (Stride * j);
			for (y = 0; y < height; y++)
			{
				for (x = 0; x < Stride; x += channel)
				{
					fp[y][x] =( 0.299 * fp[y][x+2] + 0.587 * fp[y][x+1] + 0.114 * fp[y][x]);
					fp[y][x + 1] = fp[y][x];
					fp[y][x + 2] = fp[y][x];
				}
			}
			delete[] fp;
		}
	};
}

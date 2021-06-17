using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;

namespace EdgeDetection
{
	public unsafe class FastBitmap
	{
		public struct PixelData
		{
			public byte Blue;
			public byte Green;
			public byte Red;
		}

		public enum ColorByte
		{
			Blue = 0,
			Green = 1,
			Red = 2
		}

		Bitmap m_Bitmap;
		BitmapData m_BmpData;
		Byte* m_pBase;

		public FastBitmap(Bitmap bmp)
		{
			m_Bitmap = bmp;
		}
		
		public void LockBitmap()
		{
			Rectangle rect = new Rectangle(0, 0, m_Bitmap.Width, m_Bitmap.Height);
			m_BmpData = m_Bitmap.LockBits(rect, ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
			m_pBase = (Byte*)m_BmpData.Scan0.ToPointer();
		}

		public PixelData GetPixel(int x, int y)
		{
			PixelData returnValue = *(PixelData*)(m_pBase + y * m_BmpData.Stride + x * sizeof(PixelData));
			return returnValue;
		}

		public void SetPixel(int x, int y, PixelData color)
		{
			PixelData* pixel = (PixelData*)(m_pBase + y * m_BmpData.Stride + x * sizeof(PixelData));
			*pixel = color;
		}

		public byte GetByte(int x, int y, ColorByte color)
		{
			return *(byte*)(m_pBase + y * m_BmpData.Stride + x * sizeof(PixelData) + (byte)color);
		}

		public void SetByte(int x, int y, ColorByte color, byte newVal)
		{
			byte* cByte = (byte*)(m_pBase + y * m_BmpData.Stride + x * sizeof(PixelData) + (byte)color);
			*cByte = newVal;
		}

		public void UnlockBitmap()
		{
			m_Bitmap.UnlockBits(m_BmpData);
			m_BmpData = null;
			m_Bitmap = null;
			m_pBase = null;
		}
	}
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using hexcell;
using System.Drawing.Imaging;

namespace EdgeDetection
{
	public partial class Form1 : Form
	{
		Bitmap m_Bmp;

		public Form1()
		{
			InitializeComponent();
		}

		private void openToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (openFileDialog1.ShowDialog() == DialogResult.OK)
			{
				m_Bmp = new Bitmap(openFileDialog1.FileName);
				ImageBox.Image = m_Bmp;
			}
		}

		private void saveToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (saveFileDialog1.ShowDialog() == DialogResult.OK)
			{
				m_Bmp.Save(saveFileDialog1.FileName);
			}
		}

		private void quitToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Close();
		}

		private void laplaceSquareToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Bitmap target = new Bitmap(m_Bmp);
			FastBitmap fastSource = new FastBitmap(m_Bmp);
			fastSource.LockBitmap();
			FastBitmap fastTarget = new FastBitmap(target);
			fastTarget.LockBitmap();
			
			for (int x = 1; x < m_Bmp.Width-1; x++ )
				for (int y = 1; y < m_Bmp.Height - 1; y++)
				{
					int sum = 0;
					FastBitmap.PixelData pxl;
					for(int i = -1; i < 2; i++)
						for (int j = -1; j < 2; j++)
						{
							pxl = fastSource.GetPixel(x + i, y + j);
							if((i == 0) || (j == 0))
								sum += (pxl.Blue + pxl.Red + pxl.Green) * 2;
							else
								sum += (pxl.Blue + pxl.Red + pxl.Green);
						}

					pxl = fastSource.GetPixel(x, y);
					sum -= (pxl.Blue + pxl.Red + pxl.Green) * 14;
					sum /= 24;

					pxl.Red = (byte)(sum + 127);
					pxl.Green = (byte)(sum + 127);
					pxl.Blue = (byte)(sum + 127);
					fastTarget.SetPixel(x, y, pxl);

				}

			fastTarget.UnlockBitmap();
			fastSource.UnlockBitmap();
			m_Bmp = target;
			ImageBox.Image = m_Bmp;
		}

		Color CombineColor(Color[] colors, double[] weights)
		{
			double red = 0;
			double green = 0;
			double blue = 0;
			for (int i = 0; i < colors.Length; i++)
			{
				red += colors[i].R * weights[i];
				green += colors[i].G * weights[i];
				blue += colors[i].B * weights[i];
			}
			return Color.FromArgb(Math.Min((int)red, 255), Math.Min((int)green, 255), Math.Min((int)blue, 255));
		}

		private Color[,] GetColorArray(Bitmap bmp)
		{
			Rectangle rect = new Rectangle(0, 0, bmp.Width, bmp.Height);
			BitmapData bmpData = bmp.LockBits(rect, ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);

			// Get the address of the first line.
			IntPtr ptr = bmpData.Scan0;
			int[] rgbValues = new int[bmp.Width * bmp.Height];
			Color[,] result = new Color[bmp.Height, bmp.Width];
			// Copy the RGB values into the array.
			System.Runtime.InteropServices.Marshal.Copy(ptr, rgbValues, 0, bmp.Width * bmp.Height);
			bmp.UnlockBits(bmpData);

			// Create a Color array
			for (int y = 0; y < bmp.Height; y++)
				for (int x = 0; x < bmp.Width; x++)
					result[y, x] = Color.FromArgb(rgbValues[y * bmp.Width + x]);


			return result;
		}

		Bitmap GetBitmapByColorArray(Color[,] color)
		{
			int width = color.GetLength(1);
			int height = color.GetLength(0);

			Bitmap bmp = new Bitmap(width, height);

			// Lock the bitmap's bits.
			Rectangle rect = new Rectangle(0, 0, bmp.Width, bmp.Height);
			BitmapData bmpData = bmp.LockBits(rect, ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);


			// Declare an array to hold the bytes of the bitmap.
			int intCnt = bmp.Width * bmp.Height;
			int[] rgbValues = new int[intCnt];

			// Copy the RGB values into the array.
			for (int y = 0; y < height; y++)
				for (int x = 0; x < width; x++)
					rgbValues[y * width + x] = color[y, x].ToArgb();

			// Get the address of the first line.
			IntPtr ptr = bmpData.Scan0;

			// Copy the RGB values back to the bitmap
			System.Runtime.InteropServices.Marshal.Copy(rgbValues, 0, ptr, bmp.Width * bmp.Height);

			// Unlock the bits.
			bmp.UnlockBits(bmpData);

			// Create a Color array
			for (int y = 0; y < height; y++)
				for (int x = 0; x < width; x++)
					bmp.SetPixel(x, y, color[y, x]);

			return bmp;
		}

		private void laplaceHexToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Vector2D gridSize = new Vector2D((int)m_Bmp.Width, (int)m_Bmp.Height);
			DataGrid<Color> grid = new FixedSizeDataGrid<Color>((int)(gridSize.X), (int)gridSize.Y);
			HAlignedOrthogonalAdressingScheme<Color> scheme = new HAlignedOrthogonalAdressingScheme<Color>(grid);

			DataGrid<Color> targetgrid = new FixedSizeDataGrid<Color>((int)(gridSize.X), (int)gridSize.Y);
			HAlignedOrthogonalAdressingScheme<Color> targetscheme = new HAlignedOrthogonalAdressingScheme<Color>(targetgrid);


			Point topLeft = new Point(0, 0);
			Point bottomRight = new Point((int)(gridSize.X), (int)(gridSize.Y)); //VAlignedOrth

			GridConverter<Color, Color, Point>.ArrayToHexGridLinear(GetColorArray(m_Bmp), scheme, topLeft, bottomRight, CombineColor);
			
			foreach(CellPointer<Color, Point> cell in scheme.GetCompleteEnumerator())
			{
				int sum = 0;
				Color color = cell.GetData();
				foreach (CellPointer<Color, Point> neighbour in scheme.GetNeighbourhoodEnumerator(cell.GetAddress()))
				{
					Color c = neighbour.GetData();
					sum += (c.R + c.G + c.B) - (color.R + color.G + color.B);
				}

				sum /= 3 * 6;
				sum = Math.Max(Math.Min(sum, 127), -127);
				targetscheme.SetData(cell.GetAddress(),Color.FromArgb(128 + sum, 128 + sum, 128 + sum));
			}

			Color[,] result = new Color[(int)bottomRight.Y, (int)bottomRight.X];
			GridConverter<Color, Color, Point>.HexGridToArrayLinear(targetscheme, topLeft, bottomRight, result, CombineColor);
			m_Bmp = GetBitmapByColorArray(result);
			ImageBox.Image = m_Bmp;
		
		}

		private void laplace4erSquareToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Bitmap target = new Bitmap(m_Bmp);
			FastBitmap fastSource = new FastBitmap(m_Bmp);
			fastSource.LockBitmap();
			FastBitmap fastTarget = new FastBitmap(target);
			fastTarget.LockBitmap();

			for (int x = 1; x < m_Bmp.Width - 1; x++)
				for (int y = 1; y < m_Bmp.Height - 1; y++)
				{
					int sum = 0;
					FastBitmap.PixelData pxl;
					pxl = fastSource.GetPixel(x - 1, y);	
					sum += (pxl.Blue + pxl.Red + pxl.Green);
					pxl = fastSource.GetPixel(x + 1, y);
					sum += (pxl.Blue + pxl.Red + pxl.Green);
					pxl = fastSource.GetPixel(x, y - 1);
					sum += (pxl.Blue + pxl.Red + pxl.Green);
					pxl = fastSource.GetPixel(x, y + 1);
					sum += (pxl.Blue + pxl.Red + pxl.Green);
					
					pxl = fastSource.GetPixel(x, y);
					sum -= (pxl.Blue + pxl.Red + pxl.Green) * 4;
					sum /= 6;

					pxl.Red = (byte)(sum + 128);
					pxl.Green = (byte)(sum + 128);
					pxl.Blue = (byte)(sum + 128);
					fastTarget.SetPixel(x, y, pxl);

				}

			fastTarget.UnlockBitmap();
			fastSource.UnlockBitmap();
			m_Bmp = target;
			ImageBox.Image = m_Bmp;
		}
	}
}
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;
using hexcell;
using System.Runtime.InteropServices;

namespace converterLab
{
	public partial class ConverterLabForm : Form
	{
		DataGrid<Color> m_Grid;
		//VAlignedTrapezoidalAdressingScheme<Color> m_Scheme;
		HAlignedOrthogonalAdressingScheme<Color> m_Scheme;
		SimpleGridRenderer<Color, Point> m_Renderer;
		Bitmap m_Source;
		Point m_TopLeft;
		Point m_BottomRight;

		Vector2D m_MousePos;

		public ConverterLabForm()
		{
			InitializeComponent();
		}

		private void ConverterLabForm_Load(object sender, EventArgs e)
		{

		}

		private void btnLoadSource_Click(object sender, EventArgs e)
		{
			if (openFileDialog.ShowDialog() == DialogResult.OK)
			{
				m_Source = new Bitmap(openFileDialog.FileName);
				picSource.Image = m_Source;
				numGridWidth.Value = Math.Min(picSource.Image.Width, numGridWidth.Maximum);
				numGridHeight.Value = Math.Min(picSource.Image.Height, numGridHeight.Maximum) ;
			}
		}

		bool Visualize(Color col, ref CellVisualProperties vp)
		{
			vp.BorderStrength = 0;
			vp.CoreColor = col;
			return true;
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

		Color[,] GetColorArray(Bitmap bmp)
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

		private void btnConvertToHex_Click(object sender, EventArgs e)
		{
			if (m_Source == null)
				return;
			Vector2D gridSize = new Vector2D((int)numGridWidth.Value, (int)numGridHeight.Value);
			//m_Grid = new DynamicDataGrid<Color>();
			m_Grid = new FixedSizeDataGrid<Color>((int)(gridSize.X), (int)gridSize.Y);
			m_Scheme = new HAlignedOrthogonalAdressingScheme<Color>(m_Grid);

			m_TopLeft = new Point(0, 0);
			m_BottomRight = new Point((int)(gridSize.X), (int)(gridSize.Y)); //VAlignedOrth
			//m_BottomRight = new Point((int)(gridSize.X), (int)(gridSize.Y - gridSize.X / 2)); //VAlignedTrap
			//m_BottomRight = new Point((int)(gridSize.X - gridSize.Y / 2), (int)(gridSize.Y)); //HAlignedTrap
		
			
			GridView view = new GridView(gridSize);
			//foreach (Cell<Color, Point> c in m_Scheme.GetCompleteEnumerator())
			//	c.SetData(Color.Green);
			int ticks = Environment.TickCount;
			if(comboBox1.Text == "Geometric")
				GridConverter<Color, Color, Point>.ArrayToHexGridGeometric(GetColorArray(m_Source), m_Scheme, m_TopLeft, m_BottomRight, CombineColor);
			else if(comboBox1.Text == "Linear")
				GridConverter<Color, Color, Point>.ArrayToHexGridLinear(GetColorArray(m_Source), m_Scheme, m_TopLeft, m_BottomRight, CombineColor);
			else if (comboBox1.Text == "Nearest")
				GridConverter<Color, Color, Point>.ArrayToHexGridNearest(GetColorArray(m_Source), m_Scheme, m_TopLeft, m_BottomRight, delegate(Color c) { return c; });
			System.Diagnostics.Debug.Write("Converting took:" + (Environment.TickCount - ticks).ToString() + "ticks.");
			
			//visualize grid
			Vector2D rendererSize = new Vector2D(picResult.Width, picResult.Height);
			m_Renderer = new SimpleGridRenderer<Color, Point>(rendererSize, m_Scheme, Visualize);
			m_Renderer.View.CellShape.Area = (int)numCellArea.Value;

			picResult.Image = m_Renderer.Image;
		}

		private void btnCopnvertToSquare_Click(object sender, EventArgs e)
		{
			Color[,] result = new Color[(int)numGridHeight.Value, (int)numGridWidth.Value];
			int ticks = Environment.TickCount;
			if (comboBox1.Text == "Geometric")
				GridConverter<Color, Color, Point>.HexGridToArrayGeometric(m_Scheme, m_TopLeft, m_BottomRight, result, CombineColor);
			else if (comboBox1.Text == "Linear")
				GridConverter<Color, Color, Point>.HexGridToArrayLinear(m_Scheme, m_TopLeft, m_BottomRight, result, CombineColor);
			else if (comboBox1.Text == "Nearest")
				GridConverter<Color, Color, Point>.HexGridToArrayNearest(m_Scheme, m_TopLeft, m_BottomRight, result, delegate(Color c) { return c; });
			System.Diagnostics.Debug.Write("Converting took:" + (Environment.TickCount - ticks).ToString() + "ticks.");
			//Test();
			m_Source = GetBitmapByColorArray(result);
			picSource.Image = m_Source;
		}

		private void UpdateResult()
		{
			picResult.Image = m_Renderer.Image;
		}

		private void numCellArea_ValueChanged(object sender, EventArgs e)
		{
			if (m_Renderer == null)
				return;
			m_Renderer.View.CellShape.Area = (int)numCellArea.Value;
			UpdateResult();
		}

		private void picResult_MouseDown(object sender, MouseEventArgs e)
		{
			m_MousePos = new Vector2D(e.X, e.Y);
		}

		private void picResult_MouseUp(object sender, MouseEventArgs e)
		{
			if (m_Renderer == null)
				return;
			Vector2D scrollDelta = m_MousePos - new Vector2D(e.X, e.Y);
			m_Renderer.View.Scroll(scrollDelta);
			UpdateResult();
		}

		private void btnSaveSource_Click(object sender, EventArgs e)
		{
			if (saveFileDialog.ShowDialog() == DialogResult.OK)
			{
				m_Source.Save(saveFileDialog.FileName);
			}
		}

		private void groupBox1_Enter(object sender, EventArgs e)
		{

		}
		

	}
}
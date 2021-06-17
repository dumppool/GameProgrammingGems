using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using hexcell;

namespace HexLife
{
	public partial class Form1 : Form
	{
		DataGrid<SimpleLifeCell> m_Grid;
		VAlignedTrapezoidalAdressingScheme<SimpleLifeCell> m_Scheme;
		GridRenderer m_Renderer;
		Vector2D m_RenderSize;

		public Form1()
		{
			InitializeComponent();
			InitGame();
		}

		private void InitGame()
		{
			m_Grid = new FixedSizeDataGrid<SimpleLifeCell>(30, 24);
			m_Scheme = new VAlignedTrapezoidalAdressingScheme<SimpleLifeCell>(m_Grid);
			m_RenderSize = new Vector2D(ImageBox.Width, ImageBox.Height);
			//Init the cells
			Random rnd = new Random();
			foreach (ICellData<SimpleLifeCell> data in m_Scheme.GetCompleteEnumerator())
				data.SetData(new SimpleLifeCell(rnd.Next(5) == 0));

			m_Renderer = new SimpleGridRenderer<SimpleLifeCell, Point>(m_RenderSize, m_Scheme, VisualizeState);
			m_Renderer.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
			m_Renderer.View.CellShape.Radius = 10;
		}

		bool VisualizeAge(SimpleLifeCell cell, ref CellVisualProperties vp)
		{
			int c = 25 * cell.Activity;
			vp.BorderStrength = 0;
			vp.CoreColor = Color.FromArgb(c, c, c);
			return true;
		}

		bool VisualizeState(SimpleLifeCell cell, ref CellVisualProperties vp)
		{
			if (cell.Alive)
			{
				vp.BorderColor = Color.Black;
				vp.BorderStrength = 2;
				vp.CoreColor = Color.Green;
				return true;
			}
			else
				return false;
		}

		private void Timer_Tick(object sender, EventArgs e)
		{
			//Next Step
			foreach (CellPointer<SimpleLifeCell, Point> cell in m_Scheme.GetCompleteEnumerator())
				cell.GetData().CalcNextState(new StarNeighbours<SimpleLifeCell, Point>(m_Scheme, cell.GetAddress()));

			foreach (ICellData<SimpleLifeCell> data in m_Scheme.GetCompleteEnumerator())
				data.GetData().Progress();

			m_Renderer.ForceUpdate();
			ImageBox.Image = m_Renderer.Image;
		}

		private void ResetButton_Click(object sender, EventArgs e)
		{
			InitGame();
			rbState.Checked = true;
		}

		private void VisColorButton_Click(object sender, EventArgs e)
		{
			m_Renderer = new SimpleGridRenderer<SimpleLifeCell, Point>(m_RenderSize, m_Scheme, VisualizeState);
			m_Renderer.View.CellShape.Apothem = 8;
		}

		private void VisIntensityButton_Click(object sender, EventArgs e)
		{
			m_Renderer = new SimpleGridRenderer<SimpleLifeCell, Point>(m_RenderSize, m_Scheme, VisualizeAge);
			m_Renderer.View.CellShape.Apothem = 8;
		}

		private void VisBlobButton_Click(object sender, EventArgs e)
		{
			CustomGridRenderer<SimpleLifeCell, Point> renderer = new CustomGridRenderer<SimpleLifeCell, Point>(m_RenderSize, m_Scheme);
			renderer.View.CellShape.Apothem = 8;
			renderer.OnCellPaint += renderer_OnCellPaint;
			renderer.OnViewPaint += renderer_OnViewPaint;
			m_Renderer = renderer;
		}

		void renderer_OnViewPaint(GridView view, Graphics gfx)
		{
			gfx.Clear(Color.White);
		}

		void renderer_OnCellPaint(CellPointer<SimpleLifeCell, Point> cell, GridView view, Graphics gfx)
		{
			Vector2D pos = view.GetCenterPos(cell);
			float radius = cell.GetData().Activity;
			int colVal = 200 - cell.GetData().Activity * 20;
			Color col = Color.FromArgb(colVal, colVal, colVal);
			pos -= new Vector2D(radius, radius);
			gfx.FillEllipse(new SolidBrush(col), (float)pos.X, (float)pos.Y, radius*2, radius*2);
		}

		private void speedValueChanged(object sender, EventArgs e)
		{
			Timer.Interval = 5000 / speedBar.Value;
		}

		
	}
}
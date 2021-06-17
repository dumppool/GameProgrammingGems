using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using hexcell;

namespace Pathfinding
{
	public enum PathState { Start, Goal, Path, None };
		
	public class PathCell
	{
		public PathCell(bool moveable)
		{
			Moveable = moveable;
			Path = PathState.None;
			ExpandedFrom = null;
		}
		public PathCell ExpandedFrom;
		public bool Moveable;
		public PathState Path;
	}

	public partial class Form1 : Form
	{
		DataGrid<PathCell> m_Grid;
		VAlignedTrapezoidalAdressingScheme<PathCell> m_Scheme;
		GridRenderer m_Renderer;
		Vector2D m_RenderSize;
		CellPointer<PathCell, Point> m_Start;
		CellPointer<PathCell, Point> m_Goal;
	
		public Form1()
		{
			InitializeComponent();
			InitGame();
		}

		private void InitGame()
		{
			m_Grid = new FixedSizeDataGrid<PathCell>(26, 24);
			m_Scheme = new VAlignedTrapezoidalAdressingScheme<PathCell>(m_Grid);
			m_RenderSize = new Vector2D(ImageBox.Width, ImageBox.Height);
			//Init the cells
			Random rnd = new Random();
			foreach (ICellData<PathCell> data in m_Scheme.GetCompleteEnumerator())
				data.SetData(new PathCell(rnd.Next(2) != 0));

			m_Renderer = new SimpleGridRenderer<PathCell, Point>(m_RenderSize, m_Scheme, VisualizeState);
			m_Renderer.View.CellShape.Radius = 15;
			m_Renderer.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
			UpdateView();
		}

		private void UpdateView()
		{
			m_Renderer.ForceUpdate();
			ImageBox.Image = m_Renderer.Image;
		}

		bool VisualizeState(PathCell cell, ref CellVisualProperties vp)
		{	
			vp.BorderColor = Color.Black;
			vp.BorderStrength = 1;
			vp.CoreColor = Color.White;
			
			if(!cell.Moveable)
				vp.CoreColor = Color.Gray;
			else if (cell.Path == PathState.Path)
				vp.CoreColor = Color.Blue;
			else if (cell.Path == PathState.Start)
				vp.CoreColor = Color.Red;
			else if (cell.Path == PathState.Goal)
				vp.CoreColor = Color.Green;
			
			return true;
		}

		private void btnReset_Click(object sender, EventArgs e)
		{
			InitGame();
		}

		private void FindPath()
		{
			Queue<CellPointer<PathCell, Point>> openCells = new Queue<CellPointer<PathCell, Point>>();
			openCells.Enqueue(m_Start);
			
			//expand
			while (openCells.Count > 0)
			{
				CellPointer<PathCell, Point> current = openCells.Dequeue();
				foreach(CellPointer<PathCell, Point> cell in m_Scheme.GetNeighbourhoodEnumerator(current.GetAddress()))
					if(cell.IsValid() && (cell.GetData().Moveable) && (cell.GetData().ExpandedFrom == null))
					{
						cell.GetData().ExpandedFrom = current.GetData();
						openCells.Enqueue(cell);
					}
			}

			//resolve path
			PathCell pc = m_Goal.GetData().ExpandedFrom;
			while ((pc != null) && (pc != m_Start.GetData()))
			{
				pc.Path = PathState.Path;
				pc = pc.ExpandedFrom;
			}
		}

		private void Reset()
		{
			if (m_Goal != null)
				m_Goal.GetData().Path = PathState.None;
			if (m_Start != null)
				m_Start.GetData().Path = PathState.None;
			
			//init
			foreach (ICellData<PathCell> data in m_Scheme.GetCompleteEnumerator())
			{
				data.GetData().ExpandedFrom = null;
				data.GetData().Path = PathState.None;
			}
		}

		private void ImageBox_MouseClick(object sender, MouseEventArgs e)
		{
			Vector2D clickPos = new Vector2D(e.X, e.Y);
			if (m_Start == null || ((m_Start != null) && (m_Goal != null)))
			{
				Reset();
				m_Start = new CellPointer<PathCell, Point>(m_Scheme);
				m_Renderer.View.SetCellTo(m_Start, clickPos);
				if(m_Start.GetData().Moveable)
				{
					m_Goal = null;
					m_Start.GetData().Path = PathState.Start;
				}
			}
			else
			{
				m_Goal = new CellPointer<PathCell, Point>(m_Scheme);
				m_Renderer.View.SetCellTo(m_Goal, clickPos);
				if (m_Goal.GetData().Moveable)
				{
					m_Goal.GetData().Path = PathState.Goal;
					FindPath();
				}
				else
					m_Goal = null;
			}


			UpdateView();
		}

	}
}
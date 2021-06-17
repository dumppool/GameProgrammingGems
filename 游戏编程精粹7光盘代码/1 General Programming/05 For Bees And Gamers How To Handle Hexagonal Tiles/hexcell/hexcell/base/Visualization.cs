using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace hexcell
{
	public abstract class GridRenderer
	{
		protected GridView m_View;
		protected Bitmap m_Bitmap;
		protected Graphics m_Gfx;
		private bool m_Dirty;

		public GridRenderer(Vector2D size)
		{
			SetView(new GridView(size));
			m_Bitmap = new Bitmap((int)m_View.Width, (int)m_View.Height);
			m_Gfx = Graphics.FromImage(m_Bitmap);
			SetView(m_View);
			m_Dirty = true;
		}

		public SmoothingMode SmoothingMode
		{
			set { m_Gfx.SmoothingMode = value; }
			get { return m_Gfx.SmoothingMode; }
		}

		public GridView View
		{
			set { SetView(value); }
			get	{ return GetView(); }
		}
		public Bitmap Image
		{
			get{ return GetImage();	}
		}
		public bool ImageChanged //true if image returned when the Image property was last accessed isn't up to date
		{
			get { return m_Dirty; }
		}

		private Bitmap GetImage()
		{
			if (m_Dirty)
				UpdateImage();

			return m_Bitmap;
		}
		private void SetView(GridView view)
		{
			if (view == m_View)
				return; //nothing to do

			m_View = view;

			//unregister from m_View
			if (m_View != null)
				m_View.OnChange -= new GridView.GridViewEventHandler(ViewChanged);
			m_View = view;
			//register at m_View
			if (m_View != null)
				m_View.OnChange += new GridView.GridViewEventHandler(ViewChanged);
			
			m_Dirty = true;

		}
		private GridView GetView()
		{
			return m_View;
		}

		public void ForceUpdate()
		{
			UpdateImage();
		}

		protected virtual void UpdateImage()
		{
			//Verify correct size
			if ((m_Bitmap.Size.Width != m_View.Width) || (m_Bitmap.Size.Height != m_View.Height))
			{
				m_Bitmap = new Bitmap((int)m_View.Width, (int)m_View.Height);
				m_Gfx = Graphics.FromImage(m_Bitmap);
			}

			m_Dirty = false;
		}

		//react to events
		public void ViewChanged()
		{
			m_Dirty = true;
			#if DEBUG
				System.Diagnostics.Debug.Print("*** View Dirty ***");
			#endif
		}
	};

	public class CustomGridRenderer<DataType, CellAddress> : GridRenderer
	{
		protected AddressingScheme<DataType, CellAddress> m_Scheme;

		public CustomGridRenderer(Vector2D size, AddressingScheme<DataType, CellAddress> scheme)
			: base(size) 
		{
			m_Scheme = scheme;
		}

		protected override void UpdateImage()
		{
			base.UpdateImage();

			m_Gfx.Clear(Color.Crimson);

			if (OnViewPaint != null)
				OnViewPaint(m_View, m_Gfx);

			//draw each cell
			if (OnCellPaint != null)
			{
				ViewCells<DataType, CellAddress> cellsInView = m_Scheme.GetViewEnumerator(m_View);
				foreach (CellPointer<DataType, CellAddress> p in cellsInView)
					if (p.IsValid() && (OnCellPaint != null))
						OnCellPaint(p, m_View, m_Gfx);
			}
		}

		/* events */
		public delegate void CellPaintEventHandler(CellPointer<DataType, CellAddress> cell, GridView view, Graphics gfx);
		public delegate void ViewPaintEventHandler(GridView view, Graphics gfx);

		public event CellPaintEventHandler OnCellPaint;
		public event ViewPaintEventHandler OnViewPaint;

	}


	public struct CellVisualProperties
	{
		public Color BorderColor;
		public int BorderStrength;
		public Color CoreColor;
		public CellVisualProperties(Color border, int borderStrength, Color core)
		{
			BorderColor = border;
			BorderStrength = borderStrength;
			CoreColor = core;
		}
	}

	public class SimpleGridRenderer<DataType, CellAddress> : GridRenderer
	{
		public delegate bool Visualizer<TInput>(TInput input, ref CellVisualProperties output);

		protected Color m_BgColor;
		public Color Background
		{
			get { return m_BgColor; }
			set { m_BgColor = Background; }
		}

		protected AddressingScheme<DataType, CellAddress> m_Scheme;
		protected Visualizer<DataType> m_Visualizer;
		protected CellVisualProperties m_VisProp = default(CellVisualProperties);
				

		public SimpleGridRenderer(Vector2D size, AddressingScheme<DataType, CellAddress> scheme, Visualizer<DataType> visualizer)
			: base(size) 
		{
			m_Scheme = scheme;
			m_Visualizer = visualizer;
			m_BgColor = Color.White;
		}

		protected override void UpdateImage()
		{
			base.UpdateImage();

			m_Gfx.Clear(m_BgColor);

			//draw each cell
			ViewCells<DataType, CellAddress> cellsInView = m_Scheme.GetViewEnumerator(m_View);
			foreach (CellPointer<DataType, CellAddress> p in cellsInView)
				DrawCell( p );
		}

		protected void DrawCell(CellPointer<DataType, CellAddress> data)
		{
			if (data.IsValid())
			{
				if(m_Visualizer(data.GetData(), ref m_VisProp) == true)
				{
					Vector2D center = m_View.GetCenterPos(data);
					Point[] points = new Point[7];

					//core
					for (int i = 0; i < 6; i++)
						points[i] = m_View.CellShape[i] + center;
					points[6] = points[0];

					m_Gfx.FillPolygon(new SolidBrush(m_VisProp.CoreColor), points);

					//line
					if (m_VisProp.BorderStrength > 0)
						for (int i = 0; i < 6; i++)
							m_Gfx.DrawLine(new Pen(m_VisProp.BorderColor, m_VisProp.BorderStrength), points[i], points[i + 1]);
				}
			}
		}
	}
}

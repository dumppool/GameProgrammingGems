using System;
using System.Collections.Generic;
using System.Text;

namespace hexcell
{
	//The GridView class adds geometrical properties (cell size and a position of the origin in pixelspace) to a dimension-less grid.
	//Provides helper functions for pixel->cell space conversions etc.
	//Sends events to registered observers on change
	public class GridView
	{
		protected Vector2D m_Size;
		protected Vector2D m_TopLeft;
		protected RegularShapeGeometry m_CellShape;

		public GridView()
		{
			m_CellShape = new RegularShapeGeometry(6);
			m_CellShape.OnChange += new RegularShapeGeometry.RSGEventHandler(CellShapeChanged);
		}	

		public GridView(Vector2D size)
		{
			m_Size = size;
			m_CellShape = new RegularShapeGeometry(6);
			m_CellShape.OnChange += new RegularShapeGeometry.RSGEventHandler(CellShapeChanged);
		}		

		public GridView(Vector2D size, double cellSize)
		{
			m_Size = size;
			m_CellShape = new RegularShapeGeometry(6);
			m_CellShape.Area = cellSize;
		}
		
		//Properties

		public IRegularShapeProperties CellShape
		{
			get { return m_CellShape; }
		}

		public double Width
		{
			set { m_Size.X = value; Notify(); }
			get { return m_Size.X; }
		}

		public double Height
		{
			set { m_Size.Y = value; Notify(); }
			get { return m_Size.Y; }
		}

		public Vector2D Size
		{
			set { m_Size = value; Notify(); }
			get { return m_Size; }
		}

		public double Left
		{
			set { m_TopLeft.X = value; Notify(); }
			get { return m_TopLeft.X; }
		}

		public double Top
		{
			set { m_TopLeft.Y = value; Notify(); }
			get { return m_TopLeft.Y; }
		}

		public Vector2D TopLeft
		{
			set { m_TopLeft = value; Notify(); }
			get { return m_TopLeft; }
		}

		public Vector2D BottomRight
		{
			set { m_TopLeft = value - m_Size; Notify(); }
			get { return m_TopLeft + m_Size; }
		}

		public Vector2D Center
		{
			set { m_TopLeft = value - m_Size * 0.5; Notify(); }
			get { return m_TopLeft + m_Size * 0.5; }
		}

		//Methods

		public void CellShapeChanged()
		{
			Notify();
		}

		public Vector2D GetCenterPos(ICellGeometry cell)
		{
			if (cell.GetAlignment() == CellAllignment.Horizontal)
				m_CellShape.Rotation = Math.PI / 2.0;
			else
				m_CellShape.Rotation = 0;

			return TransformOrthoToView(cell.GetOrthogonalPosition());
		}

		public void SetCellShapeRotation(CellAllignment alignment)
		{
			if (alignment == CellAllignment.Horizontal)
				m_CellShape.Rotation = Math.PI / 2.0;
			else
				m_CellShape.Rotation = 0;
		}

		public void Scroll(Vector2D delta)
		{
			m_TopLeft += delta;
			OnChange(); //notify observers
		}

		public void Focus(ICellGeometry cell)
		{
			//get cells distance d from origin
			Vector2D d = cell.GetOrthogonalPosition();
			d *= m_CellShape.Apothem * 2; //now we got the distance in pixel
			Center = d;
		}

		public bool IsCenterInside(ICellGeometry cell) //is the cells center inside?
		{
			//get cells distance d from origin
			Vector2D d = cell.GetOrthogonalPosition();
			d = TransformOrthoToView(d);
			return IsInside(d);
		}

		public bool IsPartiallyInside(ICellGeometry cell) //is a part of the cell inside? (DANGER: might return cells that are not inside but very close!)
		{
			Vector2D d = cell.GetOrthogonalPosition();
			d = TransformOrthoToView(d);
			return IsInside(d, m_CellShape.Apothem);
		}

		public bool SetCellTo(ICellGeometry cell, Vector2D viewPos) //causes the passed Iterator to point at the cell that is at the given position in the view
		{
			Vector2D d = TransformViewToOrtho(viewPos);
			cell.SetOrthogonalPosition(d);
			return true;
		}

		public void Enclose(ICellGeometry topLeft, ICellGeometry bottomRight)
		{
			Vector2D a = topLeft.GetOrthogonalPosition();
			a *= m_CellShape.Apothem * 2; //now we got the distance in pixel
			Vector2D b = bottomRight.GetOrthogonalPosition();
			b *= m_CellShape.Apothem * 2;
			TopLeft = a;
			Size = b - a;
		}

		protected bool IsInside(Vector2D viewPos)
		{
			return (viewPos.X >= 0 && viewPos.X < m_Size.X && viewPos.Y >= 0 && viewPos.Y < m_Size.Y);
		}

		protected bool IsInside(Vector2D viewPos, double enlarge)
		{
			return (viewPos.X >= -enlarge && viewPos.X < m_Size.X + enlarge && viewPos.Y >= -enlarge && viewPos.Y < m_Size.Y + enlarge);
		}

		protected Vector2D TransformViewToOrtho(Vector2D pos)
		{
			pos += m_TopLeft;
			pos /= m_CellShape.Apothem * 2;
			return pos; 
		}

		protected Vector2D TransformOrthoToView(Vector2D pos)
		{
			pos *= m_CellShape.Apothem * 2; //distance in pixel from the origin
			pos -= m_TopLeft; //distance in pixel from the left-top edge of the view
			return pos;
		}

		/* events */
		public delegate void GridViewEventHandler();

		public event GridViewEventHandler OnChange;

		public void Notify()
		{
			if (OnChange != null)
				OnChange();
		}

		/*maybe later	
		 *  public void Rotate()
		 *  public void Zoom()
		 * */
	}
}

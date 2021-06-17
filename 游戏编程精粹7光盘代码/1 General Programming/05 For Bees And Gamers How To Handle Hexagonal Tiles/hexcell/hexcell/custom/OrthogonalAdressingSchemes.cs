using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;

namespace hexcell
{
	/* Implementation of a horizontally aligned orthogonal Adressing-Scheme
     *       
     *       ^     ^               
     *     /   \ /   \             
     *    | 0,0 | 1,0 |--- X
	 *     \   / \   /
     *		 v     v
     *       | 1,0 |
     *       |\   /  
     *       |  v    
     *     Y |     
     */

	public class HAlignedOrthogonalAdressingScheme<DataType> : HAlignedAddressingScheme<DataType, Point>
	{
		private double HalfSqrt3 = Math.Sqrt(3) / 2;
		private RegularShapeGeometry m_HexGeom;

		public HAlignedOrthogonalAdressingScheme(DataGrid<DataType> grid)
			: base(grid)
		{
			m_HexGeom = new RegularShapeGeometry(6);
			m_HexGeom.Apothem = 0.5;
		}

		protected override Point ToDataGridCoord(Point coord)
		{
			//trapezoidal point used by the adressing scheme -> orthogonal point used by the data grid
			return new Point(coord.X, coord.Y);
		}

		protected override Point FromDataGridCoord(Point coord)
		{
			return new Point(coord.X, coord.Y);
		}

		public override Point GetNeighbour(Point coord, HAlignedCellNeighbours direction)
		{
			if (Math.Abs(coord.Y) % 2 == 1) //indented...
			{
				if (direction == HAlignedCellNeighbours.Left)
					coord.X--;
				else if ((direction == HAlignedCellNeighbours.Right) || (direction == HAlignedCellNeighbours.TopRight) || (direction == HAlignedCellNeighbours.BottomRight))
					coord.X++;
			}
			else
			{
				if ((direction == HAlignedCellNeighbours.TopLeft) || (direction == HAlignedCellNeighbours.Left) || (direction == HAlignedCellNeighbours.BottomLeft))
					coord.X--;
				else if ((direction == HAlignedCellNeighbours.Right))
					coord.X++;
			}
			
			if ((direction == HAlignedCellNeighbours.TopLeft) || (direction == HAlignedCellNeighbours.TopRight))
				coord.Y--;
			else if ((direction == HAlignedCellNeighbours.BottomLeft) || (direction == HAlignedCellNeighbours.BottomRight))
				coord.Y++;
	
			return coord;
		}

		public override Vector2D GetOrthogonalPosition(Point a)
		{
			return new Vector2D(a.X + (Math.Abs(a.Y) % 2) * 0.5, HalfSqrt3 * a.Y);
		}

		public override Point GetAddress(Vector2D orthoPos)
		{
			double sectWidth = 2 * m_HexGeom.Apothem;
			double sectHeight = m_HexGeom.Radius + 0.5 * m_HexGeom.SideLength;
			//apply offset...
			orthoPos.X += m_HexGeom.Apothem;
			orthoPos.Y += m_HexGeom.Radius;
			//calc section info
			int SectX = (int)(orthoPos.X / sectWidth);
			int SectY = (int)(orthoPos.Y / sectHeight);
			double SectPxlX = orthoPos.X - SectX * sectWidth;
			double SectPxlY = orthoPos.Y - SectY * sectHeight;
			double h = m_HexGeom.Radius - 0.5 * m_HexGeom.SideLength;
			double m = h / m_HexGeom.Apothem;

			Point result = new Point(SectX, SectY);

			if (Math.Abs(SectY) % 2 == 0) //SectA
			{
				if (SectPxlY < h - SectPxlX * m)
					return GetNeighbour(result, HAlignedCellNeighbours.TopLeft);
				else if (SectPxlY < -h + SectPxlX * m)
					return GetNeighbour(result, HAlignedCellNeighbours.TopRight);
				else
					return result;
			}
			else
			{
				if (SectPxlX >= m_HexGeom.Apothem)
				{
					if (SectPxlY < 2 * h - SectPxlX * m)
						return GetNeighbour(result, HAlignedCellNeighbours.TopLeft);
					else
						return result;
				}
				else
				{
					if (SectPxlY < SectPxlX * m)
						return GetNeighbour(result, HAlignedCellNeighbours.TopLeft);
					else
						return GetNeighbour(result, HAlignedCellNeighbours.Left);
				}
			}
		}

	}

	/* Implementation of a vertically aligned orthogonal Adressing-Scheme
     *       ___       ___
     *      /   \     /   \        
     *     /     \___/     \______X   
     *     \ 0,0 /   \ 2,0 /
     *      \__ /     \___/
     *	    /	\ 1,0 /
     *	   /	 \___/
     *     \ 0,1 /     
     *      \___/  
     *        |
     *		  |Y
	 */

	public class VAlignedOrthogonalAdressingScheme<DataType> : VAlignedAddressingScheme<DataType, Point>
	{
		private double HalfSqrt3 = Math.Sqrt(3) / 2;
		private RegularShapeGeometry m_HexGeom;


		public VAlignedOrthogonalAdressingScheme(DataGrid<DataType> grid)
			: base(grid)
		{
			m_HexGeom = new RegularShapeGeometry(6);
			m_HexGeom.Apothem = 0.5;
		}

		protected override Point ToDataGridCoord(Point coord)
		{
			//trapezoidal point used by the adressing scheme -> orthogonal point used by the data grid
			return new Point(coord.X, coord.Y);
		}

		protected override Point FromDataGridCoord(Point coord)
		{
			return new Point(coord.X, coord.Y);
		}

		public override Point GetNeighbour(Point coord, VAlignedCellNeighbours direction)
		{
			if (Math.Abs(coord.X) % 2 == 1) //indented...
			{
				if (direction == VAlignedCellNeighbours.Top)
					coord.Y--;
				else if ((direction == VAlignedCellNeighbours.BottomLeft) || (direction == VAlignedCellNeighbours.Bottom) || (direction == VAlignedCellNeighbours.BottomRight))
					coord.Y++;
			}
			else
			{
				if ((direction == VAlignedCellNeighbours.TopLeft) || (direction == VAlignedCellNeighbours.Top) || (direction == VAlignedCellNeighbours.TopRight))
					coord.Y--;
				else if ((direction == VAlignedCellNeighbours.Bottom))
					coord.Y++;
			}

			if ((direction == VAlignedCellNeighbours.TopLeft) || (direction == VAlignedCellNeighbours.BottomLeft))
				coord.X--;
			else if ((direction == VAlignedCellNeighbours.TopRight) || (direction == VAlignedCellNeighbours.BottomRight))
				coord.X++;

			return coord;
		}

		public override Vector2D GetOrthogonalPosition(Point a)
		{
			return new Vector2D(HalfSqrt3 * a.X, (Math.Abs(a.X) % 2) * 0.5 + a.Y);
		}

		public override Point GetAddress(Vector2D orthoPos)
		{
			double sectHeight = 2 * m_HexGeom.Apothem;
			double sectWidth = m_HexGeom.Radius + 0.5 * m_HexGeom.SideLength;
			//apply offset...
			orthoPos.X += m_HexGeom.Radius;
			orthoPos.Y += m_HexGeom.Apothem;
			//calc section info
			int SectX = (int)(orthoPos.X / sectWidth);
			int SectY = (int)(orthoPos.Y / sectHeight);
			double SectPxlX = orthoPos.X - SectX * sectWidth;
			double SectPxlY = orthoPos.Y - SectY * sectHeight;
			double h = m_HexGeom.Radius - 0.5 * m_HexGeom.SideLength;
			double m = h / m_HexGeom.Apothem;

			Point result = new Point(SectX, SectY);

			if (Math.Abs(SectX) % 2 == 0) //SectA
			{
				if (SectPxlX < h - SectPxlY * m)
					return GetNeighbour(result, VAlignedCellNeighbours.TopLeft);
				else if (SectPxlX < -h + SectPxlY * m)
					return GetNeighbour(result, VAlignedCellNeighbours.BottomLeft);
				else
					return result;
			}
			else
			{
				if (SectPxlY >= m_HexGeom.Apothem)
				{
					if (SectPxlX < 2 * h - SectPxlY * m)
						return GetNeighbour(result, VAlignedCellNeighbours.TopLeft);
					else
						return result;
				}
				else
				{
					if (SectPxlX < SectPxlY * m)
						return GetNeighbour(result, VAlignedCellNeighbours.TopLeft);
					else
						return GetNeighbour(result, VAlignedCellNeighbours.Top);
				}
			}
		}
	}
}

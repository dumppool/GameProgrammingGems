using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;

namespace hexcell
{
	public enum CellAllignment { Horizontal, Vertical };

    public abstract class AddressingScheme<DataType, Address>
    {
        Point m_Origin;
        DataGrid<DataType> m_Grid;

        public AddressingScheme(DataGrid<DataType> grid)
        {
            m_Grid = grid;
            Origin = new Point(0,0);
        }

        public Point Origin
        {
            get { return m_Origin; }
            set { m_Origin = value; }
        }

        public DataType GetData(Address coord)
        {
            return m_Grid.GetData(ToDataGridCoord(coord));
        }

        public void SetData(Address coord, DataType value)
        {
            m_Grid.SetData(ToDataGridCoord(coord), value);
        }

		public bool IsValid(Address coord)
		{
			return m_Grid.IsInside(ToDataGridCoord(coord));
		}

		//some Adressing schemes might be ambigous. This is the only secure way to ensure two adresses are not referring to the same cell.
		public bool AreEqual(Address coord1, Address coord2)
		{
			return (ToDataGridCoord(coord1) == ToDataGridCoord(coord2));
		}

		public bool GetFirstValidAddress(ref Address result)
		{
			Point coord = default(Point);
			if(m_Grid.GetFirst(ref coord))
			{
				result = FromDataGridCoord(coord);
				return true;
			}
			else
				return false;
		}

		public bool GetNextValidAddress(ref Address result)
		{
			Point coord = default(Point);

			if (m_Grid.GetNext(ref coord))
			{
				result = FromDataGridCoord(coord);
				return true;
			}
			else
				return false;
		}

        //Iterator Factory
        public CellPointer<DataType, Address> GetCellIterator(Address targetCell)
		{
			return new CellPointer<DataType, Address>(this, targetCell);
		}
		public CellPointer<DataType, Address> GetCellIterator()
		{
			return new CellPointer<DataType, Address>(this);
		}

		public NeighbourCells<DataType, Address> GetNeighbourhoodEnumerator(Address targetCell)
		{
			return new NeighbourCells<DataType, Address>(this, targetCell);
		}
		public NeighbourCells<DataType, Address> GetNeighbourhoodEnumerator()
		{
			return new NeighbourCells<DataType, Address>(this);
		}

		public RingCells<DataType, Address> GetRingEnumerator(Address targetCell)
		{
			return new RingCells<DataType, Address>(this, targetCell);
		}
		public RingCells<DataType, Address> GetRingEnumerator()
		{
			return new RingCells<DataType, Address>(this);
		}

		public ViewCells<DataType, Address> GetViewEnumerator(GridView view)
		{
			return new ViewCells<DataType, Address>(view, this);
		}

		public AllCells<DataType, Address> GetCompleteEnumerator()
		{
			return new AllCells<DataType, Address>(this);
		}

        //Abstract Methods that need implementation
        public abstract Address GetNeighbour(Address coord, CellSide direction);
        protected abstract Point ToDataGridCoord(Address coord);
		protected abstract Address FromDataGridCoord(Point coord);
		public abstract Vector2D GetOrthogonalPosition(Address a); //the unit of the return value is the diameter of a hexagon. (distance between 2 parallel sides)
		public abstract Address GetAddress(Vector2D orthoPos);  //the unit of orthoPos is again the diameter of a hexagon.
		public abstract CellAllignment GetCellAlignment(); //is the scheme vertically or horizontally alligned
	}

    
	public abstract class HAlignedAddressingScheme<DataType, Address> : AddressingScheme<DataType, Address>
    {
        public HAlignedAddressingScheme(DataGrid<DataType> grid)
            : base(grid)
        {
        }

        public override Address GetNeighbour(Address coord, CellSide side)
        {
            HAlignedCellNeighbours direction = (HAlignedCellNeighbours)(side.Side);
            return GetNeighbour(coord, direction);
        }

		public override CellAllignment GetCellAlignment()
		{
			return CellAllignment.Horizontal;
		}

        public abstract Address GetNeighbour(Address coord, HAlignedCellNeighbours direction);
    }


	public abstract class VAlignedAddressingScheme<DataType, Address> : AddressingScheme<DataType, Address>
	{
		public VAlignedAddressingScheme(DataGrid<DataType> grid)
			: base(grid)
		{
		}

		public override Address GetNeighbour(Address coord, CellSide side)
		{
			VAlignedCellNeighbours direction = (VAlignedCellNeighbours)(side.Side);
			return GetNeighbour(coord, direction);
		}

		public override CellAllignment GetCellAlignment()
		{
			return CellAllignment.Vertical;
		}

		public abstract Address GetNeighbour(Address coord, VAlignedCellNeighbours direction);
	}

   
}

using System;
using System.Collections.Generic;
using System.Text;

namespace hexcell
{

    //Abstract Methods that need implementation
    //public abstract NeighbourType GetNextCW(NeighbourType direction);
    //public abstract NeighbourType GetNextCCW(NeighbourType direction);

    public enum HAlignedCellNeighbours { TopLeft, TopRight, Right, BottomRight, BottomLeft, Left };
    public enum VAlignedCellNeighbours { Top, TopRight, BottomRight, Bottom, BottomLeft, TopLeft };
    
    public struct CellSide
    {
        private int m_Side; //0..5

        public CellSide(int side)
        {
            m_Side = side;
        }

        public int Side
        {
            get { return m_Side; }
            set { m_Side = Math.Min(5, Math.Max(0, value)); }
        }

        public void RotateClockwise()
        {
            m_Side = (m_Side + 1) % 6;
        }

        public void RotateCounterClockwise()
        {
            m_Side = m_Side - 1;
            if (m_Side < 0)
                m_Side = 5;
        }

        //== and friends
        public override bool Equals(Object obj)
        {
            return obj is CellSide && this == (CellSide)obj;
        }
        public override int GetHashCode()
        {
            return m_Side;
        }
        public static bool operator ==(CellSide a, CellSide b)
        {
            return a.m_Side == b.m_Side;
        }
        public static bool operator !=(CellSide a, CellSide b)
        {
            return !(a == b);
        }

        //implicit conversion
        public static implicit operator CellSide(HAlignedCellNeighbours side)
        {
            return new CellSide((int)side);
        }

        public static implicit operator CellSide(VAlignedCellNeighbours side)
        {
            return new CellSide((int)side);
        }
    }
}

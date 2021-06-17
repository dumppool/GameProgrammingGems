using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;

namespace hexcell
{
    public abstract class DataGrid<DataType>
    {
        public abstract DataType GetData(int x, int y);
        public abstract DataType GetData(Point p);
        public abstract void SetData(int x, int y, DataType newVal);
        public abstract void SetData(Point p, DataType newVal);
        public abstract bool IsInside(Point p);
        public abstract bool IsInside(int x, int y);
		//allow the scheme to iterate over all gridcells
		internal abstract bool GetFirst(ref Point result);
		internal abstract bool GetNext(ref Point result);
	};

    //A DataGrid Implementation based on a 2-dimensional Array
    public class FixedSizeDataGrid<DataType> : DataGrid<DataType>
    {
        private DataType[][] m_DataArray2D;
        private int m_Width;
        private int m_Height;

		private Point m_Current;

        public FixedSizeDataGrid(int width, int height)
        {
            m_Width = width;
            m_Height = height;
			m_Current = new Point(0, 0);

            m_DataArray2D = new DataType[width][];

            for (int i = 0; i < width; i++)
                m_DataArray2D[i] = new DataType[height];
        }

        public override DataType GetData(int x, int y)
        {
			if ((x >= 0) && (x < m_Width) && (y >= 0) && (y < m_Height))
				return m_DataArray2D[x][y];
			else
				return default(DataType);
        }
        public override DataType GetData(Point p)
        {
			if ((p.X >= 0) && (p.X < m_Width) && (p.Y >= 0) && (p.Y < m_Height))
				return m_DataArray2D[p.X][p.Y];
			else
				return default(DataType);
        }

        public override void SetData(int x, int y, DataType newVal)
        {
			if((x >= 0) && (x < m_Width) && (y >= 0) && (y < m_Height))
				m_DataArray2D[x][y] = newVal;
        }
        public override void SetData(Point p, DataType newVal)
        {
			if ((p.X >= 0) && (p.X < m_Width) && (p.Y >= 0) && (p.Y < m_Height))
				m_DataArray2D[p.X][p.Y] = newVal;
        }

        public override bool IsInside(Point p)
        {
            return ( (p.X >= 0) && (p.X < m_Width) && (p.Y >= 0) && (p.Y < m_Height) );
        }
        public override bool IsInside(int x, int y)
        {
            return ( (x >= 0) && (x < m_Width) && (y >= 0) && (y < m_Height));
        }
    
        public DataType[] this[int i]
        {
            get
            {
                return m_DataArray2D[i];
            }
        }

		internal override bool GetFirst(ref Point result)
		{
			m_Current = new Point(0, 0);
			if(IsInside(m_Current))
			{
				result = m_Current;
				return true;
			}
			else
				return false;
		}

		internal override bool GetNext(ref Point result)
		{
			m_Current.X++;
			if (m_Current.X < m_Width)
			{
				result = m_Current;
				return true;
			}
			else
			{
				m_Current.X = 0;
				m_Current.Y++;
				if (m_Current.Y < m_Height)
				{
					result = m_Current;
					return true;
				}
			}
			return false;
		}

    };


    //A DataGrid Implementation based on a 2-dimensional Array
    public class DynamicDataGrid<T> : DataGrid<T>
    {
        Dictionary<Point, T> m_Data = new Dictionary<Point, T>();
		Dictionary<Point, T>.Enumerator m_Enum;

        public DynamicDataGrid()
        {
        }

        public override T GetData(int x, int y)
        {
            return m_Data[new Point(x, y)];
        }
        public override T GetData(Point p)
        {
            return m_Data[p];
        }

        public override void SetData(int x, int y, T newVal)
        {
            m_Data[new Point(x, y)] = newVal;
        }
        public override void SetData(Point p, T newVal)
        {
            m_Data[p] = newVal;
        }

        public override bool IsInside(Point p)
        {
            return m_Data.ContainsKey(p);
        }
        public override bool IsInside(int x, int y)
        {
            return m_Data.ContainsKey(new Point(x, y));
        }

        public T this[Point p]
        {
            get
            {
                return m_Data[p];
            }
        }

		internal override bool GetFirst(ref Point result)
		{
			m_Enum = m_Data.GetEnumerator();
			bool nextExists = m_Enum.MoveNext();
			if (nextExists)
				result = m_Enum.Current.Key;
			
			return nextExists;
		}

		internal override bool GetNext(ref Point result)
		{
			bool nextExists = m_Enum.MoveNext();
			if (nextExists)
				result = m_Enum.Current.Key;

			return nextExists;
		}
    };


   
}

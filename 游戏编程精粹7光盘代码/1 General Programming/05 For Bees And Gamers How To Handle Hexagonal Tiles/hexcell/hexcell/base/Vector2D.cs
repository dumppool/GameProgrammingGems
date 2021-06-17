using System;
using System.Drawing;
using System.Text;

namespace hexcell
{
    public struct Vector2D
    {
        double m_X;
        double m_Y;

        //Constructors
        public Vector2D(double x, double y)
        {
            m_X = x;
            m_Y = y;
        }
        public Vector2D(PointF p)
        {
            m_X = p.X;
            m_Y = p.Y;
        }
        public Vector2D(Point p)
        {
            m_X = p.X;
            m_Y = p.Y;
        }

        public double X
        {
            get { return m_X;  }
            set { m_X = value; }
        }

        public double Y
        {
            get { return m_Y; }
            set { m_Y = value; }
        }

		public void Set(double x, double y)
		{
			m_X = x;
			m_Y = y;
		}

        public double Length()
        {
            return Math.Sqrt(LengthSq());
        }

        public double LengthSq()
        {
            return m_X * m_X + m_Y * m_Y;
        }

        //Dot Product
        public double Dot(Vector2D p)
        {
            return m_X * p.m_X + m_Y * p.m_Y;
        }
        public static double Dot(Vector2D p1, Vector2D p2)
        {
            return p1.m_X * p2.m_X + p1.m_Y * p2.m_Y;
        }

        //Rotate around Zero
        public Vector2D Rotate(double angle)
        {
            return RotateRadian(angle * Math.PI / 180);
        }
        public Vector2D RotateRadian(double angle)
        {
            //x' = x * cos ? - y * sin ? 
            double newx = m_X * Math.Cos(angle) - m_Y * Math.Sin(angle);
            //y' = x * sin ? + y * cos ?  
            double newy = m_X * Math.Sin(angle) + m_Y * Math.Cos(angle);
            return new Vector2D(newx, newy);
        }
        
        //Basic Arithmetic
		public static Vector2D operator -(Vector2D p)
		{
			return new Vector2D(-p.X, -p.Y);
		}
        public static Vector2D operator+(Vector2D p1, Vector2D p2)
        {
            return new Vector2D(p1.m_X + p2.m_X, p1.m_Y + p2.m_Y);
        }
        public static Vector2D operator-(Vector2D p1, Vector2D p2)
        {
            return new Vector2D(p1.m_X - p2.m_X, p1.m_Y - p2.m_Y);
        }
        public static Vector2D operator*(Vector2D p1, double scale)
        {
            return new Vector2D(p1.m_X * scale, p1.m_Y * scale);
        }
        public static Vector2D operator*(Vector2D p1, Vector2D p2)
        {
            return new Vector2D(p1.m_X * p2.m_X, p1.m_Y * p2.m_Y);
        }
        public static Vector2D operator/(Vector2D p1, double scale)
        {
            return new Vector2D(p1.m_X / scale, p1.m_Y / scale);
        }
        public static Vector2D operator/(Vector2D p1, Vector2D p2)
        {
            return new Vector2D(p1.m_X / p2.m_X, p1.m_Y / p2.m_Y);
        }
        public static bool operator==(Vector2D p1, Vector2D p2)
        {
            return (p1.X == p2.X && p1.Y == p2.Y);
        }
        public static bool operator!=(Vector2D p1, Vector2D p2)
        {
            return (p1.X != p2.X || p1.Y != p2.Y);
        }
        public override bool Equals(Object obj)
        {
            // Check for null values and compare run-time types.
            if (obj == null || GetType() != obj.GetType())
                return false;
            Vector2D p = (Vector2D)obj;
            return (m_X != p.X || m_Y != p.Y);
        }
        public override int GetHashCode()
        {
            return (int)m_X ^ (int)m_Y;
        }
        
        //Conversions
        public static implicit operator PointF(Vector2D p)
        {
            return new PointF((float)p.m_X, (float)p.m_Y);
        }

        public static implicit operator Point(Vector2D p)
        {
            return new Point((int)p.m_X, (int)p.m_Y);
        }
		
		public override string ToString()
		{
			return string.Format("Vector2D ({0},{1})", m_X, m_Y);
		}
    }


	
	public class VectorList
    {
		protected Vector2D[] m_VertexArray;
		
		public VectorList(int count)
        {
            m_VertexArray = new Vector2D[count];
        }

        public VectorList(params Vector2D[] vertices)
        {
            int i = 0;
            m_VertexArray = new Vector2D[vertices.Length];
            foreach (Vector2D v in vertices)
            {
                m_VertexArray[i++] = v;
            }
        }

        public int Count
        {
            get { return m_VertexArray.Length; }
        }

        public Vector2D this[int i]
        {
            get
            {
                int idx = i % (m_VertexArray.Length);
                return m_VertexArray[idx];
            }
            set
            {
                int idx = i % (m_VertexArray.Length);
                m_VertexArray[idx] = value;
            }
        }

    }

}

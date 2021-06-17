using System;
using System.Collections.Generic;
using System.Text;

namespace hexcell
{
	public interface IRegularShapeProperties
	{
		//config the size
		double Area { get; set; }
		double SideLength { get; set; }
		double Radius { get; set; } //highest distance between center and border
		double Apothem { get; set; }//closest distance between center and border
		//read only
		int SideCount { get; }
		double Perimeter { get; }
		double Rotation { get; }
		Vector2D this[int i] { get; }
		Vector2D[] GetVertexArray();
	}

    /// <summary>
    /// Tilings are defined by their Tiles, which are called Cells in this Framework. 
    /// Cells are geometric shapes and the properties of some geometric shapes, regular polygons, are defined by this class.
    /// </summary>
    public class RegularShapeGeometry : IRegularShapeProperties
    {
        protected double m_Area;
        protected double m_SideLength;
        protected double m_Radius;
        protected double m_Apothem;
        protected double m_PolyMultiplier;
        protected double m_HalfInnerAngle;
        protected double m_Rotation;

        protected VectorList m_Vertices;

        public RegularShapeGeometry(int sides)
        {
            m_Vertices = new VectorList(sides);
            m_HalfInnerAngle = Math.PI / sides;
            m_PolyMultiplier = 4 * Math.Tan(m_HalfInnerAngle) / sides;
            Area = 1.0f;
            m_Rotation = 0;
        }

		public RegularShapeGeometry(IRegularShapeProperties copy)
		{
			m_Vertices = new VectorList(copy.SideCount);
			m_HalfInnerAngle = Math.PI / copy.SideCount;
			m_PolyMultiplier = 4 * Math.Tan(m_HalfInnerAngle) / copy.SideCount;
			m_Rotation = copy.Rotation;
			SetArea(copy.Area);
		}
                
        protected void SetArea(double newArea)
        {
			if (m_Area == newArea)
				return;
			m_Area = newArea;
            m_SideLength = Math.Sqrt(m_Area * m_PolyMultiplier);
            m_Radius = 0.5 * m_SideLength / Math.Sin(m_HalfInnerAngle);
            m_Apothem = 0.5 * m_SideLength / Math.Tan(m_HalfInnerAngle);
           
            //and update the rest
            UpdateVertices();
        }

        protected void SetSideLength(double newSideLength)
        {
			if (m_SideLength == newSideLength)
				return;
			m_SideLength = newSideLength;
            m_Area = Math.Pow(m_SideLength, 2) / m_PolyMultiplier;
            m_Radius = 0.5 * m_SideLength / Math.Sin(m_HalfInnerAngle);
            m_Apothem = 0.5 * m_SideLength / Math.Tan(m_HalfInnerAngle);
      
            UpdateVertices();
        }

        protected void SetApothem(double newApothem)
        {
			if (m_Apothem == newApothem)
				return;
			m_Apothem = newApothem;
            m_SideLength = m_Apothem * Math.Tan(m_HalfInnerAngle) * 2;
            m_Area = Math.Pow(m_SideLength, 2) / m_PolyMultiplier;
            m_Radius = 0.5 * m_SideLength / Math.Sin(m_HalfInnerAngle);

            UpdateVertices();
        }

        protected void SetRadius(double newRadius)
        {
			if (m_Radius == newRadius)
				return; 
			m_Radius = newRadius;
            m_SideLength = m_Radius * Math.Sin(m_HalfInnerAngle) * 2;
            m_Area = Math.Pow(m_SideLength, 2) / m_PolyMultiplier;
            m_Apothem = 0.5 * m_SideLength / Math.Tan(m_HalfInnerAngle);

            UpdateVertices();
        }

		public void SetRotation(double newRot)
		{
			if (m_Rotation != newRot) 
			{
				m_Rotation = newRot; 
				UpdateVertices(); 
			} 
		}

        protected void UpdateVertices()
        {
			//calc vertices by rotating v
            Vector2D v = new Vector2D(0, -m_Radius);
            double angle = -m_HalfInnerAngle + m_Rotation;
            for (int i = 0; i < SideCount; i++)
            {
                Vector2D vec = v.RotateRadian(angle);
				m_Vertices[i] = v.RotateRadian(angle);

				angle = ((2 * i + 1) * m_HalfInnerAngle) + m_Rotation;
            }

			/*
			#if DEBUG
				System.Diagnostics.Debug.Print("*** RegularShapeGeometry ***");
				for (int i = 0; i < SideCount; i++)
					System.Diagnostics.Debug.Print(m_Vertices[i].ToString());
			#endif
			*/
			Notify();

        }

        public double Rotation
        {
            get { return m_Rotation; }
			set { SetRotation(value); }
        }

		public double Area
        {
            get { return m_Area; }
            set { SetArea(value); }
        }

		public double SideLength
        {
            get { return m_SideLength; }
            set { SetSideLength(value); }
        }

		public double Radius //highest distance between center and border
        {
            get { return m_Radius; }
            set { SetRadius(value); }
        }

		public double Apothem //closest distance between center and border
        {
            get { return m_Apothem; }
            set { SetApothem(value); }
        }

		public int SideCount
        {
            get { return m_Vertices.Count; }
        }

		public double Perimeter
        {
			get { return SideCount * m_SideLength; }
        }

		public Vector2D this[int i]
		{
			get
			{
				return m_Vertices[i%SideCount];
			}
		}

		public Vector2D[] GetVertexArray()
		{
			Vector2D[] vertices = new Vector2D[m_Vertices.Count+1];
			for (int i = 0; i <= m_Vertices.Count; i++)
				vertices[i] = m_Vertices[i];
			return vertices;
		}

		/* events */
		public delegate void RSGEventHandler();

		public event RSGEventHandler OnChange;

		public void Notify()
		{
			if (OnChange != null)
				OnChange();
		}

    }

}

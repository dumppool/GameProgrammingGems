using System;
using System.Collections.Generic;
using System.Text;

namespace hexcell
{
	public interface ICellGeometry
	{
		Vector2D GetOrthogonalPosition();
		void SetOrthogonalPosition(Vector2D pos);
		CellAllignment GetAlignment();
	}

	public interface ICellData<DataType> : ICellGeometry
	{
		bool IsValid();
		DataType GetData();
		void SetData(DataType value);
	}

	public interface ICellAddress<Address> : ICellGeometry
	{
		void SetAddress(Address newCell);
		Address GetAddress();
	}

	public class CellPointer<DataType, Address> : ICellGeometry, ICellData<DataType>, ICellAddress<Address>
	{
		protected Address m_TargetCell;
		protected AddressingScheme<DataType, Address> m_Scheme;

		public CellPointer(AddressingScheme<DataType, Address> targetScheme)
		{
			m_Scheme = targetScheme;
		}

		public CellPointer(AddressingScheme<DataType, Address> targetScheme, Address targetCell)
		{
			m_Scheme = targetScheme;
			SetAddress(targetCell);
		}

		public DataType GetData()
		{
			return m_Scheme.GetData(m_TargetCell);
		}

		public bool IsValid()
		{
			return m_Scheme.IsValid(m_TargetCell);
		}

		public void SetData(DataType value)
		{
			m_Scheme.SetData(m_TargetCell, value);
		}

		public virtual void SetAddress(Address newCell)
		{
			m_TargetCell = newCell;
		}

		public Address GetAddress()
		{
			return m_TargetCell;
		}

		public Address TargetCell
		{
			get { return GetAddress(); }
			set { SetAddress(value); }
		}

		public void Move(CellSide direction)
		{
			SetAddress(m_Scheme.GetNeighbour(m_TargetCell, direction));
		}

		//Interface Implementation
		public Vector2D GetOrthogonalPosition()
		{
			return m_Scheme.GetOrthogonalPosition(m_TargetCell);
		}

		public void SetOrthogonalPosition(Vector2D pos)
		{
			SetAddress(m_Scheme.GetAddress(pos));
		}

		public CellAllignment GetAlignment()
		{
			return m_Scheme.GetCellAlignment();
		}
	}

	public abstract class CellEnumerator<DataType, Address> : CellPointer<DataType, Address>, IEnumerable<CellPointer<DataType, Address>>, IEnumerator<CellPointer<DataType, Address>>
	{
		public CellEnumerator(AddressingScheme<DataType, Address> targetScheme)
			:base(targetScheme)
		{
			m_Scheme = targetScheme;
		}

		//Implement this if you want to write an EnumerableIterator
		protected abstract void ResetTargetCell();
		protected abstract bool NextTargetCell(); //return true if done
		
		#region IEnumerator

		public IEnumerator<CellPointer<DataType, Address>> GetEnumerator()
		{
			return this;
		}

		public CellPointer<DataType, Address> Current
		{
			get { return new CellPointer<DataType, Address>(m_Scheme, m_TargetCell); }
		}

		public bool MoveNext()
		{
			return !NextTargetCell();
		}

		public void Dispose()
		{
		}

		public void Reset()
		{
			ResetTargetCell();
		}

		System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
		{
			return this;
		}

		object System.Collections.IEnumerator.Current
		{
			get { return m_TargetCell; }
		}

		#endregion
	}

	public class NeighbourCells<DataType, Address> : CellEnumerator<DataType, Address>
	{
		protected CellSide m_Direction;
		protected CellSide m_StartingDirection;
		protected Address m_OriginCell;
		protected int m_StepCnt;

		public NeighbourCells(AddressingScheme<DataType, Address> targetScheme)
			: base(targetScheme)
		{
			m_StartingDirection = m_Direction;
			m_StepCnt = 0;
			UpdateTargetCell();
		}

		public NeighbourCells(AddressingScheme<DataType, Address> targetScheme, Address originCell)
			: base(targetScheme)
		{
			m_OriginCell = originCell;
			m_StartingDirection = m_Direction;
			m_StepCnt = 0;
			UpdateTargetCell();
		}

		public void SetDirection(CellSide direction)
		{
			m_Direction = direction;
			m_StartingDirection = m_Direction;
			m_StepCnt = 0;
			UpdateTargetCell();
		}

		public bool NextClockwise()
		{
			m_Direction.RotateClockwise();
			UpdateTargetCell();
			return (m_StepCnt++ == 6);
		}

		public bool NextCounterClockwise()
		{
			m_Direction.RotateClockwise();
			UpdateTargetCell();
			return (m_StepCnt++ == 6);
		}

		public override void SetAddress(Address newCell)
		{
			m_OriginCell = newCell;
			UpdateTargetCell();
		}

		protected void UpdateTargetCell()
		{
			m_TargetCell = m_Scheme.GetNeighbour(m_OriginCell, m_Direction);
		}

		override protected void ResetTargetCell()
		{
			m_Direction = m_StartingDirection;
			m_StepCnt = 0;
		}

		override protected bool NextTargetCell()
		{
			return NextClockwise();
		}

	}

	public class RingCells<DataType, Address> : CellEnumerator<DataType, Address>
	{
		protected Address m_OriginCell;
		private int m_RingRadius;

		protected CellSide m_ExpandDirection;
		protected CellSide m_IterationDirection;
		protected int m_StepCnt;


		public RingCells(AddressingScheme<DataType, Address> targetScheme)
			: base(targetScheme)
		{
			ResetTargetCell();
		}

		public RingCells(AddressingScheme<DataType, Address> targetScheme, Address originCell)
			: base(targetScheme)
		{
			m_OriginCell = originCell;
			ResetTargetCell();
		}

		public int RingRadius
		{
			get { return m_RingRadius; }
			set { m_RingRadius = value; ResetTargetCell(); }
		}

		public override void SetAddress(Address newCell)
		{
			m_OriginCell = newCell;
			ResetTargetCell();
		}

		protected override void ResetTargetCell()
		{
			m_TargetCell = m_OriginCell;
			m_IterationDirection = m_ExpandDirection;
			m_IterationDirection.RotateClockwise();
			m_IterationDirection.RotateClockwise();

			m_StepCnt = 1;

			//move out by m_RingRadius steps
			for (int i = 0; i < RingRadius; i++)
				m_TargetCell = m_Scheme.GetNeighbour(m_TargetCell, m_ExpandDirection);
		}

		protected override bool NextTargetCell()
		{
			if (m_RingRadius == 0)
			{
				if(m_StepCnt == 2)
					return true;

				m_StepCnt++;
				m_TargetCell = m_OriginCell;
				return false;
			}

			m_TargetCell = m_Scheme.GetNeighbour(m_TargetCell, m_IterationDirection);
			if (m_StepCnt % m_RingRadius == 0)
				m_IterationDirection.RotateClockwise();

			m_StepCnt++;

			return (m_StepCnt == m_RingRadius * 6 + 2);
		}
	}

	public class CustomNeighbourhoodCells<DataType, Address> : CellEnumerator<DataType, Address>
	{
		protected CellSide m_Direction;
		protected Address m_OriginCell;
		protected int m_Radius;

		protected CellSide m_IterationDirection;
		protected int m_StepCnt;
		protected byte m_CurCellCnt;
		protected byte m_MaxCellCnt;
		protected ulong m_Bitmask;

		public CustomNeighbourhoodCells(AddressingScheme<DataType, Address> targetScheme)
			: base(targetScheme)
		{
			ResetTargetCell();
		}

		public CustomNeighbourhoodCells(AddressingScheme<DataType, Address> targetScheme, Address originCell)
			: base(targetScheme)
		{
			DefineNeighbourhood(0x7f);
			m_OriginCell = originCell;
			ResetTargetCell();
		}

		public CustomNeighbourhoodCells(AddressingScheme<DataType, Address> targetScheme, Address originCell, ulong neighbourhoodMask)
			: base(targetScheme)
		{
			DefineNeighbourhood(neighbourhoodMask);
			m_OriginCell = originCell;
			ResetTargetCell();
		}
		
		public override void SetAddress(Address newCell)
		{
			m_OriginCell = newCell;
			ResetTargetCell();
		}

		protected void DefineNeighbourhood(ulong bitmask)
		{
			m_Bitmask = bitmask;
			m_MaxCellCnt = 0;
			//What's the last cell thats in the neighbourhood?
			for (byte i = 0; i < 64; i++)
			{
				 if (((m_Bitmask >> i) & 1) == 1)
					 m_MaxCellCnt = (byte)(i + 1);
			}
		}

		protected override void ResetTargetCell()
		{
			m_TargetCell = m_Scheme.GetNeighbour(m_OriginCell, new CellSide(0));
			m_IterationDirection = new CellSide(3);
			m_StepCnt = -2; //needed so we don't get to the first ring immediately
			m_Radius = 0;
			m_CurCellCnt = 0;
		}

		protected override bool NextTargetCell()
		{
			if(m_CurCellCnt == m_MaxCellCnt) return true; //The current cell is the last cell in the neighbourhood. Done!

			bool bitSet;
			do
			{
				//progress to next cell
				if (m_StepCnt == m_Radius * 6 - 1)
					NextRing();
				else
				{
					m_TargetCell = m_Scheme.GetNeighbour(m_TargetCell, m_IterationDirection);
					m_StepCnt++;
					if (m_Radius > 0 && m_StepCnt % m_Radius == 0)
						m_IterationDirection.RotateClockwise();
				}
				bitSet = ((m_Bitmask >> m_CurCellCnt) & 1) == 1;
				m_CurCellCnt++;
			}
			while ( !bitSet); //terminates when the cell is marked with a 1 in the bitmask

			return false; //not done...
		}

		protected void NextRing()
		{
			m_IterationDirection = new CellSide(2);
			m_StepCnt = 0;
			m_Radius++;

			m_TargetCell = m_OriginCell;
			for (int i = 0; i < m_Radius; i++)
				m_TargetCell = m_Scheme.GetNeighbour(m_TargetCell, new CellSide(0));
		}
	}

	public class AllCells<DataType, Address> : CellEnumerator<DataType, Address>
	{
		private bool m_Valid;

		public AllCells(AddressingScheme<DataType, Address> targetScheme)
			: base(targetScheme)
		{
			m_Valid = false;
		}
		//Implement this if you want to write an EnumerableIterator
		protected override void ResetTargetCell()
		{
			m_Valid = true;
		}
		protected override bool NextTargetCell()
		{
			if(m_Valid)
				m_Valid = m_Scheme.GetNextValidAddress(ref m_TargetCell);
			else 
				m_Valid = m_Scheme.GetFirstValidAddress(ref m_TargetCell);
			 
			//done if we didn't get a valid target-cell
			return !m_Valid;
		}
	}
	
	public class ViewCells<DataType, Address> : CellEnumerator<DataType, Address>
	{
		protected Address m_OriginCell;
		protected int m_Radius;

		protected CellSide m_IterationDirection;
		protected int m_StepCnt;
		protected bool m_Done;

		protected GridView m_View;

		public ViewCells(GridView view, AddressingScheme<DataType, Address> targetScheme)
			: base(targetScheme)
		{
			m_View = view;
			Focus(m_View.Size * 0.5);
		}

		public override void SetAddress(Address newCell)
		{
			m_OriginCell = newCell;
			ResetTargetCell();
		}

		public void Focus(Vector2D viewPos)
		{
			m_View.SetCellTo(this, viewPos);
		}

		protected override void ResetTargetCell()
		{
			m_TargetCell = m_Scheme.GetNeighbour(m_OriginCell, new CellSide(0));
			m_IterationDirection = new CellSide(3);
			m_StepCnt = -2; //needed so we don't get to the first ring immediately
			m_Radius = 0;
			m_Done = false;
		}

		protected override bool NextTargetCell()
		{
			int oldRadius = m_Radius;
			bool inside = false;
			do
			{
				inside = CheckNextCell();
			}
			while (!inside && (m_Radius <= oldRadius + 1));

			return !inside; //done if we didn't found one inside
		}

		protected bool CheckNextCell()
		{
			if (m_StepCnt == m_Radius * 6 - 1)
				NextRing();
			else
			{
				m_TargetCell = m_Scheme.GetNeighbour(m_TargetCell, m_IterationDirection);
				m_StepCnt++;
				if (m_Radius > 0 && m_StepCnt % m_Radius == 0)
					m_IterationDirection.RotateClockwise();
			}

			return m_View.IsPartiallyInside(this); // done if not visible
		}

		protected void NextRing()
		{
			m_IterationDirection = new CellSide(2);
			m_StepCnt = 0;
			m_Radius++;
			
			m_TargetCell = m_OriginCell;
			for (int i = 0; i < m_Radius; i++)
				m_TargetCell = m_Scheme.GetNeighbour(m_TargetCell, new CellSide(0));
		}
	}

}
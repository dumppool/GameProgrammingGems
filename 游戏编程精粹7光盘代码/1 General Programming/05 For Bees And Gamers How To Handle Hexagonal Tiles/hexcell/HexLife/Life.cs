using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using hexcell;

namespace HexLife
{
	struct SimpleLifeState
	{
		public bool Alive;
		public SimpleLifeState(bool alive)
		{
			Alive = alive;
		}
	}

	// A little more complex LifeCellState (for nicer Visualisation)

	struct AgingLifeState
	{
		public bool Alive;
		public int Activity;
		public AgingLifeState(bool alive)
		{
			Alive = alive;
			Activity = 5;
		}
	}

	class SimpleLifeCell
	{
		protected AgingLifeState m_NextState;
		protected AgingLifeState m_CurrentState;

		public SimpleLifeCell(bool alive)
		{
			m_NextState = new AgingLifeState(alive);
			m_CurrentState = new AgingLifeState(alive);
		}

		public void Progress()
		{
			m_CurrentState = m_NextState;
		}

		public AgingLifeState CurrentState
		{
			get { return m_CurrentState; }
		}

		public bool Alive
		{
			get { return m_CurrentState.Alive; }
		}

		public int Activity
		{
			get { return m_CurrentState.Activity; }
		}


		public void CalcNextState(CustomNeighbourhoodCells<SimpleLifeCell, Point> neighbourhood)
		{
			int aliveCells = 0;
			foreach (CellPointer<SimpleLifeCell, Point> cell in neighbourhood)
				if (cell.IsValid())
				{
					if (cell.GetData().Alive)
					aliveCells++;
				}

			m_NextState = m_CurrentState;

			if ((aliveCells == 3) && !m_CurrentState.Alive) //come to life
			{
				m_NextState.Alive = true;
				m_NextState.Activity--;
			}
			else if (aliveCells < 3 || aliveCells > 4) //die
			{
				m_NextState.Alive = false;
				m_NextState.Activity--;
			}
			else
				m_NextState.Activity++;

			m_NextState.Activity = Math.Min(Math.Max(m_NextState.Activity, 0), 10);
		}
	
	}



}

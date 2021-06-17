using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

public delegate Output DataConverter<Input, Output>(Input[] inputs, double[] weights);

namespace hexcell
{

	public class GridConverter<InputData, OutputData, Address>
	{

		public static void HexGridToArrayGeometric(AddressingScheme<InputData, Address> source, Address a, Address b, OutputData[,] target, DataConverter<InputData, OutputData> convFunction)
		{
			int targetWidth = target.GetLength(1);
			int targetHeight = target.GetLength(0);
			GridView view = new GridView();
			view.Enclose(new CellPointer<InputData, Address>(source, a), new CellPointer<InputData, Address>(source, b));
			Vector2D arrayToView = new Vector2D(view.Width / targetWidth, view.Height / targetHeight);

			Vector2D[] targetCellShape = new Vector2D[5];
			targetCellShape[0] = new Vector2D(-0.5 * arrayToView.X, -0.5 * arrayToView.Y);
			targetCellShape[1] = new Vector2D(0.5 * arrayToView.X, -0.5 * arrayToView.Y);
			targetCellShape[2] = new Vector2D(0.5 * arrayToView.X, 0.5 * arrayToView.Y);
			targetCellShape[3] = new Vector2D(-0.5 * arrayToView.X, 0.5 * arrayToView.Y);
			targetCellShape[4] = targetCellShape[0];
			view.SetCellShapeRotation(source.GetCellAlignment());
			Vector2D[] sourceCellShape = view.CellShape.GetVertexArray();

			List<InputData> inputs = new List<InputData>();
			List<double> weights = new List<double>();
			CellPointer<InputData, Address> closestcell = new CellPointer<InputData, Address>(source);
			for (int y = 0; y < targetHeight; y++)
				for (int x = 0; x < targetWidth; x++)
				{
					inputs.Clear();
					weights.Clear();
					Vector2D targetCellPos = new Vector2D(x * arrayToView.X, y * arrayToView.Y);

					view.SetCellTo(closestcell, targetCellPos);

					RingCells<InputData, Address> cellring = source.GetRingEnumerator(closestcell.GetAddress());
					cellring.RingRadius = 0;
					double oldweightsum = 0;
					double weightsum = 0;
					do
					{
						oldweightsum = weightsum;
						foreach (CellPointer<InputData, Address> cell in cellring)
						{
							Vector2D sourceCellPos = view.GetCenterPos(cell);
							double area = PolygonClipper.GetIntersectionArea(sourceCellShape, targetCellShape, targetCellPos - sourceCellPos) / arrayToView.X / arrayToView.Y;
							if (area > 0)
							{
								if (cell.IsValid())
								{
									inputs.Add(cell.GetData());
									weights.Add(area);
									weightsum += area;
								}
							}
						}
						cellring.RingRadius = cellring.RingRadius + 1;
					}
					while ((weightsum > oldweightsum) && (weightsum < 0.98));

					for (int i = 0; i < weights.Count; i++)
						weights[i] /= weightsum;
					target[y, x] = convFunction(inputs.ToArray(), weights.ToArray());
				}
		}

		public static void HexGridToArrayNearest(AddressingScheme<InputData, Address> source, Address a, Address b, OutputData[,] target, Converter<InputData, OutputData> convFunction)
		{
			int targetWidth = target.GetLength(1);
			int targetHeight = target.GetLength(0);
			GridView view = new GridView();
			view.Enclose(new CellPointer<InputData, Address>(source, a), new CellPointer<InputData, Address>(source, b));
			Vector2D arrayToView = new Vector2D(view.Width / targetWidth, view.Height / targetHeight);

			CellPointer<InputData, Address> cell = new CellPointer<InputData, Address>(source);
			for (int y = 0; y < targetHeight; y++)
				for (int x = 0; x < targetWidth; x++)
				{
					view.SetCellTo(cell, new Vector2D(x * arrayToView.X, y * arrayToView.Y));
					target[y, x] = convFunction(cell.GetData());
				}
		}

		public static void HexGridToArrayLinear(AddressingScheme<InputData, Address> source, Address a, Address b, OutputData[,] target, DataConverter<InputData, OutputData> convFunction)
		{
			int targetWidth = target.GetLength(1);
			int targetHeight = target.GetLength(0);
			GridView view = new GridView();
			view.Enclose(new CellPointer<InputData, Address>(source, a), new CellPointer<InputData, Address>(source, b));
			Vector2D arrayToView = new Vector2D((view.Width) / (targetWidth), view.Height / (targetHeight));
			double maxdist = view.CellShape.Apothem * 2;
			List<InputData> inputs = new List<InputData>();
			List<double> weights = new List<double>();
			CellPointer<InputData, Address> closestcell = new CellPointer<InputData, Address>(source);
			for(int y = 0; y < targetHeight; y++)
				for (int x = 0; x < targetWidth; x++)
				{
					inputs.Clear();
					weights.Clear(); 
					Vector2D targetCellPos = new Vector2D(x * arrayToView.X, y * arrayToView.Y);
					view.SetCellTo(closestcell, targetCellPos);
					double weightsum = 0;
							
					//consider closest cell
					if (closestcell.IsValid())
					{
						Vector2D sourceCellPos = view.GetCenterPos(closestcell);
						double dist = (targetCellPos - sourceCellPos).Length();
						if(dist < maxdist)
						{
							weights.Add(1 - (dist / maxdist));
							weightsum += 1 - (dist / maxdist);
							inputs.Add(closestcell.GetData());
						}
							
						//consider 6 neighbours
						NeighbourCells<InputData, Address> cellring = source.GetNeighbourhoodEnumerator(closestcell.GetAddress());
						foreach (CellPointer<InputData, Address> cell in cellring)
							if (cell.IsValid())
							{
								sourceCellPos = view.GetCenterPos(cell);
								dist = (targetCellPos - sourceCellPos).Length();
								if(dist < maxdist)
								{
									weights.Add(1 - (dist / maxdist));
									weightsum += 1 - (dist / maxdist);
									inputs.Add(cell.GetData());
								}
							}
						
						for (int i = 0; i < weights.Count; i++)
							weights[i] /= weightsum;
						target[y, x] = convFunction(inputs.ToArray(), weights.ToArray());
					}
				}
		}

		public static void ArrayToHexGridGeometric(InputData[,] source, AddressingScheme<OutputData, Address> target, Address a, Address b, DataConverter<InputData, OutputData> convFunction)
		{
			int sourceWidth = source.GetLength(1);
			int sourceHeight = source.GetLength(0);
			GridView view = new GridView();
			view.Enclose(new CellPointer<OutputData, Address>(target, a), new CellPointer<OutputData, Address>(target, b));
			Vector2D viewToArray = new Vector2D(sourceWidth / view.Width, sourceHeight / view.Height);

			Vector2D[] sourceCellShape = new Vector2D[5];
			sourceCellShape[0] = new Vector2D(-0.5 / viewToArray.X, -0.5 / viewToArray.Y);
			sourceCellShape[1] = new Vector2D(0.5 / viewToArray.X, -0.5 / viewToArray.Y);
			sourceCellShape[2] = new Vector2D(0.5 / viewToArray.X, 0.5 / viewToArray.Y);
			sourceCellShape[3] = new Vector2D(-0.5 / viewToArray.X, 0.5 / viewToArray.Y);
			sourceCellShape[4] = sourceCellShape[0];

			Vector2D[] targetCellShape = view.CellShape.GetVertexArray();

			List<InputData> inputs = new List<InputData>();
			List<double> weights = new List<double>();
			ViewCells<OutputData, Address> cellsInView = target.GetViewEnumerator(view);
			Vector2D searchRadius = new Vector2D(view.CellShape.Radius + 0.5, view.CellShape.Radius + 0.5);
			foreach (CellPointer<OutputData, Address> cell in cellsInView)
			{
				inputs.Clear();
				weights.Clear();
				Vector2D targetCellPos = view.GetCenterPos(cell);
				Vector2D top = (targetCellPos - searchRadius) * viewToArray;
				Vector2D bottom = (targetCellPos + searchRadius) * viewToArray;
				//go through all sourceCells in that area
				for (int y = (int)Math.Floor(top.Y); y < (int)Math.Ceiling(bottom.Y); y++)
					for (int x = (int)Math.Floor(top.X); x < (int)Math.Ceiling(bottom.X); x++)
					{
						Vector2D sourceCellPos = new Vector2D(x + 0.5, y + 0.5) / viewToArray;
						double area = PolygonClipper.GetIntersectionArea(sourceCellShape, targetCellShape, sourceCellPos - targetCellPos);
						if (area > 0)
						{
							int arrayX = Math.Max(Math.Min(x, sourceWidth - 1), 0);
							int arrayY = Math.Max(Math.Min(y, sourceHeight - 1), 0);
							inputs.Add(source[arrayY, arrayX]);
							weights.Add(area / view.CellShape.Area);
						}
					}
				OutputData c = convFunction(inputs.ToArray(), weights.ToArray());
				cell.SetData(c);
				//System.Diagnostics.Debug.Write(cnt.ToString()+"/"+wgt.ToString()+" ");
			}
		}


		public static void ArrayToHexGridLinear(InputData[,] source, AddressingScheme<OutputData, Address> target, Address a, Address b, DataConverter<InputData, OutputData> convFunction)
		{
			int sourceWidth = source.GetLength(1);
			int sourceHeight = source.GetLength(0);
			GridView view = new GridView();
			view.Enclose(new CellPointer<OutputData, Address>(target, a), new CellPointer<OutputData, Address>(target, b));
			Vector2D viewToArray = new Vector2D(sourceWidth / view.Width, sourceHeight / view.Height);

			List<InputData> inputs = new List<InputData>();
			List<double> weights = new List<double>();
			ViewCells<OutputData, Address> cellsInView = target.GetViewEnumerator(view);
			double maxDist = view.CellShape.Apothem * 1.5;
			foreach (CellPointer<OutputData, Address> cell in cellsInView)
			{
				inputs.Clear();
				weights.Clear();
				Vector2D targetCellPos = view.GetCenterPos(cell);
				Vector2D bestFit = targetCellPos * viewToArray;
				int x = (int)Math.Floor(bestFit.X);
				int y = (int)Math.Floor(bestFit.Y);
				double aX = bestFit.X - x;
				double aY = bestFit.Y - y;
				
				if(x >= 0 && y >= 0)
				{
					if(x < source.GetLength(1)-1 && y < source.GetLength(0)-1)
					{
						inputs.Add(source[y, x]);
						weights.Add((1 - aX) * (1 - aY));
						inputs.Add(source[y + 1, x]);
						weights.Add((1 - aX) * aY);
						inputs.Add(source[ y, x + 1]);
						weights.Add(aX * (1 - aY));
						inputs.Add(source[y + 1, x + 1]);
						weights.Add(aX * aY);
					}
					else if (x < source.GetLength(1) && y < source.GetLength(0) - 1)
					{
						inputs.Add(source[y, x]);
						weights.Add((1 - aY));
						inputs.Add(source[y + 1, x]);
						weights.Add(aY);
					}
					else if (x < source.GetLength(1) - 1 && y < source.GetLength(0))
					{
						inputs.Add(source[y, x]);
						weights.Add((1 - aX));
						inputs.Add(source[y, x + 1]);
						weights.Add(aX);
					}
				}
				cell.SetData(convFunction(inputs.ToArray(), weights.ToArray()));
				//System.Diagnostics.Debug.Write(cnt.ToString()+"/"+wgt.ToString()+" ");
			}
		}

		public static void ArrayToHexGridNearest(InputData[,] source, AddressingScheme<OutputData, Address> target, Address a, Address b, Converter<InputData, OutputData> convFunction)
		{
			int sourceWidth = source.GetLength(1);
			int sourceHeight = source.GetLength(0);
			GridView view = new GridView();
			view.Enclose(new CellPointer<OutputData, Address>(target, a), new CellPointer<OutputData, Address>(target, b));
			Vector2D viewToArray = new Vector2D(sourceWidth / view.Width, sourceHeight / view.Height);

			ViewCells<OutputData, Address> cellsInView = target.GetViewEnumerator(view);
			foreach (CellPointer<OutputData, Address> cell in cellsInView)
			{
				Vector2D sourceCellPos = view.GetCenterPos(cell) * viewToArray + new Vector2D(0.5, 0.5);
				int arrayX = (int)Math.Max(Math.Min(sourceCellPos.X, sourceWidth - 1), 0);
				int arrayY = (int)Math.Max(Math.Min(sourceCellPos.Y, sourceHeight - 1), 0);
				cell.SetData(convFunction(source[arrayY, arrayX]));
				//System.Diagnostics.Debug.Write(cnt.ToString()+"/"+wgt.ToString()+" ");
			}
		}
		
	}
}

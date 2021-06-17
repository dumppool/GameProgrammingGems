using System;
using System.Collections.Generic;
using System.Text;

namespace hexcell
{
	public class PolygonClipper
	{
		
		public static double GetIntersectionArea(Vector2D[] polyA, Vector2D[] polyB, Vector2D offsetA)
		{
			LinkedList<Vector2D> polyList = new LinkedList<Vector2D>();
			//fill polyList
			for (int i = 0; i < polyA.Length; i++)
				polyList.AddLast(polyA[i] + offsetA);
		
			//clip polyList
			for (int i = 0; i < polyB.Length-1; i++) //for each side in polyA
				Clip(polyList, polyB[i], polyB[i + 1]);

			if (polyList.Count < 3)
				return 0;
			//calculate Area
			LinkedListNode<Vector2D> current = polyList.First;
			LinkedListNode<Vector2D> next = current.Next;
			double squaresum = 0;
			while (next != null)
			{
				squaresum += current.Value.X * next.Value.Y - next.Value.X * current.Value.Y;
				current = next;
				next = next.Next;
			}	
			return squaresum / 2;
		}

		public static Vector2D[] CalculateIntersection(Vector2D[] polyA, Vector2D[] polyB)
		{
			LinkedList<Vector2D> polyList = new LinkedList<Vector2D>(polyA);

			//is the first point of polyA in polyB?
			for (int i = 0; i < polyB.Length-1; i++) //for each side in polyA
				Clip(polyList, polyB[i], polyB[i + 1]);
		
			Vector2D[] result = new Vector2D[polyList.Count];
			polyList.CopyTo(result, 0);
			return result;
		}

		public static double CalcArea(Vector2D[] poly)
		{
			//implementation based on Green's Theorem
			double squaresum = 0;
			for (int i = 0; i < poly.Length - 1; i++)
				squaresum += poly[i].X * poly[i + 1].Y - poly[i + 1].X * poly[i].Y;

			return squaresum / 2;
		}

		private static bool GetIntersection(Vector2D line_start, Vector2D line_end, Vector2D clipper_a, Vector2D clipper_b, ref Vector2D intersection)
		{
			//http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
			Vector2D line = line_end - line_start;
			Vector2D clipper = clipper_b - clipper_a;
			double a = clipper.X * (line_start.Y - clipper_a.Y) - clipper.Y * (line_start.X - clipper_a.X);
			double b = clipper.Y * (line_end.X - line_start.X)  - clipper.X * (line_end.Y - line_start.Y);
			double scaleLine = a / b;
			//intersection
			intersection = line_start + (line * scaleLine);

			return (scaleLine <= 1 && scaleLine >= 0);
		}

		private static void Clip(LinkedList<Vector2D> subject, Vector2D clipper_a, Vector2D clipper_b)
		{
			if (subject.Count < 3)
				return;
			//is the first point of polyA in polyB?
			LinkedListNode<Vector2D> current = subject.First;
			LinkedListNode<Vector2D> next = current.Next;
			
			Vector2D normal = new Vector2D(clipper_b.Y - clipper_a.Y, clipper_a.X - clipper_b.X);
							
			Vector2D intersection = new Vector2D(0, 0);
			while (next != null)
			{
				//current inside?
				bool isCurrentIn = normal.Dot(current.Value - clipper_a) < 0;
				//next inside?
				bool isNextIn = normal.Dot(next.Value - clipper_a) < 0;

				//does current-next intersect with the clipping plane?
				bool intersected = GetIntersection(current.Value, next.Value, clipper_a, clipper_b, ref intersection);

				//	  Sutherland-Hodgman Polygon Clipping
				//polygon edge goes from outside clipping edge to outside clipping edge: remove current 
				if (!isCurrentIn && !isNextIn)
				{
					subject.Remove(current);
				}
				//polygon edge goes from outside clipping edge to inside clipping edge: remove current, add intersection
				else if (!isCurrentIn && isNextIn)
				{
					subject.AddAfter(current, intersection);
					subject.Remove(current);
				}
				//polygon edge goes from inside clipping edge to outside clipping edge: add intersection
				else if (isCurrentIn && !isNextIn)
				{
					subject.AddAfter(current, intersection);
				}
				 
				//polygon edge goes from inside clipping edge to inside clipping edge: do nothing
				current = next;
				next = next.Next;
			}

			subject.RemoveLast();
			if(subject.Count > 0)
				subject.AddLast(subject.First.Value);
		}
	}
}

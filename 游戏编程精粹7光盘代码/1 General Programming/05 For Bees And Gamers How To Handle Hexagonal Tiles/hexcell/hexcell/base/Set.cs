using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace hexcell
{
	public class Set<T> : ICollection<T>, ICloneable, ICollection
	{
		//Basing my set class on the more sophisticated dictionary is kinda retarded but it's reaonably fast and i just cba...
		//Big gz to the turds at M$ for forgetting the set class!^^
		private struct Dummy { }
		private static Dummy m_Dummy = new Dummy();
		private Dictionary<T, Dummy> m_Data;

		public Set()
		{
			m_Data = new Dictionary<T, Dummy>();
		}

		public Set(Set<T> original)
		{
			m_Data = new Dictionary<T, Dummy>(original.m_Data);
		}

		public Set(IEnumerable<T> original)
		{
			m_Data = new Dictionary<T, Dummy>();
			Add(original);
		}

		public int Count
		{
			get { return m_Data.Count; }
		}
		public bool IsReadOnly
		{
			get { return false; }
		}

		public IEnumerator<T> GetEnumerator()
		{
			return m_Data.Keys.GetEnumerator();
		}
				
		public bool Contains(T item)
		{
			return m_Data.ContainsKey(item);
		}

		public void CopyTo(T[] array, int index)
		{
			m_Data.Keys.CopyTo(array, index);
		}

		public Set<T> Clone()
		{
			return new Set<T>(this);
		}

		// Adds a new item to the set. If the set already contains an item equal to
		public void Add(T item)
		{
			m_Data[item] = m_Dummy;
		}
		
		// Adds all the items in <paramref name="collection"/> to the set. Override duplicates
		public void Add(IEnumerable<T> collection)
		{
			if (collection == null)
				throw new ArgumentNullException("collection");

			// If we're adding ourselves, then there is nothing to do.
			if (object.ReferenceEquals(collection, this))
				return;

			foreach (T item in collection)
				Add(item);
		}

		//Searches the set for item and if found removes it.
		public bool Remove(T item)
		{
			return m_Data.Remove(item);
		}

		//Searches the set for items in collection and if found removes them. return nr of removed items
		public int Remove(IEnumerable<T> collection)
		{
			if (collection == null)
				throw new ArgumentNullException("collection");

			int count = 0;

			if (collection == this)
			{
				count = Count;
				Clear();            // special case, otherwise we will throw.
			}
			else
			{
				foreach (T item in collection)
				{
					if (Remove(item))
						++count;
				}
			}
			return count;
		}

		// Removes all items from the set.
		public void Clear()
		{
			m_Data = new Dictionary<T, Dummy>();
		}

		public Set<U> ConvertAll<U>(Converter<T, U> converter)
		{
			Set<U> result = new Set<U>();
			foreach (T element in this)
				result.Add(converter(element));
			return result;
		}
		public bool TrueForAll(Predicate<T> predicate)
		{
			foreach (T element in this)
				if (!predicate(element))
					return false;
			return true;
		}
		public Set<T> FindAll(Predicate<T> predicate)
		{
			Set<T> result = new Set<T>();
			foreach (T element in this)
				if (predicate(element))
					result.Add(element);
			return result;
		}

		// Determines if this set is a superset of another set. Neither set is modified.
		public bool IsSupersetOf(Set<T> otherSet)
		{
			if (otherSet.Count > this.Count)
				return false;     // Can't be a superset of a bigger set

			// Check each item in the other set to make sure it is in this set.
			foreach (T item in otherSet)
			{
				if (!this.Contains(item))
					return false;
			}

			return true;
		}

		// Determines if this set is a subset of another set. Neither set is modified.
		public bool IsSubsetOf(Set<T> otherSet)
		{
			return otherSet.IsSupersetOf(this);
		}

		// Determines if this set is equal to another set. This set is equal to
		public bool IsEqualTo(Set<T> otherSet)
		{
			// Must be the same size.
			if (otherSet.Count != this.Count)
				return false;

			// Check each item in the other set to make sure it is in this set.
			foreach (T item in otherSet)
				if (!this.Contains(item))
					return false;

			return true;
		}

		// Determines if this set is disjoint from another set. Two sets are disjoint
		// if no item from one set is equal to any item in the other set.
		public bool IsDisjointFrom(Set<T> otherSet)
		{
			Set<T> smaller = (otherSet.Count > this.Count) ? this : otherSet;
			Set<T> larger = (otherSet.Count > this.Count) ? otherSet : this;

			foreach (T item in smaller)
				if (larger.Contains(item))
					return false;

			return true;
		}

		/// Computes the union of this set with another set. The union of two sets
		/// is all items that appear in either or both of the sets. This set receives
		/// the union of the two sets, the other set is unchanged.
		public void UnionWith(Set<T> otherSet)
		{
			Add(otherSet);
		}

		// Computes the union of this set with another set. A new set is 
		// created with the union of the sets and is returned. This set and the other set 
		public Set<T> Union(Set<T> otherSet)
		{
			Set<T> smaller = (otherSet.Count > this.Count) ? this : otherSet;
			Set<T> larger = (otherSet.Count > this.Count) ? otherSet : this;

			Set<T> result = larger.Clone();
			result.Add(smaller);
			return result;
		}

		// Intersection of this set with another set. The intersection of two sets
		// is all items that appear in both of the sets. This set receives the intersection of the 
		// two sets, the other set is unchanged.
		public void IntersectWith(Set<T> otherSet)
		{
			Set<T> newSet = Intersection(otherSet);
			m_Data = newSet.m_Data;
		}

		// Intersection of this set with another set. A new set is created with the intersection.
		// This set and the other set are unchanged.
		public Set<T> Intersection(Set<T> otherSet)
		{
			Set<T> smaller = (otherSet.Count > this.Count) ? this : otherSet;
			Set<T> larger = (otherSet.Count > this.Count) ? otherSet : this;

			Set<T> result = new Set<T>();
			foreach (T item in smaller)
			{
				if (larger.Contains(item))
					result.Add(item);
			}

			return result;
		}

		// Computes the difference of this set with another set. The difference of two sets
		// are all items that appear in this set but not in the other set. This set receives
		// the difference of the two sets; the other set is unchanged.
		public void DifferenceWith(Set<T> otherSet)
		{
			// Difference with self is nothing. 
			if (this == otherSet)
				Clear();

			foreach (T item in otherSet)
				this.Remove(item);
		}

		// Computes the difference of this set with another set. A new set is returned with the difference 
		// of the sets. This set and the other set remain unchanged.
		public Set<T> Difference(Set<T> otherSet)
		{
			Set<T> result = this.Clone();
			result.DifferenceWith(otherSet);
			return result;
		}

		// Computes the symmetric difference of this set with another set. The symmetric difference of two sets
		// is all items that appear in either of the sets, but not both. This set receives
		// the symmetric difference of the two sets; the other set is unchanged.
		public void SymmetricDifferenceWith(Set<T> otherSet)
		{
			if (this == otherSet)
				Clear();

			foreach (T item in otherSet)
				if (this.Contains(item))
					this.Remove(item);
				else
					this.Add(item);
		}

		// Computes the symmetric difference of this set with another set. A new set is created with the symmetric 
		// difference of the sets and is returned. This set and the other set are unchanged.
		public Set<T> SymmetricDifference(Set<T> otherSet)
		{
			Set<T> smaller = (otherSet.Count > this.Count) ? this : otherSet;
			Set<T> larger = (otherSet.Count > this.Count) ? otherSet : this;

			Set<T> result = larger.Clone();
			foreach (T item in smaller)
				if (result.Contains(item))
					result.Remove(item);
				else
					result.Add(item);

			return result;
		}

		#region Interfaces

		void ICollection.CopyTo(Array array, int index)
		{
			((ICollection)m_Data.Keys).CopyTo(array, index);
		}
		object ICollection.SyncRoot
		{
			get { return ((ICollection)m_Data.Keys).SyncRoot; }
		}
		bool ICollection.IsSynchronized
		{
			get { return ((ICollection)m_Data.Keys).IsSynchronized; }
		}
		IEnumerator IEnumerable.GetEnumerator()
		{
			return ((IEnumerable)m_Data.Keys).GetEnumerator();
		}
		object ICloneable.Clone()
		{
			return Clone();
		}

		#endregion

		#region Operators

		/*
		Difference	 			- 		A - B returns a Set containing all the elements of A, with the elements from B removed.
		Union 					+		A + B returns a Set containing all the elements from both input sets.
		Intersect 				& 		A & B returns a Set containing all the elements that are in both A and B.
		SymmetricDifference 	^ 		A ^ B returns a Set containing the elements that are in A or in B, but are not in both A and B. (xor)
		*/

		public static Set<T> operator +(Set<T> s1, Set<T> s2)
		{
			return s1.Union(s2);
		}
		public static Set<T> operator -(Set<T> s1, Set<T> s2)
		{
			return s1.Difference(s2);
		}
		public static Set<T> operator &(Set<T> s1, Set<T> s2)
		{
			return s1.Intersection(s2);
		}
		public static Set<T> operator ^(Set<T> s1, Set<T> s2)
		{
			return s1.SymmetricDifference(s2);
		}

		/*
		Equals		 			== 		A == B returns true if A and B containe the same elements.
		IsSuperset				>=		A >= B returns true if all elements in B are also in A
		IsSubset 				<=		A <= B returns true if all elements in A are also in B
		IsDisjoint			 	!= 		A != B returns true if no elements are contained in A and B
		*/

		public static bool operator >=(Set<T> s1, Set<T> s2)
		{
			return s1.IsSupersetOf(s2);
		}

		public static bool operator <=(Set<T> s1, Set<T> s2)
		{
			return s1.IsSubsetOf(s2);
		}

		//public static bool operator !=(Set<T> s1, Set<T> s2)
		//{
		//    return s1.IsDisjointFrom(s2);
		//}

		//public static bool operator ==(Set<T> s1, Set<T> s2)
		//{
		//    return s1.IsEqualTo(s2);
		//}

		#endregion

	}

}

using System;
using System.Collections.Generic;
using System.Text;

namespace hexcell
{
	enum Neighbourhoods :ulong 
	{
		Six = 0x7f,
		Star = 0x5557f,
		Far = 0x55501,
		Asterisk = 0x2aaff,
		Eighteen = 0x7ffff
	}

	public class SixNeighbours<DataType, Address> : CustomNeighbourhoodCells<DataType, Address>
	{
		public SixNeighbours(AddressingScheme<DataType, Address> targetScheme, Address originCell) 
			: base(targetScheme, originCell, (ulong)Neighbourhoods.Six)
		{}
	}

	public class StarNeighbours<DataType, Address> : CustomNeighbourhoodCells<DataType, Address>
	{
		public StarNeighbours(AddressingScheme<DataType, Address> targetScheme, Address originCell)
			: base(targetScheme, originCell, (ulong)Neighbourhoods.Star)
		{}
	}

	public class FarNeighbours<DataType, Address> : CustomNeighbourhoodCells<DataType, Address>
	{
		public FarNeighbours(AddressingScheme<DataType, Address> targetScheme, Address originCell)
			: base(targetScheme, originCell, (ulong)Neighbourhoods.Far)
		{ }
	}

	public class EigtheenNeighbours<DataType, Address> : CustomNeighbourhoodCells<DataType, Address>
	{
		public EigtheenNeighbours(AddressingScheme<DataType, Address> targetScheme, Address originCell)
			: base(targetScheme, originCell, (ulong)Neighbourhoods.Eighteen)
		{ }
	}

	public class AsteriskNeighbours<DataType, Address> : CustomNeighbourhoodCells<DataType, Address>
	{
		public AsteriskNeighbours(AddressingScheme<DataType, Address> targetScheme, Address originCell)
			: base(targetScheme, originCell, (ulong)Neighbourhoods.Asterisk)
		{ }
	}
}

using System.Numerics;

namespace UnitTests.Domain
{
	public class TestSet<X> where X : INumber<X>
	{
		public List<GetInsertIndexBSTestSet<X>> GetInsertIndexBSTestSets { get; set; } = new();
		public List<AddElementToSortedArrayTestSet<X>> AddElementToSortedArrayTestSets { get; set; } = new();
		public List<SortTestSet<X>> SortTestSets { get; set; } = new();
	}
}

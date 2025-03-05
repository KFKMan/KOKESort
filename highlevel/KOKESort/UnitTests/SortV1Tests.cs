using KOKESort;
using System.Data;
using System.Numerics;
using System.Text.Json;
using UnitTests.Domain;

namespace UnitTests
{
	[TestFixture(typeof(sbyte))]
	[TestFixture(typeof(byte))]
	[TestFixture(typeof(ushort))]
	[TestFixture(typeof(short))]
	[TestFixture(typeof(uint))]
	[TestFixture(typeof(int))]
	[TestFixture(typeof(long))]
	[TestFixture(typeof(ulong))]
	[TestFixture(typeof(float))]
	[TestFixture(typeof(double))]
	[TestFixture(typeof(UInt128))]
	[TestFixture(typeof(Int128))]
	public class SortV1Tests<T> where T : INumber<T>
	{
		public TestSet<T> Dataset;

		[SetUp]
		public void Setup()
		{
			string file = Path.Combine("Dataset",nameof(GetInsertIndexBSTest) + typeof(T).Name + ".txt");

			if (!File.Exists(file))
			{
				Assert.Fail($"File not found, {file}");
			}

			string data = File.ReadAllText(file);
			var dataset = JsonSerializer.Deserialize<TestSet<T>>(data);

			if (dataset == null)
			{
				Assert.Fail($"File can't be deserialized, {file}");
			}
			Dataset = dataset;
		}

		[Test]
		public void GetInsertIndexBSTest()
		{
			foreach (var testSet in Dataset.GetInsertIndexBSTestSets)
			{
				int result = SortV1<T>.GetInsertIndexBS(testSet.Array, testSet.Array.Length, testSet.Element);
				Assert.That(result == testSet.expectedValue);
			}
		}

		[Test]
		public void AddElementToSortedArrayTest()
		{
			foreach (var testSet in Dataset.AddElementToSortedArrayTestSets)
			{
				SortV1<T>.AddElementToSortedArray(testSet.Array, testSet.Array.Length, testSet.Element);
				Assert.That(testSet.Array.SequenceEqual(testSet.expectedValue));
			}
		}

		[Test]
		public void SortTest()
		{
			foreach (var testSet in Dataset.SortTestSets)
			{
				var result = SortV1<T>.Sort(testSet.Array);
				Assert.That(result.SequenceEqual(testSet.expectedValue));
			}
		}
	}
}

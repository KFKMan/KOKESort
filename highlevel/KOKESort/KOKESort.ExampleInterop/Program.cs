void PrintArray(int[] arr)
{
	foreach (var element in arr)
	{
		Console.Write(element.ToString() + " ");
	}
	Console.WriteLine();
}

int[] arr = new int[]
{
	5, 3, 12, 9, 15, 24, 18, 60, 35, 36, 39, 21
};

using (var sortedArr = Interop.SortV1(arr))
{
	Console.WriteLine("Unsorted Array;");
	PrintArray(arr);

	Console.WriteLine("Sorted Array;");
	PrintArray(sortedArr.Data);

	Console.WriteLine("Press any key for exit");
	Console.ReadKey();
}
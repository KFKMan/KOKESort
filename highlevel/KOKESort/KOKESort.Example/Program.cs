using KOKESort;

void PrintArray(int[] arr)
{
	foreach(var element in arr)
	{
		Console.Write(element.ToString() + " ");
	}
	Console.WriteLine();
}

int[] arr = new int[]
{
	5, 3, 12, 9, 15, 24, 18, 60, 35, 36, 39, 21
};

var sortedArr = SortV1<int>.Sort(arr);

Console.WriteLine("Unsorted Array;");
PrintArray(arr);

Console.WriteLine("Sorted Array;");
PrintArray(sortedArr);

Console.WriteLine("Press any key for exit");
Console.ReadKey();
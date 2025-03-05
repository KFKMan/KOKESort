using System.Runtime.InteropServices;

public class Interop
{
	[DllImport("KOKESort",EntryPoint = "SortV1")]
	private static extern nint SortV1([MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)] int[] unsortedArray,
		ulong unsortedArrayLength);

	[DllImport("KOKESort", EntryPoint = "FreeMemory")]
	public static extern void FreeMemory(nint pointer);

	public static PointerData<int[]> SortV1(int[] unsortedArray)
	{
		var ptr = SortV1(unsortedArray, (ulong)unsortedArray.LongLength);

		if(ptr == nint.Zero)
		{
			throw new ArgumentException("Result is NULL Pointer");
		}

		var ptrData = new PointerData<int[]>(ptr);

		ptrData.Data = new int[unsortedArray.Length];

		Marshal.Copy(ptr, ptrData.Data, 0, unsortedArray.Length);

		return ptrData;
	}
}

public class PointerData<T> : IDisposable
{
	public PointerData(nint ptr)
	{
		Pointer = ptr;
	}

	public T Data { get; set; }
	public nint Pointer { get; private set; }

	public void Dispose()
	{
		Interop.FreeMemory(Pointer);
	}
}

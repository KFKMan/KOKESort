using System.Numerics;

namespace KOKESort
{
	public static class SortV1<T> where T : INumber<T>
	{
		public static T[] Sort(IList<T> array)
		{
			int size = array.Count;

			if (size < 1)
			{
				T[] arr = new T[1];
				return arr;
			}

			T[] sortedArray = new T[size];
			int arraySize = 0;
			for (int i = 0; i < size; i++)
			{
				T element = array[i];
				AddElementToSortedArray(sortedArray, arraySize, element);
				arraySize++;
			}

			return sortedArray;
		}

		public static void AddElementToSortedArray(IList<T> sortedArray, int arraySize, T element)
		{
			int insertIndex = GetInsertIndexBS(sortedArray, arraySize, element);
			int currentIndex = arraySize;
			while (currentIndex > insertIndex)
			{
				sortedArray[currentIndex] = sortedArray[currentIndex - 1];
				currentIndex--;
			}
			sortedArray[insertIndex] = element;
		}

		public static int GetInsertIndexBS(IList<T> sortedArray, int arraySize, T element)
		{
			int left = 0;
			int right = arraySize;
			while (left < right)
			{
				int mid = left + (right - left) / 2;
				if (sortedArray[mid] < element)
				{
					left = mid + 1;
				}
				else
				{
					right = mid;
				}
			}
			return left;
		}


	}
}

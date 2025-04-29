using ClangSharp.Interop;
using System.Runtime.InteropServices;
using ClangSharp;

public static class MutatedFileWriter
{
	public static void ApplyMutations(string filePath, string outTemplate, List<Mutation> mutations)
	{
		string originalSource = File.ReadAllText(filePath);

		int index = 0;

		foreach (var (range, kind) in mutations.Select(x => (x.Range, x.Kind)))
		{
			var sourceCopy = originalSource;

			uint start = range.Start.GetFileOffset();
			uint end = range.End.GetFileOffset();

			string mutationText = GetReplacement(sourceCopy[(int)start..(int)end], kind);

			var mutated = sourceCopy.Remove((int)start, (int)(end - start))
									.Insert((int)start, mutationText);

			var mutatedPath = filePath
				.Replace("{name}", Path.GetFileNameWithoutExtension(filePath))
				.Replace("{ext}", Path.GetExtension(filePath))
				.Replace("{index}", index.ToString())
				.Replace("{kind}", kind.ToString());

			File.WriteAllText(mutatedPath, mutated);
			Console.WriteLine($"Generated: {mutatedPath}");

			index++;
		}
	}

	private static uint GetOffset(CXSourceLocation location)
	{
		location.GetExpansionLocation(out var file, out var line, out var col, out var offset);
		return offset;
	}

	private static string GetReplacement(string original, MutationKind kind) => kind switch
	{
		MutationKind.EqualityToInequality => original.Replace("==", "!="),
		MutationKind.InequalityToEquality => original.Replace("!=", "=="),
		MutationKind.GreaterToLessEqual => original.Replace(">", "<="),
		MutationKind.LessToGreaterEqual => original.Replace("<", ">="),
		_ => original
	};
}
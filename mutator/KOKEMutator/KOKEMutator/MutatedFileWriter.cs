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

			range.Start.GetExpansionLocation(out var _, out var _, out var _, out var start);
			range.End.GetExpansionLocation(out var _, out var _, out var _, out var end);

			string mutationText = GetReplacement(sourceCopy[(int)start..(int)end], kind);

			var mutated = sourceCopy.Remove((int)start, (int)(end - start))
									.Insert((int)start, mutationText);

			var mutatedPath = outTemplate
				.Replace("{name}", Path.GetFileNameWithoutExtension(filePath))
				.Replace("{ext}", Path.GetExtension(filePath))
				.Replace("{index}", index.ToString())
				.Replace("{kind}", kind.ToString());

			File.WriteAllText(mutatedPath, mutated);
			Console.WriteLine($"Generated: {mutatedPath}");

			index++;
		}
	}

	private static string GetReplacement(string original, MutationKind kind) => kind switch
	{
		// Compare Operators
		MutationKind.EqualityToInequality => original.Replace("==", "!="),
		MutationKind.InequalityToEquality => original.Replace("!=", "=="),
		MutationKind.GreaterToLessEqual => original.Replace(">", "<="),
		MutationKind.LessToGreaterEqual => original.Replace("<", ">="),
		_ => original
	};
}
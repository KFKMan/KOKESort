using ClangSharp.Interop;

public unsafe class Mutator : IDisposable
{
	private static readonly Dictionary<string, MutationKind> BinaryMutations = new()
	{
		// Compare Opts.
		["=="] = MutationKind.EqualityToInequality,         // == → !=
		["!="] = MutationKind.InequalityToEquality,         // != → ==
		[">"] = MutationKind.GreaterToLessEqual,           // > → <=
		["<"] = MutationKind.LessToGreaterEqual,           // < → >=
		[">="] = MutationKind.GreaterEqualToLess,           // >= → <
		["<="] = MutationKind.LessEqualToGreater,           // <= → >

		// Mathematical Opts.
		["+"] = MutationKind.PlusToMinus,                  // + → -
		["-"] = MutationKind.MinusToPlus,                  // - → +
		["*"] = MutationKind.MultiplyToDivide,             // * → /
		["/"] = MutationKind.DivideToMultiply,             // / → *

		// Logical Opts.
		["&&"] = MutationKind.AndToOr,                      // && → ||
		["||"] = MutationKind.OrToAnd,                      // || → &&

		// Bitwise Opts.
		["&"] = MutationKind.BitwiseAndToOr,               // & → |
		["|"] = MutationKind.BitwiseOrToAnd,               // | → &

		// Increment/Decrement Opts.
		["++"] = MutationKind.IncrementToDecrement,         // ++ → --
		["--"] = MutationKind.DecrementToIncrement,         // -- → ++
	};


	private CXTranslationUnit _tu;

	public Mutator(string filePath)
	{
		using var index = CXIndex.Create();

		string[] args = new[] { "-std=c11" };

		_tu = CXTranslationUnit.Parse(index, filePath, args, Array.Empty<CXUnsavedFile>(), CXTranslationUnit_Flags.CXTranslationUnit_None);
	}

	public List<Mutation> FindMutations()
	{
		var mutations = new List<Mutation>();
		var cursor = clang.getTranslationUnitCursor(_tu);

		Visit(cursor);

		void Visit(CXCursor node)
		{
			if (node.kind == CXCursorKind.CXCursor_BinaryOperator)
			{
				var tokens = node.TranslationUnit.Tokenize(node.Extent);
				foreach (var token in tokens)
				{
					var spelling = token.GetSpelling(_tu).ToString();
					if (BinaryMutations.TryGetValue(spelling, out var mutationKind))
					{
						mutations.Add(new Mutation(token.GetExtent(_tu), mutationKind));
						Console.WriteLine($"Found mutation: {spelling} -> {mutationKind}");
					}
				}
			}

			int numChildren = clang.Cursor_getNumArguments(node);
			for (uint i = 0; i < numChildren; ++i)
			{
				var child = clang.Cursor_getArgument(node, i);
				Visit(child);
			}

			node.VisitChildren((child, parent, data) =>
			{
				Visit(child);
				return CXChildVisitResult.CXChildVisit_Continue;
			}, default);
		}

		return mutations;
	}

	public void Dispose()
	{
		clang.disposeTranslationUnit(_tu);
	}
}

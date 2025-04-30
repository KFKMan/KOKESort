using ClangSharp;
using ClangSharp.Interop;
using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;

namespace KOKEMutator
{
	public class MutationTool
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

		private readonly string _inputFilePath;
		private readonly string OutTemplate;

		public MutationTool(string inputFilePath, string outTemplate)
		{
			_inputFilePath = inputFilePath;
			OutTemplate = outTemplate;
		}

		public void Run()
		{
			using var index = CXIndex.Create();
			var translationUnit = CXTranslationUnit.Parse(index, _inputFilePath, Array.Empty<string>(), Array.Empty<CXUnsavedFile>(), CXTranslationUnit_Flags.CXTranslationUnit_None);

			if (translationUnit == null)
			{
				Console.WriteLine("File Analysis Error");
				return;
			}

			var cursor = translationUnit.Cursor;
			var mutations = CollectMutations(cursor);

			Console.WriteLine($"{mutations.Count} mutation found");
			Console.WriteLine($"Applying mutations....");

			MutatedFileWriter.ApplyMutations(_inputFilePath, OutTemplate, mutations);

			Console.WriteLine("Mutations Applied");
		}

		private unsafe CXChildVisitResult VisitHandlerBody(CXCursor child, CXCursor parent, void* client_data)
		{
			if (child.Kind == CXCursorKind.CXCursor_BinaryOperator ||
				child.Kind == CXCursorKind.CXCursor_UnaryOperator ||
				child.Kind == CXCursorKind.CXCursor_CompoundAssignOperator)
			{
				var sourceRange = child.Extent;
				var originalCode = GetSourceCode(sourceRange);

				Console.WriteLine($"Processing -> {originalCode}");

				foreach (var (op, kind) in BinaryMutations)
				{
					if (originalCode.Contains(op))
					{
						mutations.Add(new Mutation(sourceRange, kind));
					}
				}
			}

			return CXChildVisitResult.CXChildVisit_Recurse;
		}

		private unsafe CXChildVisitResult VisitHandler(CXCursor child, CXCursor parent, void* client_data)
		{
			// Library fonksiyonlarını hariç tut
			if ((child.Kind == CXCursorKind.CXCursor_FunctionDecl ||
				child.Kind == CXCursorKind.CXCursor_StructDecl ||
				child.Kind == CXCursorKind.CXCursor_TypedefDecl ||
				child.Kind == CXCursorKind.CXCursor_VarDecl) && !IsFromMainFile(child))
			{
				var functionName = child.Spelling.ToString();
				//Console.WriteLine($"Skipping library function: {functionName}");
				return CXChildVisitResult.CXChildVisit_Continue;
			}

			// Binary, Unary ve Compound Assignment operatörlerini kontrol et
			if (child.Kind == CXCursorKind.CXCursor_FunctionDecl)
			{
				Console.WriteLine($"Function Declaration Found, Processing Child");
				child.VisitChildren(VisitHandlerBody, new CXClientData((nint)client_data));
			}
			else
			{
				var sourceRange = child.Extent;
				var originalCode = GetSourceCode(sourceRange);
				Console.WriteLine($"Not Processing [{child.Kind.ToString()}] -> {originalCode}");
			}

			return CXChildVisitResult.CXChildVisit_Continue;
		}

		List<Mutation> mutations = new List<Mutation>();

		private unsafe List<Mutation> CollectMutations(CXCursor cursor)
		{
			mutations = new List<Mutation>();

			var userDefinedFunctions = new HashSet<string>();

			cursor.VisitChildren(VisitHandler, new CXClientData(nint.Zero));

			return mutations;
		}

		private bool IsFromMainFile(CXCursor cursor)
		{
			cursor.Location.GetFileLocation(out CXFile file, out _, out _, out _);
			return file.Name.CString == _inputFilePath;
		}

		private string GetSourceCode(CXSourceRange range)
		{
			range.Start.GetFileLocation(out CXFile startFile, out uint startLine, out uint startColumn, out uint startOffset);
			range.End.GetFileLocation(out CXFile endFile, out uint endLine, out uint endColumn, out uint endOffset);

			if (startFile.Handle == endFile.Handle)
			{
				var filePath = startFile.Name.CString;
				var fileContent = File.ReadAllText(filePath);
				return fileContent.Substring((int)startOffset, (int)(endOffset - startOffset));
			}

			return string.Empty;
		}
	}
}
using ClangSharp.Interop;

public record Mutation(CXSourceRange Range, MutationKind Kind);

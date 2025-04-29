using ClangSharp.Interop;

public static class ClangSharpExtensions
{
	public static uint GetFileOffset(this CXSourceLocation sourceLoc)
	{
		sourceLoc.GetFileLocation(out var file, out var line, out var col, out var offset);
		return offset;
	}
}

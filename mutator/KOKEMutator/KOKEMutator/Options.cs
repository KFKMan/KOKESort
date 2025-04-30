using CommandLine;

public class Options
{
	[Option('f', "files", Required = false, HelpText = "C Files To Process | Use FilePaths or DirectoryPath not both !", Separator = ';')]
	public IEnumerable<string>? FilePaths { get; set; }

	[Option('d', "directory", Required = false, HelpText = "Folder which have C files to process | Use FilePaths or DirectoryPath not both !")]
	public string? DirectoryPath { get; set; }

	[Option('o', "output", Required = false, HelpText = "Output Path Template {name} for filename, {ext} for file extension, {index} for index, {kind} for type | Use FileOutPaths or OutputPath not both !")]
	public string? OutputTemplate { get; set; }
}


using CommandLine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KOKEMutator
{
	public static class AppMain
	{
		public static int ExitCode = 0;

		public static void Exit(int exitCode)
		{
			ExitCode = exitCode;
		}

		public static void Run(string[] args)
		{
			Parser.Default.ParseArguments<Options>(args)
				.WithParsed(RunOptions)
				.WithNotParsed(RunOptionsError);
		}

		public static void RunOptions(Options opt)
		{
			List<string> GetFilesToProcess(Options opt)
			{
				List<string> files = new List<string>();

				if(opt.FilePaths != null && opt.FilePaths.Count() != 0)
				{
					files.AddRange(opt.FilePaths);
				}

				if(opt.DirectoryPath != null)
				{
					files.AddRange(new DirectoryInfo(opt.DirectoryPath).GetFiles().Select(x => x.FullName));
				}

				return files;
			}

			var filesToProcess = GetFilesToProcess(opt);

			if (filesToProcess.Count == 0)
			{
				Console.WriteLine("You need to specify files to process.");
				Exit(101);
				return;
			}

			if(opt.OutputTemplate == null)
			{
				Console.WriteLine("You need to specify output paths.");
				Exit(102);
				return;
			}

			foreach(var file in filesToProcess)
			{
				var fileName = Path.GetFileName(file);

				Console.WriteLine($"Processing {fileName} | {file}");

				Console.WriteLine("Finding Mutations...");
				Mutator mutator = new Mutator(file);
				var mutations = mutator.FindMutations();
				Console.WriteLine($"{mutations.Count} mutation found");

				Console.WriteLine("Applying Mutations...");
				MutatedFileWriter.ApplyMutations(file, opt.OutputTemplate, mutations);
				Console.WriteLine("Mutations Applied");

				Console.WriteLine($"Processed {fileName} | {file}");
			}
		}

		public static void RunOptionsError(IEnumerable<Error> errors)
		{
			Console.WriteLine("Parsing Error Accoured.");
			Console.WriteLine("Errors;");
			foreach(var error in errors)
			{
				Console.WriteLine(error.ToString());
			}
			Exit(100);
		}
	}
}

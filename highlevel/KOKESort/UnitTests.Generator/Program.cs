using System.Reflection;
using UnitTests.Domain;


T[] ReadArray<T>(Func<string, T?> parser)
{
	List<T> list = new List<T>();
	while (true)
	{
		var input = GetInput("Write next element or exit with writing 'exit': ");

		if(input.ToLower().Trim() == "exit")
		{
			break;
		}	

		var val = parser(input);

		if(val == null)
		{
			Console.WriteLine("Value can't be parsed.");
			continue;
		}

		list.Add(val);
	}

	return list.ToArray();
}

string GetInput(string question)
{
	Console.Write(question);
	var response = Console.ReadLine();
	if(response == null)
	{
		return GetInput(question);
	}
	return response;
}

Type? TryGetType(string typeStr)
{
	Type? cType = null;
	cType = Type.GetType("System." + typeStr);
	
	if(cType != null)
	{
		return cType;
	}

	cType = Type.GetType(typeStr);

	return cType;
}

while (true)
{
	var VariableTypeStr = GetInput("Input Variable Type (like Int32, Int64...) : ");
	Type? VariableType = TryGetType(VariableTypeStr);

	if (VariableType == null)
	{
		Console.WriteLine("Type not found");
		Console.ReadKey();
		return;
	}

	var testSetType = typeof(TestSet<>).MakeGenericType(VariableType);
	var testSet = Activator.CreateInstance(testSetType);


}



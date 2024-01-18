using UnrealBuildTool;
using System.IO;

public class NLP : ModuleRules
{
	private string ModulePath
	{
		get { return ModuleDirectory; }
	}

	public NLP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new []
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore"
		});
		//PrivateDependencyModuleNames.AddRange();
		
		PublicIncludePaths.AddRange(new string[]
		{
			Path.Combine(ModulePath,"../cppjieba/include"),
			Path.Combine(ModulePath,"../cppjieba/deps/limonp/include"),
			Path.Combine(ModulePath, "../cpp-httplib"),
			Path.Combine(ModulePath, "../rapidjson"),
			Path.Combine(ModulePath, "../nlohmann"),
			Path.Combine(ModulePath, "../StringFormat"),
			"E:/Anaconda/envs/NLPFORUE/include"
		});
		PublicAdditionalLibraries.AddRange(new string[]
		{
			Path.Combine(ModulePath,"../SQLite3/sqlite3.lib"),
			"E:/Anaconda/envs/NLPFORUE/libs/_tkinter.lib",
			"E:/Anaconda/envs/NLPFORUE/libs/python3.lib",
			"E:/Anaconda/envs/NLPFORUE/libs/python39.lib"
		});
	}
}
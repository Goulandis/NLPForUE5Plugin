using UnrealBuildTool;
using System.IO;

public class NLP : ModuleRules
{
	private string ModulePath
	{
		get { return ModuleDirectory; }
	}

	// private string CppJiebaPath
	// {
	// 	get { return Path.GetFullPath(Path.Combine(ModulePath, "../cppjieba")); }
	// }

	public NLP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new []
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
		});
		//PrivateDependencyModuleNames.AddRange();
		
		PublicIncludePaths.AddRange(new string[]
		{
			Path.Combine(ModulePath,"../cppjieba/include"),
			Path.Combine(ModulePath,"../cppjieba/deps/limonp/include"),
			Path.Combine(ModulePath, "../cpp-httplib"),
			//Path.Combine(ModulePath, "../jsoncpp/src"),
			//Path.Combine(ModulePath, "../jsoncpp/include/json"),
			Path.Combine(ModulePath, "../jsoncpp2"),
			Path.Combine(ModulePath, "../jsoncpp2/json")
		});
	}
}
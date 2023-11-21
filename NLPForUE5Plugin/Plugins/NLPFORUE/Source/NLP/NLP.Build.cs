using UnrealBuildTool;
using System.IO;

public class NLP : ModuleRules
{
	private string ModulePath
	{
		get { return ModuleDirectory; }
	}

	private string CppJiebaPath
	{
		get { return Path.GetFullPath(Path.Combine(ModulePath, "../cppjieba")); }
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
			Path.Combine(CppJiebaPath,"include"),
			Path.Combine(CppJiebaPath,"deps/limonp/include"),
			Path.Combine(ModulePath, "../cpp-httplib")
		});
	}
}
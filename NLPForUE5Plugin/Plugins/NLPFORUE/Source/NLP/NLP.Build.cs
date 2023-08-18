using UnrealBuildTool;
using System.IO;

public class NLP : ModuleRules
{
	private string ModulePath
	{
		get { return ModuleDirectory; }
	}

	private string ThirdPartyPath
	{
		get { return Path.GetFullPath(Path.Combine(ModulePath, "../ThirdParty")); }
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
			Path.Combine(ThirdPartyPath,"cppjieba/include"),
			Path.Combine(ThirdPartyPath,"cppjieba/deps/limonp/include")
		});
	}
}
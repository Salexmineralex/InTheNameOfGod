// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class InTheNameOfGod : ModuleRules
{
	public InTheNameOfGod(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "GameplayTasks","Niagara","UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] { "EnhancedInput", "EnhancedInput", "EnhancedInput","Slate","SlateCore" });

		PrivateIncludePaths.AddRange(new string[] { "InTheNameOfGod"}); // Add the path to your custom node class file

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

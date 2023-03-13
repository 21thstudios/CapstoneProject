// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class CapstoneProject : ModuleRules
{
	public CapstoneProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OnlineSubsystem", "OnlineSubsystemUtils" });

		// Uncomment if you are using online features
		PrivateDependencyModuleNames.Add("OnlineSubsystem");

		DynamicallyLoadedModuleNames.AddRange(new string[] { "OnlineSubsystemNull", "OnlineSubsystemSteam" });
	}
}

// Copyright Ludens Studio

using UnrealBuildTool;
using System.Collections.Generic;

public class LokiTarget : TargetRules
{
	public LokiTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "Loki" } );
	}
}

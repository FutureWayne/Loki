// Copyright Ludens Studio

using UnrealBuildTool;
using System.Collections.Generic;

public class LokiEditorTarget : TargetRules
{
	public LokiEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "Loki" } );
	}
}

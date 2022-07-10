// Shoot Them Up Game , All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;
using System;

public class ShootThemUpEditorTarget : TargetRules
{
	public ShootThemUpEditorTarget(TargetInfo Target) : base(Target)
	{
		Console.WriteLine("STU editor target ---------->");
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ShootThemUp" } );
	}
}

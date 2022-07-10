// Shoot Them Up Game , All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;
using System;

public class ShootThemUpTarget : TargetRules
{
	public ShootThemUpTarget(TargetInfo Target) : base(Target)
	{
		Console.WriteLine("STU game target ---------->");
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "ShootThemUp" } );
	}
}

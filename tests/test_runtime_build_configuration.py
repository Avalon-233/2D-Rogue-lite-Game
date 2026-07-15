from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PROJECT = ROOT / "LightRogue" / "LightRogue.vcxproj"


def main() -> None:
    project = PROJECT.read_text(encoding="utf-8")

    assert project.count("<AdditionalOptions>/utf-8 %(AdditionalOptions)</AdditionalOptions>") == 4
    assert '<Target Name="CopySfmlRuntime"' in project
    assert 'AfterTargets="Build"' in project
    assert 'Condition="\'$(Platform)\'==\'x64\'"' in project
    assert 'Condition="\'$(Configuration)\'==\'Debug\'"' in project
    assert 'Condition="\'$(Configuration)\'==\'Release\'"' in project
    assert '<Copy SourceFiles="@(SfmlRuntimeDll)" DestinationFolder="$(OutDir)"' in project
    assert r'<ResourceFile Include="$(ProjectDir)..\resource\**\*.*" />' in project
    assert (
        r'<Copy SourceFiles="@(ResourceFile)" '
        r'DestinationFolder="$(OutDir)resource\%(RecursiveDir)" '
        r'SkipUnchangedFiles="true" />'
    ) in project


if __name__ == "__main__":
    main()

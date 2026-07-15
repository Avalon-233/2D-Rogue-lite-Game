from pathlib import Path


PROJECT = Path(__file__).resolve().parents[1] / "LightRogue" / "LightRogue.vcxproj"


def main() -> None:
    project = PROJECT.read_text(encoding="utf-8")
    assert "<VCProjectVersion>17.0</VCProjectVersion>" in project
    assert project.count("<PlatformToolset>v143</PlatformToolset>") == 4
    assert "<PlatformToolset>v145</PlatformToolset>" not in project


if __name__ == "__main__":
    main()

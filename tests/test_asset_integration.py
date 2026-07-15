from pathlib import Path

from PIL import Image


ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "LightRogue"
RESOURCE = ROOT / "resource"

ASSETS = {
    "player.png",
    "enemy_basic.png",
    "enemy_ranged.png",
    "enemy_bomber.png",
    "enemy_giant.png",
    "bullet_player.png",
    "bullet_enemy.png",
    "pickup_experience.png",
    "pickup_health.png",
    "effect_explosion.png",
}

COLLISION_ASSETS = ASSETS - {"effect_explosion.png"}


def main() -> None:
    missing = sorted(name for name in ASSETS if not (RESOURCE / name).is_file())
    assert not missing, f"missing asset files: {', '.join(missing)}"

    runtime_sources = "\n".join(
        path.read_text(encoding="utf-8")
        for path in (
            SOURCE / "Game.cpp",
            SOURCE / "GameWorld.cpp",
            SOURCE / "Player.cpp",
        )
    )

    unused = sorted(name for name in ASSETS if name not in runtime_sources)
    assert not unused, f"assets not loaded by runtime code: {', '.join(unused)}"

    assert "loadFromImage" not in runtime_sources, "placeholder color textures remain"

    padded = []
    for name in sorted(COLLISION_ASSETS):
        image = Image.open(RESOURCE / name).convert("RGBA")
        bounds = image.getchannel("A").getbbox()
        if bounds != (0, 0, image.width, image.height):
            padded.append(f"{name}: canvas={image.size}, alpha={bounds}")
    assert not padded, "transparent padding enlarges collision boxes:\n" + "\n".join(padded)


if __name__ == "__main__":
    main()

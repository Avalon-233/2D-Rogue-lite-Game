#pragma once

struct SaveData
{
	float HP = 100.f;
	float maxHP = 100.f;
	float position_x = 740.f;
	float position_y = 405.f;
	float speed = 100.f;
	float shootCooldown = 0.5f;
	float shootDamage = 10.f;
	float shootSpeed = 500.f;
	int projectileCount = 1;
	float experience = 0.f;
	float experienceNeedMultiplier = 1.f;
	int level = 1;
	int pendingUpgradeCount = 0;

	float gameTime = 0.f;
	int score = 0;
};
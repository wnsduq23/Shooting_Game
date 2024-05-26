#include "StageInfo.h"
#include "Rendering.h"

void Draw_Player()
{
	Sprite_Draw(player.x, player.y, '*');
	Sprite_Draw(player.x - 1, player.y, '-');
	Sprite_Draw(player.x + 1, player.y, '-');
	Sprite_Draw(player.x - 2, player.y, '<');
	Sprite_Draw(player.x + 2, player.y, '>');
}

void Draw_Enemy()
{
	for (const auto& enemy : g_stages[g_stage - 1].enemies)
	{
		Sprite_Draw(enemy.pos.x, enemy.pos.y, 'E');
	}
}


void Draw_Bullets()
{
	for (auto bullet : player_bullets) {
		if (bullet.y >= 0 && bullet.y < dfSCREEN_HEIGHT) {
			Sprite_Draw(bullet.x, bullet.y, '.');
		}
	}
	for (auto bullet : enemy_bullets) {
		if (bullet.y >= 0 && bullet.y < dfSCREEN_HEIGHT) {
			if (bullet.dx == 0)
				Sprite_Draw(bullet.x, bullet.y, '|');
			else
				Sprite_Draw(bullet.x, bullet.y, '*');
		}
	}
}

void Draw_UI()
{
	Sprite_Draw(dfSCREEN_WIDTH - 6, dfSCREEN_HEIGHT - 1, 'H');
	Sprite_Draw(dfSCREEN_WIDTH - 5, dfSCREEN_HEIGHT - 1, 'P');
	if (g_player_health >= 3)
		Sprite_Draw(dfSCREEN_WIDTH - 4, dfSCREEN_HEIGHT - 1, '*');
	if (g_player_health >= 2)
		Sprite_Draw(dfSCREEN_WIDTH - 3, dfSCREEN_HEIGHT - 1, '*');
	Sprite_Draw(dfSCREEN_WIDTH - 2, dfSCREEN_HEIGHT - 1, '*');
}

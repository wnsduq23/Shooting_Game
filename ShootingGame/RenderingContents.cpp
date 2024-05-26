#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "main.h"
#include "Console.h"
#include "Rendering.h"
#include "StageInfo.h"
#include <cmath>

int g_stage = 1;
int g_player_health = 3;
int g_score = 0;
g_stPosition player;
std::vector<g_stPosition> player_bullets;
std::vector<g_stPosition> enemy_bullets;


void	UpdateTitle()
{
	int idx = 0;
	const char* message = "Shooting Game!";
	std::string current_display = "";


	// Ű���� �Է�
	for (int key = 8; key <= 255; key++)
	{
		//���� �����ִ� Ű Ȯ�� 
		if (GetAsyncKeyState(key) & 0x8000)
		{
			g_stage = 1;
			cs_ClearScreen();
			cs_MoveCursor(0, 0);
			g_scene = LOAD;
		}
	}
	// �޴� ������
	while (idx < strlen(message))
	{
		clock_t	current_time = clock();
		if ((current_time / CLOCKS_PER_SEC) >= idx * 0.2)
		{
			current_display += message[idx];
			//cs_ClearScreen();
			//cs_MoveCursor((dfSCREEN_WIDTH - strlen(message)) / 2, dfSCREEN_HEIGHT / 2);
			Buffer_Clear();
			int x = (dfSCREEN_WIDTH - current_display.length()) / 2;
			int y = dfSCREEN_HEIGHT / 2;
			for (int i = 0; i < current_display.length(); i++)
			{
				Sprite_Draw(x + i, y, current_display[i]);
			}
			Buffer_Flip();
			idx++;
		}
	}
	// ���ӽ���! �޴� ���ý� ���Ӿ� ��ȯ
	cs_MoveCursor((dfSCREEN_WIDTH - strlen("======== = Game Start!==========")) / 2, dfSCREEN_HEIGHT / 2 + 1);
	printf("========= Game Start! ==========\n");
	cs_MoveCursor((dfSCREEN_WIDTH - strlen("====== = Press any key.... ======")) / 2, dfSCREEN_HEIGHT / 2 + 2);
	printf("======= Press any key.... ======\n");
}

void	UpdateGame()
{

	/*************************
				INPUT
	**************************/
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		player.x = max(0, player.x - 1);
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		player.x = min(dfSCREEN_WIDTH - 1, player.x + 1);
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		player.y = max(0, player.y - 1);
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		player.y = min(dfSCREEN_HEIGHT - 1, player.y + 1);
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		player_bullets.push_back({ player.x, player.y - 1 });
	}
	/*************************
				LOGIC
	**************************/

	//���� �̵���Ű��
	for (auto& bullet : player_bullets) {
		bullet.y--;
	}
	for (auto& bullet : enemy_bullets) {
		bullet.x += bullet.dx;
		bullet.y += bullet.dy;
	}
	// ȭ�� ������ ���� �Ѿ� ����
	player_bullets.erase(remove_if(player_bullets.begin(), player_bullets.end(), [](g_stPosition bullet) {
		return bullet.y < 0;
		}), player_bullets.end());

	// ���� �̵� �� ����
	for (auto& enemy : g_stages[g_stage - 1].enemies) {
		// �̵� ���� ����
		if (!enemy.move_pattern.path.empty()) {
			if (enemy.move_cooldown == 0) {
				enemy.pos.x += enemy.move_pattern.path[enemy.current_move_idx].x;
				enemy.pos.y += enemy.move_pattern.path[enemy.current_move_idx].y;
				enemy.current_move_idx = (enemy.current_move_idx + 1) % enemy.move_pattern.path.size();
				enemy.move_cooldown = 10;  // �̵� ��ٿ� �缳��
			}
			else
			{
				enemy.move_cooldown--;  // ��ٿ� ����
			}
		}

		// ���� ���� ����
		if (enemy.attack_cooldown == 0) {
			enemy.attack_pattern(enemy.pos, enemy_bullets);
			enemy.attack_cooldown = 5; // ��ٿ� ����
		}
		else {
			enemy.attack_cooldown--;
		}
	}

	// ������ �÷��̾� �Ѿ� �浹 �˻�
	for (auto bulletIt = player_bullets.begin(); bulletIt != player_bullets.end();)
	{
		bool hit = false;
		for (auto enemyIt = g_stages[g_stage - 1].enemies.begin(); enemyIt != g_stages[g_stage - 1].enemies.end(); ++enemyIt)
		{
			if (bulletIt->x == enemyIt->pos.x && bulletIt->y == enemyIt->pos.y)
			{
				enemyIt->health--;
				bulletIt = player_bullets.erase(bulletIt);
				hit = true;
				if (enemyIt->health <= 0)
				{
					g_stages[g_stage - 1].enemies.erase(enemyIt);
				}
				break;
			}
		}
		if (!hit)
		{
			++bulletIt;
		}
	}
	//�÷��̾�� ���� �Ѿ� �浹 �˻�
	for (auto bulletIt = enemy_bullets.begin(); bulletIt != enemy_bullets.end();)
	{
		if (bulletIt->x <= player.x + 2 && bulletIt->x >= player.x - 2 && bulletIt->y == player.y)
		{
			g_player_health--;
			bulletIt = enemy_bullets.erase(bulletIt);
			if (g_player_health <= 0)
			{
				g_scene = GAMEOVER;
			}
			break;
		}
		else
		{
			++bulletIt;
		}
	}


	//Ŭ���� Ȯ��
	if (g_stages[g_stage - 1].enemies.empty()) // ��� ���� �������� ��
	{
		g_stage++;
		if (g_stage - 1 == g_stages.size())
		{
			g_scene = CLEAR;
		}
		else
		{
			g_scene = LOAD;
		}
	}
	else
	{
		/*************************
				RENDERING
		**************************/
		Buffer_Clear();
		cs_MoveCursor(0, 0);
		Draw_Player();
		Draw_Enemy();
		Draw_Bullets();
		Draw_UI();
		Buffer_Flip();
	}
}

//�������� Ŭ���� ��
void	UpdateClear()
{
	Buffer_Clear();
	int idx = 0;
	const char* message = "VICTORY!!!!!";
	std::string current_display = "";

	clock_t start_time = clock();
	Buffer_Clear();
	while (idx < strlen(message))
	{
		clock_t	current_time = clock();
		if (((current_time - start_time) / CLOCKS_PER_SEC) >= idx * 0.7)
		{
			current_display += message[idx];
			cs_ClearScreen();
			cs_MoveCursor((dfSCREEN_WIDTH - strlen(message)) / 2, dfSCREEN_HEIGHT / 2);
			printf("%s\n", current_display);
			idx++;
		}
	}


	for (int key = 8; key <= 255; key++)
	{
		//���� �����ִ� Ű Ȯ�� 
		if (GetAsyncKeyState(key) & 0x8000)
		{
			g_stage = 1;
			cs_ClearScreen();
			cs_MoveCursor(0, 0);
			g_scene = CLEAR;
		}
	}
	Buffer_Flip();
}

//���� ����
void	UpdateOver()
{
	Buffer_Clear();
	int idx = 0;
	const char* message = "GAME OVER!!!";
	std::string current_display = "";

	clock_t start_time = clock();
	Buffer_Clear();
	while (idx < strlen(message))
	{
		clock_t	current_time = clock();
		if (((current_time - start_time) / CLOCKS_PER_SEC) >= idx * 0.5)
		{
			current_display += message[idx];
			cs_ClearScreen();
			cs_MoveCursor((dfSCREEN_WIDTH - strlen(message)) / 2, dfSCREEN_HEIGHT / 2);
			printf("%s\n", current_display);
			idx++;
		}
	}

	for (int key = 8; key <= 255; key++)
	{
		//���� �����ִ� Ű Ȯ�� 
		if (GetAsyncKeyState(key) & 0x8000)
		{
			g_stage = 1;
			cs_ClearScreen();
			cs_MoveCursor(0, 0);
			g_player_health = 3;
			g_scene = GAMEOVER;
		}
	}
}

// ���� ��ġ�� �ð� ����
void AttackPattern(g_stPosition& pos, std::vector<g_stPosition>& bullets) {
    if (rand() % 10 == 0) {
        bullets.push_back({pos.x, pos.y + 1, 0, 1});
    }
}

void AttackPattern2(g_stPosition& pos, std::vector<g_stPosition>& bullets) {

	float dx = (player.x - pos.x);
	float dy = (player.y - pos.y);
	float distance = std::sqrt(dx * dx + dy * dy);
	dx /= distance;
	dy /= distance;
	if (rand() % 4 == 0)
		bullets.push_back({ pos.x, pos.y + 1, (float)floor(dx + 0.5), (float)floor(dy + 0.5) });
}

std::function<void(g_stPosition&, std::vector<g_stPosition>&)> GetAttackPattern(const std::string& patternName) {
	if (patternName == "attack2")
		return AttackPattern2;
	else
		return AttackPattern;
}


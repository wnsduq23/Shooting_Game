#ifndef STAGE_H
# define STAGE_H

#include <string>
#include <vector>
#include "Console.h"
#include <functional>



struct g_stPosition
{
	int	x = dfSCREEN_WIDTH / 2;
	int	y = dfSCREEN_HEIGHT - 1;
	float	dx;
	float	dy;
};

extern int g_stage;
extern g_stPosition player;
extern std::vector<g_stPosition> player_bullets;
extern std::vector<g_stPosition> enemy_bullets;
extern std::vector<g_stPosition> enemy_bullets2;


// enemy1.txt , enemy2.txt ... -> enemy�� ���� ����
struct stEnemyType
{
	std::string name;
	std::string move_pattern_file;
	int			health;
};

// move1.txt, move2.txt ...
struct stMovePattern
{
	std::string					name;
	std::vector<g_stPosition>	path;
};

// enemy1, enemy2 ... -> enemy ��ü
struct stEnemy
{
	stEnemyType		type; //�̰� ��ǻ� enemy_name
	g_stPosition	pos;
	stMovePattern	move_pattern;
	int				current_move_idx; // ���� �̵� ��� idx
	bool			move_forward;  // �̵� ���� (true: ������, false: ������)
	std::function<void(g_stPosition&, std::vector<g_stPosition>&)> attack_pattern;
	int				health; // ���� ���� ü��
	int				move_cooldown;
	int				attack_cooldown;
};

struct stStage
{
	int enemy_cnt; //for�� ���� ���ؼ� �ʿ�
	std::vector<stEnemy> enemies;
};

extern std::vector<stEnemyType>		g_enemy_types;
extern std::vector<stMovePattern>	g_move_patterns;
extern std::vector<stStage>			g_stages;

//========================================
//			RenderingContents.cpp
//========================================

stStage						ParseStageInfo(const std::vector<std::string>& lines, int& current_line);
std::vector<std::string>	ReadLinesFromFile(const std::string& filename);
stEnemyType					ParseEnemyType(const std::vector<std::string>& lines, int& currentLine);
stMovePattern				ParseMovePattern(const std::string& line);
std::function<void(g_stPosition&, std::vector<g_stPosition>&)> GetAttackPattern(const std::string& patternName);
std::function<void(g_stPosition&, std::vector<g_stPosition>&)> GetAttackPattern2(const std::string& patternName);

#endif

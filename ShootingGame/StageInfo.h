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


// enemy1.txt , enemy2.txt ... -> enemy에 대한 정의
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

// enemy1, enemy2 ... -> enemy 객체
struct stEnemy
{
	stEnemyType		type; //이게 사실상 enemy_name
	g_stPosition	pos;
	stMovePattern	move_pattern;
	int				current_move_idx; // 현재 이동 경로 idx
	bool			move_forward;  // 이동 방향 (true: 정방향, false: 역방향)
	std::function<void(g_stPosition&, std::vector<g_stPosition>&)> attack_pattern;
	int				health; // 적의 현재 체력
	int				move_cooldown;
	int				attack_cooldown;
};

struct stStage
{
	int enemy_cnt; //for문 돌기 위해서 필요
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

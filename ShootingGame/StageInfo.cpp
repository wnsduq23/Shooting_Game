#include "StageInfo.h"
#include <fstream>
#include <sstream>

//txt 파일 읽어오기

//txt 파일에서 파싱하기 
stStage ParseStageInfo(const std::vector<std::string>& lines, int& current_line){
	stStage stage;
	stage.enemy_cnt = std::stoi(lines[current_line].substr(lines[current_line].find("enemy_cnt") + 10));
	current_line++;

	for (int i = 0; i < stage.enemy_cnt; i++)
	{
		std::stringstream ss(lines[current_line]);
		stEnemy enemy;
		std::string enemy_type_name;
		ss >> enemy_type_name >> enemy.pos.x >> enemy.pos.y;

		for (const auto& enemy_type : g_enemy_types)
		{
			if (enemy_type.name == enemy_type_name)
			{
				enemy.type = enemy_type;
				enemy.health = enemy.type.health;
				break;
			}
		}
		for (const auto& move_pattern : g_move_patterns)
		{
			if (move_pattern.name == enemy.type.move_pattern_file)
			{
				enemy.move_pattern = move_pattern;
				break;
			}
		}
		enemy.attack_pattern = GetAttackPattern("attack2");
		//enemy.attack_pattern2 = GetAttackPattern("attack2");
		enemy.current_move_idx = 0;  // Initialize move index
        enemy.move_forward = true;  // 초기 이동 방향 설정
        enemy.attack_cooldown = 0;  // Initialize attack cooldown
		enemy.move_cooldown = 10;  // 이동 쿨다운 초기화

		stage.enemies.push_back(enemy);
		current_line++;
	}
	return (stage);
}

//한줄씩 읽어오기
std::vector<std::string> ReadLinesFromFile(const std::string& filename)
{
	std::vector<std::string> lines;
	std::ifstream file(filename);
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line))
		{
			lines.push_back(line);
		}
		file.close();
	}
	else
	{
		printf("Unable to open file !!");
	}
	return (lines);
}

stEnemyType ParseEnemyType(const std::vector<std::string>& lines, int& current_line) {
    stEnemyType enemyType;
    //std::stringstream ss(lines[currentLine]);
	enemyType.name = lines[current_line].substr(2);
    //ss >> enemyType.name;
	current_line++;

	while (current_line < lines.size() && !lines[current_line].empty() && lines[current_line][0] != '#') {
        std::stringstream ss(lines[current_line]);
        std::string key;
        ss >> key;
        if (key == "move_pattern") {
            ss >> enemyType.move_pattern_file;
        } else if (key == "health") {
            ss >> enemyType.health;
        }
        current_line++;
    }
    return enemyType;
}

stMovePattern ParseMovePattern(const std::string& filename) {
    std::vector<std::string> lines = ReadLinesFromFile(filename);
    stMovePattern movePattern;
    movePattern.name = filename;

    for (const auto& line : lines) {
        std::stringstream ss(line);
        g_stPosition pos;
        ss >> pos.x >> pos.y;
        movePattern.path.push_back(pos);
    }

    return movePattern;
}


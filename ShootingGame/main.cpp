#include "Rendering.h"
#include "main.h"
#include "Console.h"
#include <Windows.h>
#include "StageInfo.h"
#include <time.h>

int g_scene = 0;
std::vector<stEnemyType> g_enemy_types;
std::vector<stMovePattern> g_move_patterns;
std::vector<stStage> g_stages;

void main(void)
{
	bool flag = 0;
	cs_Initial();

	//--------------------------------------------------------------------
	// 게임의 메인 루프
	// 이 루프가  1번 돌면 1프레임이다.
	//--------------------------------------------------------------------
	while (1)
	{
		switch (g_scene)
		{
		case TITLE:
			UpdateTitle();
			break;
		case LOAD:
		{
			if (!flag)
			{
				//게임 씬을 위한 stage 데이터 불러오기
				//enemyinfo 읽기
				std::vector<std::string> enemy_info_lines = ReadLinesFromFile("enemyinfo.txt");
				int current_line = 0;
				while (current_line < enemy_info_lines.size()) {
					if (enemy_info_lines[current_line].find("# enemy") != std::string::npos) {
						g_enemy_types.push_back(ParseEnemyType(enemy_info_lines, current_line));
					}
					else {
						current_line++;
					}
				}
				// move patterns 파싱
				for (const auto& enemyType : g_enemy_types) {
					g_move_patterns.push_back(ParseMovePattern(enemyType.move_pattern_file));
				}

				//stageinfo 읽기
				std::vector<std::string> stage_info_lines = ReadLinesFromFile("stageinfo.txt");
				int stage_count = std::stoi(stage_info_lines[0].substr(stage_info_lines[0].find('=') + 1));
				current_line = 1;
				for (int i = 0; i <= stage_count; i++)
				{
					while (current_line < stage_info_lines.size() && stage_info_lines[current_line].find("stage") == std::string::npos) {
						current_line++;
					}
					if (current_line < stage_info_lines.size()) {
						current_line++;
						g_stages.push_back(ParseStageInfo(stage_info_lines, current_line));
					}
				}
				flag = 1;
			}
			//로딩 씬 렌더링
			int idx = 0;
			const char* message = "Next Stage";
			std::string current_display = "";

			clock_t start_time = clock();
			Buffer_Clear();
			while (idx < strlen(message))
			{
				clock_t	current_time = clock();
				if (((current_time - start_time) / CLOCKS_PER_SEC) >= idx * 0.2)
				{
					current_display += message[idx];
					cs_ClearScreen();
					cs_MoveCursor((dfSCREEN_WIDTH - strlen(message)) / 2, dfSCREEN_HEIGHT / 2);
					printf("%s\n", current_display);
					idx++;
				}
			}

			g_scene = INGAME;
		}
		break;
		case INGAME:
			UpdateGame();
			break;
		case CLEAR:
			UpdateClear();
			break;
		case GAMEOVER:
			UpdateOver();
			break;
		}
		Sleep(50);
	}
}

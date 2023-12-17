#include "WinScene.h"
#include "MainMenu.h"

void WinScene_Init(void)
{
	background_ = CP_Image_Load("./Images/EndMessage.png");

	Transit_Create(&transition);
	transition.scale = 1;
	transition.isTransitingToBig = true;
	setSpeed(&transition, -1);
}
void WinScene_Update(void)
{

	if (CP_Input_KeyTriggered(KEY_SPACE))
	{
		Transit_NextScene(&transition, MainMenu_Init, MainMenu_Update, MainMenu_Exit);
	}

	//transit
	

	WinScene_Render();

	Transit_UpdateToBig(&transition, CP_System_GetDt());
}
void WinScene_Render(void)
{
	CP_Image_Draw(background_, 800, 450, 1600, 900, 255);

	CP_Settings_TextSize(40.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));

	//CP_Font_DrawText("Stage 1 Cleared!", width * 0.5f, height * 0.5f);
	//CP_Font_DrawText("Press SPACE to return to menu", width * 0.5f, height * 0.6f);

}
void WinScene_Exit(void)
{

}

#include "pch.h"
#include "Menu.h"
#include "Features.h"
#include "Fonts.h"
#include "ImGuiAddons.h"

void Menu::SetupStyle()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& Style = ImGui::GetStyle();

	ImGui::StyleColorsDark();

	Style.WindowRounding = 4.0f;
	Style.WindowTitleAlign.x = 0.5f;
	Style.ChildRounding = 4.0f;
	Style.PopupRounding = 4.0f;
	Style.FrameRounding = 4.0f;
	Style.ScrollbarRounding = 4.0f;
	Style.GrabRounding = 4.0f;
	Style.TabRounding = 4.0f;

	// Made static so it persists until end of the program
	static std::string s_IniPath(Features::GetConfigPath().append("imgui.ini").string());
	io.IniFilename = s_IniPath.c_str();

	// Load font
	ImFontConfig FontCfg{};
	FontCfg.FontDataOwnedByAtlas = false;
	strcpy_s(FontCfg.Name, "Chalet London 1960");
	io.FontDefault = io.Fonts->AddFontFromMemoryTTF((void*)Fonts::ChaletLondon1960, sizeof(Fonts::ChaletLondon1960), 20.0f, &FontCfg);
}

void Menu::Render()
{
	if (!IsOpen)
		return;

	ImGui::SetNextWindowSize(ImVec2(400, 400));
	ImGui::SetNextWindowPos(ImVec2(300, 200), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("RDR2NoRestrictions", &IsOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		ImGui::Text("Hello, world");

		if (ImGui::Button("Exit"))
			Features::OnExit();

		ImGui::SeparatorText("Toggles");
		ImGui::Checkbox("Disable Guarma invisible snipers", g_Settings["disable_guarma_snipers"].get<bool*>());
		ImGui::Checkbox("Disable West Elizabeth invisible snipers", g_Settings["disable_west_eliz_snipers"].get<bool*>());
		ImGui::Checkbox("Disable Pinkerton patrols", g_Settings["disable_pinkerton_patrols"].get<bool*>());
		ImGui::Checkbox("Disable cutscene black borders", g_Settings["disable_cutscene_borders"].get<bool*>());

		// https://www.rockstargames.com/newswire/article/9k1248838o1892/The-Red-Dead-Redemption-2-Special-Edition-Ultimate-Edition-and-Collect
		ImGui::SeparatorText("DLCs");
		ImGui::Checkbox("Enable physical pre-order content", g_Settings["enable_phys_preorder"].get<bool*>());

		ImGui::Checkbox("Enable pre-order content", g_Settings["enable_preorder"].get<bool*>());
		ImGui::SameLine();
		ImGui::HelpTooltip({
			"War Horse",
			"The Outlaw Survival Kit"
		});

		ImGui::Checkbox("Enable Special Edition content", g_Settings["enable_special_edition"].get<bool*>());
		ImGui::SameLine();
		ImGui::HelpTooltip({
			"Bank Robbery Mission and Gang Hideout",
			"Dappled Black Thoroughbred",
			"Talisman and Medallion Gameplay Bonuses",
			"Gameplay Boosts, Cash Bonuses & Discounts",
			"The Nuevo Paraiso Gunslinger Outfit",
			"Free Access to Additional Weapons"
		});

		ImGui::Checkbox("Enable Ultimate Edition content", g_Settings["enable_ultimate_edition"].get<bool*>());
		ImGui::SameLine();
		ImGui::HelpTooltip({
			"Special Edition content",
			"Bonus Outfits",
			"Black Chestnut Thoroughbred",
			"Free Access to the Survivor Camp Theme",
			"Free Access to Additional Weapons",
			"Rank Bonuses"
		});

		ImGui::Checkbox("Enable treasure map bonus", g_Settings["enable_treasure_map_bonus"].get<bool*>());
	}
	ImGui::End();
}

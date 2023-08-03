#include "pch.h"
#include "Menu.h"
#include "Features.h"
#include "Fonts.h"
#include "ImGuiAddons.h"
#include "JobQueue.h"
#include "rage/Natives.h"

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

	ImGui::SetNextWindowSize(ImVec2(500, 600));
	ImGui::SetNextWindowPos(ImVec2(300, 200), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("RDR2NoRestrictions", &IsOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
	{
		ImGui::SeparatorText("Buttons");
		if (ImGui::Button("Exit"))
			Features::OnExit();
		ImGui::SameLine();
		if (ImGui::Button("Reveal Map"))
			Features::RevealMap();

		ImGui::SeparatorText("Toggles");
		if (Features::IsEpilogueUnlocked())
			ImGui::BeginDisabled();
		if (ImGui::Checkbox("Disable Pinkerton patrols", g_Settings["disable_pinkerton_patrols"].get<bool*>()))
		{
			// Actually disable patrols when checkbox is unchecked
			if (!g_Settings["disable_pinkerton_patrols"].get<bool>())
				Features::SetDisablePinkertonPatrols(false);
		}
		ImGui::SameLine();
		ImGui::HelpTooltip({
			"Disabling patrols also disables invisible snipers in West Elizabeth",
			"This won't remove lawmen in cities"
		});
		if (Features::IsEpilogueUnlocked())
			ImGui::EndDisabled();

		ImGui::Checkbox("Disable invisible snipers", g_Settings["disable_invisible_snipers"].get<bool*>());
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

		ImGui::SeparatorText("Teleports");
		if (ImGui::BeginChild("##teleport_list"))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_SeparatorTextAlign, ImVec2(0.5, 0.5));
			ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5, 0.5));

			ImGui::SeparatorText("Ambarino");
			if (ImGui::Selectable("Colter"))
				Features::TeleportOnGround(-1360.84f, 2394.43f, 307.06f);
			if (ImGui::Selectable("Wapiti Indian Reservation"))
				Features::TeleportOnGround(475.95f, 2183.3f, 244.57f);

			ImGui::SeparatorText("New Hanover");
			if (ImGui::Selectable("Valentine"))
				Features::TeleportOnGround(-332.713f, 785.19f, 116.39f);
			if (ImGui::Selectable("Annesburg"))
				Features::TeleportOnGround(2921.04f, 1294.39f, 44.64f);
			if (ImGui::Selectable("Van Horn Trading Post"))
				Features::TeleportOnGround(2961.2f, 498.59f, 46.2f);

			ImGui::SeparatorText("Lemoyne");
			if (ImGui::Selectable("Rhodes"))
				Features::TeleportOnGround(1305.83f, -1298.29f, 76.57f);
			if (ImGui::Selectable("Saint Denis"))
				Features::TeleportOnGround(2376.26f, -1139.36f, 46.83f);

			ImGui::SeparatorText("West Elizabeth");
			if (ImGui::Selectable("Strawberry"))
				Features::TeleportOnGround(-1738.43f, -409.49f, 155.59f);
			if (ImGui::Selectable("Beecher's Hope"))
				Features::TeleportOnGround(-1649.0f, -1416.1f, 83.41f);

			ImGui::SeparatorText("New Austin");
			if (ImGui::Selectable("Tumbleweed"))
				Features::TeleportOnGround(-5415.37f, -2938.96f, 1.31f);

			ImGui::SeparatorText("Guarma");
			if (ImGui::Selectable("Guarma"))
				Features::TeleportOnGround(1424.31f, -7325.1f, 81.46f);

			ImGui::SeparatorText("Nuevo Paraiso (Mexico)");
			if (ImGui::Selectable("El Presidio"))
				Features::TeleportOnGround(-2124.65f, -3403.09f, 33.4f);

			ImGui::PopStyleVar(2);
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

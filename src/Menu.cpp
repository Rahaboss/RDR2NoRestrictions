#include "pch.h"
#include "Menu.h"
#include "Features.h"

void Menu::SetupStyle()
{
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& Style = ImGui::GetStyle();

	Style.WindowTitleAlign.x = 0.5f;
	Style.WindowRounding = Style.ChildRounding = Style.FrameRounding = Style.PopupRounding =
		Style.ScrollbarRounding = Style.GrabRounding = Style.TabRounding = 4.0f;
}

void Menu::Render()
{
	if (!IsOpen)
		return;

	if (ImGui::Begin("RDR2NoRestrictions", &IsOpen))
	{
		ImGui::Text("Hello, world");

		if (ImGui::Button("Exit"))
			Features::OnExit();
	}
}

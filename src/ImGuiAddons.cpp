#include "pch.h"
#include "ImGuiAddons.h"

void ImGui::HelpTooltip(std::vector<const char*> Messages)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort) && ImGui::BeginTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		for (const auto& m : Messages)
			ImGui::BulletText(m);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

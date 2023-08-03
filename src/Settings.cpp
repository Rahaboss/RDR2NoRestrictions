#include "pch.h"
#include "Settings.h"
#include "Features.h"

namespace Settings
{
	static json s_DefaultSettings = R"(
	{
		"disable_cutscene_borders": false,
		"disable_invisible_snipers": false,
		"disable_pinkerton_patrols": false,
		"enable_phys_preorder": false,
		"enable_preorder": false,
		"enable_special_edition": false,
		"enable_ultimate_edition": false,
		"enable_treasure_map_bonus": false
	}
	)"_json;

	void Save(const std::filesystem::path& FilePath)
	{
		std::ofstream File(FilePath);
		assert(File.good());
		File << g_Settings.dump(1, '\t') << '\n';
		File.close();
	}

	void WriteDefaultSettings(const std::filesystem::path& FilePath)
	{
		printf("Loading default settings.\n");
		g_Settings = s_DefaultSettings;
		Save(FilePath);
	}

	void Create()
	{
		std::filesystem::path Path(Features::GetConfigPath().append("Settings.json"));
		std::ifstream File(Path);

		if (!File)
		{
			WriteDefaultSettings(Path);
			return;
		}

		File >> g_Settings;
		File.close();

		// Check for missing settings
		if (g_Settings.size() != s_DefaultSettings.size())
		{
			bool UpdateFile = false;
			for (const auto& e : s_DefaultSettings.items())
			{
				if (!g_Settings.contains(e.key()))
				{
					UpdateFile = true;
					g_Settings[e.key()] = e.value();
				}
			}

			if (UpdateFile)
			{
				printf("Updating settings.\n");
				Save(Path);
			}
		}
		
		printf("Settings loaded.\n");
	}

	void Destroy()
	{
		printf("Saving settings.\n");
		std::filesystem::path Path(Features::GetConfigPath().append("Settings.json"));
		Save(Path);
	}
}

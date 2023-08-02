#include "pch.h"
#include "Settings.h"
#include "Features.h"

namespace Settings
{
	static json s_DefaultSettings{ R"(
	{
		"disable_guarma_snipers": false,
		"disable_west_eliz_snipers": false,
		"disable_pinkerton_patrols": false,
		"disable_cutscene_borders": false,
		"enable_phys_preorder": false,
		"enable_preorder": false,
		"enable_special_edition": false,
		"enable_ultimate_edition": false,
		"enable_treasure_map_bonus": false
	}
	)"_json };

	void WriteDefaultSettings(const std::filesystem::path& FilePath)
	{
		std::ofstream File(FilePath);
		assert(File.good());
		File << s_DefaultSettings.dump(4) << '\n';
		File.close();

		g_Settings.clear();
		g_Settings = s_DefaultSettings;
	}

	void Save(const std::filesystem::path& FilePath)
	{
		std::ofstream File(FilePath);
		assert(File.good());
		File << g_Settings.dump(4) << '\n';
		File.close();
	}

	void Create()
	{
		std::filesystem::path Path(Features::GetConfigPath().append("Settings.json"));
		std::ifstream File(Path);

		if (!File)
		{
			WriteDefaultSettings(Path);
			File.open(Path, std::fstream::in);
		}

		assert(File.good());
		File >> g_Settings;
		File.close();

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
	}

	void Destroy()
	{
		std::filesystem::path Path(Features::GetConfigPath().append("Settings.json"));
		Save(Path);
	}
}

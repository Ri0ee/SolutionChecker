#pragma once

#include <fstream>
#include <map>
#include <string>



namespace SG
{

	class SettingGuru
	{
	public:
		SettingGuru() {}
		virtual ~SettingGuru() {}

		struct Option {
			std::string value;
			bool valid = true;
		};

		void Push(std::string key_, const std::string& value_) {
			Option value{ value_ };
			option_map.try_emplace(key_, value);
		}

		Option Pull(std::string key_) {
			if (option_map.find(key_) == option_map.end()) return { "", false };
			return option_map[key_];
		}

		void Update(const std::string& key_, const std::string& value_) {
			Option value{ value_ };
			option_map[key_] = value;
		}

		bool Reload(const std::string& path_to_options_file_) {
			std::ifstream options_file(path_to_options_file_);
			if (!options_file.is_open()) return false;

			Clear();
			std::string line;
			while (std::getline(options_file, line)) {
				std::string option_key = line.substr(0, line.find_first_of(":"));
				std::string option_value = line.substr(line.find_first_of(":") + 1);
				option_map.insert({ option_key, { option_value } });
			}

			return true;
		}

		bool Save(const std::string& path_to_options_file_) {
			std::ofstream options_file(path_to_options_file_, std::ios::trunc);
			if (!options_file.is_open()) return false;

			for (auto option : option_map)
				options_file << option.first << ":" << option.second.value << "\n";

			return true;
		}

		void Clear() {
			option_map.clear();
		}

	private:

		std::map<std::string, Option> option_map;
	};

}


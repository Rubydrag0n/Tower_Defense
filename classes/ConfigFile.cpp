#include "ConfigFile.h"
#include <fstream>
#include <string>
#include <map>

ConfigFile* gConfig_file = nullptr;

std::string trim(std::string const& source, char const* delims = " \t\r\n") {
	auto result(source);
	auto index = result.find_last_not_of(delims);
	if (index != std::string::npos)
		result.erase(++index);

	index = result.find_first_not_of(delims);
	if (index != std::string::npos)
		result.erase(0, index);
	else
		result.erase();
	return result;
}



ConfigFile::ConfigFile(std::string const& configFile) {
	std::ifstream file(configFile.c_str());

	std::string line;
	std::string in_section;
	while (std::getline(file, line)) {

		if (!line.length()) continue;

		if (line[0] == '#') continue;
		if (line[0] == ';') continue;

		if (line[0] == '[') {
			in_section = trim(line.substr(1, line.find(']') - 1));
			continue;
		}

		int pos_equal = line.find('=');
		auto name = trim(line.substr(0, pos_equal));
		auto value = trim(line.substr(pos_equal + 1));

		content_[in_section + '/' + name] = Chameleon(value);
	}
}

Chameleon const& ConfigFile::Value(std::string const& section, std::string const& entry) const {
	const auto ci = content_.find(section + '/' + entry);

	if (ci == content_.end())
	{
		throw "does not exist";
	}
	return ci->second;
}

bool ConfigFile::value_exists(std::string const& section, std::string const& entry)
{
	const auto ci = content_.find(section + '/' + entry);
	if (ci == content_.end())
	{
		return false;
	}
	return true;
}

Chameleon ConfigFile::value_or_zero(std::string const& section, std::string const& entry)
{
	const auto ci = content_.find(section + '/' + entry);

	if (ci == content_.end())
	{
		auto cham = new Chameleon(0.0);
		return *cham;
	}
	return ci->second;
}



Chameleon const& ConfigFile::Value(std::string const& section, std::string const& entry, double value) {
	try {
		return Value(section, entry);
	}
	catch (const char *) {
		return content_.insert(std::make_pair(section + '/' + entry, Chameleon(value))).first->second;
	}
}

Chameleon const& ConfigFile::Value(std::string const& section, std::string const& entry, std::string const& value) {
	try {
		return Value(section, entry);
	}
	catch (const char *) {
		return content_.insert(std::make_pair(section + '/' + entry, Chameleon(value))).first->second;
	}
}
#include "Config.h"
#include <Windows.h>
#include <string>
#include <algorithm>
#include "tinyxml2.h"
#include "GameFunctions.h"
#include "Hack.h"

std::string path = "";

void loadPath()
{
	// Get ac_client.exe directory
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string str(buffer);
	str = str.substr(0, str.find_last_of("\\/"));

	// the XML doc path
	path = str + "/AHKHack.xml";
}

void loadConfig()
{
	// Load the path
	loadPath();

	// Check if file exists
	if (FILE *file = fopen(path.c_str(), "r"))
	{
		fclose(file);

		// Load the document
		tinyxml2::XMLDocument doc;
		doc.LoadFile(path.c_str());

		// Get the root
		tinyxml2::XMLNode* root = doc.FirstChild();

		// Check if root was found
		if(root == nullptr)
		{
			// Bad config, resave
			saveConfig();

			GameFunctions::OutputToConsole("Bad config! Loading default values.");

			return;
		}

		for (int i = 0; i < hacks.size(); i++)
		{
			Hack* hack = hacks[i];

			std::string name(hack->hackName);
			name.erase(std::remove_if(name.begin(), name.end(), std::isspace), name.end());

			// Get element node from name
			tinyxml2::XMLElement* element = root->FirstChildElement(name.c_str());

			if (element == nullptr)
			{
				// Bad config, resave
				saveConfig();

				GameFunctions::OutputToConsole("Bad config! Loading default values.");
				
				return;
			}

			// Loop through all options
			for (int j = 0; j < hack->options.size(); j++)
			{
				HackOption* hackOption = hack->options[j];

				std::string optionName(hackOption->name);
				optionName.erase(std::remove_if(optionName.begin(), optionName.end(), std::isspace), optionName.end());
			
				tinyxml2::XMLElement* optionElement = element->FirstChildElement(optionName.c_str());

				if (optionElement == nullptr)
				{
					// Bad config, resave
					saveConfig();

					GameFunctions::OutputToConsole("Bad config! Loading default values.");

					return;
				}

				tinyxml2::XMLElement* valueElement = optionElement->FirstChildElement("Value");

				if (valueElement == nullptr)
				{
					// Bad config, resave
					saveConfig();

					GameFunctions::OutputToConsole("Bad config! Loading default values44444.");

					return;
				}

				switch (hackOption->type)
				{
				case HACKBOOL:
					valueElement->QueryBoolText(&hackOption->asBool);
					break;

				case HACKINT:
					valueElement->QueryIntText(&hackOption->asInt);
					break;

				case HACKFLOAT:
					valueElement->QueryFloatText(&hackOption->asFloat);
					break;
				}

				hack->onOptionChanged(j);
			}
		}
	}
	else 
	{
		// Save the config
		saveConfig();
	}
}

void saveConfig()
{
	loadPath();

	tinyxml2::XMLDocument doc;

	tinyxml2::XMLElement* root = doc.NewElement("Root");

	for (int i = 0; i < hacks.size(); i++)
	{
		Hack* hack = hacks[i];

		std::string name(hack->hackName);
		name.erase(std::remove_if(name.begin(), name.end(), std::isspace), name.end());

		tinyxml2::XMLElement* hackRoot = doc.NewElement(name.c_str());

		for (int i = 0; i < hack->options.size(); i++)
		{
			HackOption* option = hack->options[i];

			std::string optionName(option->name);
			optionName.erase(std::remove_if(optionName.begin(), optionName.end(), std::isspace), optionName.end());

			tinyxml2::XMLElement* optionElement = doc.NewElement(optionName.c_str());

			tinyxml2::XMLElement* value = doc.NewElement("Value");

			switch (option->type)
			{
			case HACKBOOL:
				value->SetText(option->asBool);
				break;

			case HACKINT:
				value->SetText(option->asInt);
				break;

			case HACKFLOAT:
				value->SetText(option->asFloat);
				break;
			}

			optionElement->InsertEndChild(value);

			hackRoot->InsertEndChild(optionElement);
		}

		root->InsertEndChild(hackRoot);
	}

	doc.InsertFirstChild(root);

	tinyxml2::XMLError eResult = doc.SaveFile(path.c_str());
}

#include "ServerSettings.h"

#include <iostream>

namespace HttpServer
{
	ServerSettings::~ServerSettings()
	{
		this->clear();
	}

	void ServerSettings::addDataVariant(DataVariant::Abstract *dataVariant)
	{
		this->variants.emplace(dataVariant->getName(), dataVariant);
	}

	void ServerSettings::clear()
	{
		if (false == this->variants.empty() )
		{
			for (auto &variant : this->variants)
			{
				delete variant.second;
			}

			this->variants.clear();
		}

		if (false == this->apps_tree.empty() )
		{
			std::unordered_set<ServerApplicationSettings *> applications;
			this->apps_tree.collectApplicationSettings(applications);

			for (auto &app : applications)
			{
				try
				{
					if (app->application_final)
					{
						const std::string root = app->root_dir;
						app->application_final(root.data() );
					}
				}
				catch (std::exception &exc)
				{
					std::cout << "Warning: an exception was thrown when the application '" << app->server_module << "' was finishes: " << exc.what() << std::endl;
				}

				delete app;
			}

			applications.clear();
			this->apps_tree.clear();
		}

		if (false == this->global.empty() )
		{
			this->global.clear();
		}
	}
};

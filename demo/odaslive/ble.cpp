#include "ble.h"


//  Main Class Methods for the Meet Pie Device

BLE::BLE()
{
    // Maximum time to wait for any single async process to timeout during initialization
max_async_init_timeout_ms = 30 * 1000;
// The battery level ("battery/level") reported by the server (see Server.cpp)
server_data_battery_level = 100;  // for now we dont actually support real time batter;
}

const void * BLE::data_getter(const char *p_name)
{

	if (nullptr == p_name)
	{
		printf("NULL name sent to server data getter");
		return nullptr;
	}

	std::string str_name = p_name;

	if (str_name == "battery/level")
	{
		return &server_data_battery_level;
	}
	else if (str_name == "text/string")
	{

		return server_data_text_String.c_str();
	}

	printf((std::string("Unknown name for server data getter request: '") + p_name + "'").c_str());
	return nullptr;
}

int BLE::data_setter(const char *p_name, const void *p_data)
{

    if (nullptr == p_name)
    {
        printf("NULL name sent to server data setter");
        return 0;
    }
    if (nullptr == p_data)
    {
        printf("NULL p_data sent to server data setter");
        return 0;
    }

    std::string str_name = p_name;

    if (str_name == "mode")
    {
        std::string ble_mode = static_cast<const char *>(p_data);
        printf((std::string("Server data: mode set to '") + ble_mode + "'").c_str());
        return 1;
    }

    printf((std::string("Unknown name for server data setter request: '") + p_name + "'").c_str());

    return 0;
}

bool BLE::start()
{
        // Start Bluetooth
    if (!ggkStart("gobbledegook", "Gobbledegook", "Gobbledegook", this->data_getter, this->data_setter, max_async_init_timeout_ms))
	{
        return 0;}
else
	{return 1;}

}


void BLE::stop()
{
    ggkShutdownAndWait();

}

void BLE::update(MEETING meeting_obj) 
{
            // now create the JSON for BLE

            	mutex_buffer.lock();

			server_data_text_String = "{\"tMT\": ";
			server_data_text_String += std::to_string(meeting_obj.total_meeting_time);
			server_data_text_String += ",\"m\": [";

			for (int i = 1; i < MAXPART; i++)
			{
				server_data_text_String += "[";
				server_data_text_String += std::to_string(meeting_obj.participant[i].angle);
				server_data_text_String += ",";
				server_data_text_String += std::to_string(meeting_obj.participant[i].is_talking);
				server_data_text_String += ",";
				server_data_text_String += std::to_string(meeting_obj.participant[i].num_turns);
				server_data_text_String += ",";
				server_data_text_String += std::to_string(meeting_obj.participant[i].total_talk_time);
				server_data_text_String += "]";

 				// logic to calc turns 

				if (meeting_obj.participant[i].is_talking == 1 && meeting_obj.num_talking == 1) 
				{
					if(meeting_obj.last_talker!=i) // its a change of turn
					{
						++meeting_obj.participant[i].num_turns;
						meeting_obj.last_talker = i;
					}
				}

				meeting_obj.participant[i].is_talking = 0; // set everyone to not talking
                // end of new turn logic

				if (i < MAXPART-1)
				{
					server_data_text_String += ",";
				}
			}

			server_data_text_String += "]}\n";

			mutex_buffer.unlock();

		    if (debug_mode == 0x01) printf ("Bluetooth data: %s\n",server_data_text_String.c_str());

    		// now the output string is ready and_swe should call notify
			ggkNofifyUpdatedCharacteristic("/com/gobbledegook/text/string");

}

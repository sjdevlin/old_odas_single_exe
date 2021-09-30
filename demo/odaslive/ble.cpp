#include "ble.h"


//  Main Class Methods for the Meet Pie Device

BLE::BLE()
{
    // Maximum time to wait for any single async process to timeout during initialization
kMaxAsyncInitTimeoutMS = 30 * 1000;
// The battery level ("battery/level") reported by the server (see Server.cpp)
serverDataBatteryLevel = 100  // for now we dont actually support real time batter;
}

const void * BLE::dataGetter(const char *pName)
{

	if (nullptr == pName)
	{
		printf("NULL name sent to server data getter");
		return nullptr;
	}

	std::string strName = pName;

	if (strName == "battery/level")
	{
		return &serverDataBatteryLevel;
	}
	else if (strName == "text/string")
	{

		return serverDataTextString.c_str();
	}

	printf((std::string("Unknown name for server data getter request: '") + pName + "'").c_str());
	return nullptr;
}

int BLE::dataSetter(const char *pName, const void *pData)
{

    if (nullptr == pName)
    {
        printf("NULL name sent to server data setter");
        return 0;
    }
    if (nullptr == pData)
    {
        printf("NULL pData sent to server data setter");
        return 0;
    }

    std::string strName = pName;

    if (strName == "mode")
    {
        std::string blemode = static_cast<const char *>(pData);
        printf((std::string("Server data: mode set to '") + blemode + "'").c_str());
        return 1;
    }

    printf((std::string("Unknown name for server data setter request: '") + pName + "'").c_str());

    return 0;
}

int BLE::Start()
{
        // Start Bluetooth
    if (!ggkStart("gobbledegook", "Gobbledegook", "Gobbledegook", this->dataGetter, this->dataSetter, kMaxAsyncInitTimeoutMS))
        return -1;

}


void BLE::Stop()
{
    ggkShutdownAndWait();

}

void BLE::Update(MEETING Meeting_Obj) 
{
            // now create the JSON for BLE

            	mutex_buffer.lock();

			serverDataTextString = "{\"tMT\": ";
			serverDataTextString += std::to_string(Meeting_Obj.total_meeting_time);
			serverDataTextString += ",\"m\": [";

			for (int i = 1; i < MAXPART; i++)
			{
				serverDataTextString += "[";
				serverDataTextString += std::to_string(Meeting_Obj.participant[i].angle);
				serverDataTextString += ",";
				serverDataTextString += std::to_string(Meeting_Obj.participant[i].is_talking);
				serverDataTextString += ",";
				serverDataTextString += std::to_string(Meeting_Obj.participant[i].num_turns);
				serverDataTextString += ",";
				serverDataTextString += std::to_string(Meeting_Obj.participant[i].total_talk_time);
				serverDataTextString += "]";

                // logic to calc turns 

				if (Meeting_Obj.participant[i].is_talking == 1 && Meeting_Obj.num_talking == 1) 
				{
					if (Meeting_Obj.last_talker!=i) // its a change of turn
					{
						++Meeting_Obj.participant[i].num_turns;
						Meeting_Obj.last_talker = i;
					}
				}

				Meeting_Obj.participant[i].is_talking = 0; // set everyone to not talking
                // end of new turn logic

				if (i < MAXPART-1)
				{
					serverDataTextString += ",";
				}
			}

			serverDataTextString += "]}\n";

			mutex_buffer.unlock();

		    if (debug_mode == 0x01) printf ("Bluetooth data: %s\n",serverDataTextString.c_str());

    		// now the output string is ready and we should call notify
			ggkNofifyUpdatedCharacteristic("/com/gobbledegook/text/string");

}
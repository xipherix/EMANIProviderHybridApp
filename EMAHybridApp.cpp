#include "EMAHybridApp.h"
#include <iostream>
#include <stdlib.h>
#include <map>
#include <string>
#include <list>
#include <fstream>

using namespace thomsonreuters::ema;
using namespace thomsonreuters::ema::access;
using namespace thomsonreuters::ema::rdm;
using namespace std;

int runtime = 600; //second
UInt64 defaultNIPubServiceID = 8500;
EmaString defaultNIPubServiceName = "API_ADH_NI_PUB";
UInt64 defaultNISourceDirectoryHandle = 10;
EmaString defaultConsumerServiceName = "API_ELEKTRON_EPD_RSSL";
EmaString defaultSubUserName = "username";
EmaString defaultPubUserName = "pubusername";
EmaString defaultPosition = "127.0.0.1/net";
EmaString defaultAppID = "256";
UInt64 handleStart = 200000;
list<string> defaultitems;

bool bDumpOutput = false;

void displayUsage()
{
	std::cout << "EMA Consumer Non Interactive Provider Hybrid demo\n";
	std::cout << "Usage:\n";
	std::cout << "The following options are supported\n\n";
	std::cout << " -s       Followed by the subscribing service name \n";
	std::cout << " -nip     Followed by the non-interactive publishing service name\n";
	std::cout << " -t       Followed by a list of item names seperated by space. Required by consumer and non-iteractive client publish\n\n";
	std::cout << " -tf      Followed absolute file to filename contains item list\n";
	std::cout << "          Note: command line options can contains either -t or -tf. If it contains both options it will use item list from last one\n\n";
	std::cout << " Optional options\n\n";
	std::cout << " -nipuser Followed by Username for Non interactive provider, default is username\n";
	std::cout << " -u       Followed by Username, default is username\n";
	std::cout << " -pos     Followed by Position, default is 127.0.0.1/net\n";
	std::cout << " -svcid   Followed by Service ID for Non Interactive publishing service, default is 8500";
	std::cout << " -appid	Foolowed by Application ID, default is 256\n";
	std::cout << " -v       Print OMM Refresh and Update message to console output\n";
	std::cout << " -runtime Followed by Application runtime period in second, default is 600 second\n\n";
	std::cout << "For example:\n";
	std::cout << "\temahybridapp -s DIRECT_FEED -nip NIPUB -u username -t IBM.N CSCO.O\n";
	std::cout << "\tor\n";
	std::cout << "\temahybridapp -s DIRECT_FEED -nip NIPUB -u username -tf ./itemlist.txt\n";
	std::cout << "\tcreates a service called NIPUB which derives all its items from DIRECT_FEED\n\n";
	std::cout << "\temahybridapp.exe -s DIRECT_FEED -nip NIPUB -u username -t IBM.N -runtime 3600\n";
	std::cout << "\tFor running the application 3600 second and then exit\n\n";
	std::cout << "The Demo application works in pass through mode.\n";
	std::cout << "It will re-publish the data as is based on the data from item list user provided\n\n";
	std::cout << std::endl;
}

bool checkCommandLine(int argc, char* argv[])
{
	//Read parameters from command line argument and verify if it contains valid parameters 
	int requiredParameterCount = 0;
	if (argc < 2 || STRICMP(argv[1], "-?") == 0)
	{
		displayUsage();
		return false;
	}
	

	for (int i = 1; i < argc; i++)
	{
		if ((STRICMP(argv[i], "-s") != 0) && (STRICMP(argv[i], "-nip") != 0) && (STRICMP(argv[i], "-nipuser") != 0)
			&& (STRICMP(argv[i], "-u") != 0) && (STRICMP(argv[i], "-pos") != 0) && (STRICMP(argv[i], "-appid") != 0)
			&& (STRICMP(argv[i], "-t") != 0) && (STRICMP(argv[i], "-svcid") != 0) && (STRICMP(argv[i], "-v") != 0)
			&& (STRICMP(argv[i], "-runtime") != 0) && (STRICMP(argv[i], "-tf") != 0))
		{
			displayUsage();
			return false;
		}

		if (STRICMP(argv[i], "-s") == 0)
		{
			defaultConsumerServiceName = argv[++i];
			requiredParameterCount++;
		}
		else if (STRICMP(argv[i], "-nip") == 0)
		{
			defaultNIPubServiceName = argv[++i];
			requiredParameterCount++;
		}
		else if (STRICMP(argv[i], "-runtime") == 0)
			runtime = atoi(argv[++i]);
		else if (STRICMP(argv[i], "-u") == 0)
			defaultSubUserName = argv[++i];
		else if (STRICMP(argv[i], "-nipuser") == 0)
			defaultPubUserName = argv[++i];
		else if (STRICMP(argv[i], "-pos") == 0)
			defaultPosition = argv[++i];
		else if (STRICMP(argv[i], "-id") == 0)
			defaultAppID = argv[++i];
		else if (STRICMP(argv[i], "-tf") == 0)
		{
			defaultitems.clear();
			string ricName;
			ifstream ricFile(argv[++i]);
			if (ricFile.is_open())
			{
				while (getline(ricFile, ricName))
				{
					if(!ricName.empty())
						defaultitems.push_back(ricName);
				}
				ricFile.close();
				if (defaultitems.size() >0)
					requiredParameterCount++;
			}
			else
			{
				cout << "Unable to open file " << argv[i] << endl;
				return false;
			}

		}
		else if (STRICMP(argv[i], "-v") == 0)
			bDumpOutput = true;
		else if (STRICMP(argv[i], "-svcid") == 0)
			defaultNIPubServiceID = atoi(argv[++i]);
		else if (STRICMP(argv[i], "-t") == 0)
		{
			defaultitems.clear();
			i++;
			while ((i < argc))
			{
				defaultitems.push_back(argv[i]);
				if ((i+1) <argc && argv[i+1][0] != '-')
					i++;
				else
					break;
			}
			if (defaultitems.size() > 0)
				requiredParameterCount++;
		}
	}

	// Check if command line argument contains required options 
	if (defaultitems.empty() || requiredParameterCount < 3)
	{
		displayUsage();
		return false;
	}
	return true;
}


int main(int argc, char* argv[])
{
	if (!checkCommandLine(argc, argv))
		return -1;

	std::cout << "Hybrid application demo...\n\n";

	NIProviderManager providerManager(defaultPubUserName,defaultPosition, defaultAppID,bDumpOutput);
	providerManager.start();
	cout << "NIProvider server connecting to ADH..." << endl;
	NIProviderClient* providerClient1 = &providerManager.getNIProviderCLient();
	while (!providerClient1->isConnectionUp()) { sleep(500); } // Wait until connection up on provider side.
	cout << "NIProvder ready to publish data..." << endl;

	cout << "Publish Directory to ADH..." << endl;
	providerManager.getOmmNIProvider().submit(RefreshMsg().domainType(MMT_DIRECTORY).filter(SERVICE_INFO_FILTER | SERVICE_STATE_FILTER)
		.payload(Map()
		.addKeyUInt(defaultNIPubServiceID, MapEntry::AddEnum, FilterList()
		.add(SERVICE_INFO_ID, FilterEntry::SetEnum, ElementList()
		.addAscii(ENAME_NAME, defaultNIPubServiceName)
		.addArray(ENAME_CAPABILITIES, OmmArray()
		.addUInt(MMT_MARKET_PRICE)
		.addUInt(MMT_MARKET_BY_PRICE)
		.complete())
		.addArray(ENAME_DICTIONARYS_USED, OmmArray()
		.addAscii("RWFFld")
		.addAscii("RWFEnum")
		.complete())
		.complete())
		.add(SERVICE_STATE_ID, FilterEntry::SetEnum, ElementList()
		.addUInt(ENAME_SVC_STATE, SERVICE_UP)
		.complete())
		.complete())
		.complete()).complete(), defaultNISourceDirectoryHandle);

	cout << "Consumer connecting to Provider server..."<<endl;
	ConsumerManager consumerManager(defaultSubUserName, defaultPosition, defaultAppID,bDumpOutput);
	ConsumerClient* consumerClient = &consumerManager.getConsumerClient();
	consumerClient->setNIProvider(providerManager.getOmmNIProvider());
	consumerClient->setNIProviderClient(*providerClient1);
	consumerClient->setNIPubServiceName(defaultNIPubServiceName);
	consumerManager.start();
	while (!consumerClient->isConnectionUp()){ sleep(500); } // Wait until consumer side get login accepted.
	cout << "Consumer login accepted..." << std::endl;
	cout << "Start requesting data from Provider server..." << endl;

	//Add item to item list and generate handle for publishing side.
	ITEMLIST itemList;
	ITEMLIST::iterator it;
	for (std::list<string>::iterator strit = defaultitems.begin(); strit != defaultitems.end(); strit++)
	{
		ITEMLIST::iterator it;
		it = itemList.find(*strit);
		if (it == itemList.end())
		{
			string tempstr;
			ITEMHANDLES tempHandle; 
			cout << "Add " << (*strit).c_str() << " to item list" << endl;
			cout << "	Send Item Request for item " << (*strit).c_str() << endl;
			tempHandle.ConsumerHandle = consumerManager.getOmmConsumer().registerClient(ReqMsg().serviceName(defaultConsumerServiceName)
													   .name((*strit).c_str()), *consumerClient, (void*)(*strit).c_str());
			tempHandle.NiPubHandle = handleStart;
			itemList[*strit] = tempHandle;
			handleStart++;
		}
	}
	consumerClient->setItemList(itemList);
	sleep(runtime*1000);
	consumerManager.stop();
	providerManager.stop();
	return 0;
}
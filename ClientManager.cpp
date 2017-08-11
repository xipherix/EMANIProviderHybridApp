#include "ClientManager.h"

ConsumerManager::ConsumerManager(const EmaString& username, const EmaString& position, const EmaString& appID, const bool& dumpOutput) :
running(false)
{
	consumerClient1 = new ConsumerClient();
	consumerClient1->dumpOutput(dumpOutput);
	ommConsumer = new OmmConsumer(OmmConsumerConfig().username(username)
								.operationModel(OmmConsumerConfig::UserDispatchEnum)
								.applicationId(appID)
								.position(position),*consumerClient1,(void*)"ConsumerClient");
	consumerClient1->setConsumer(*ommConsumer);
}

ConsumerManager::~ConsumerManager()
{
	stop();

	if (ommConsumer)
	{
		delete ommConsumer;
		ommConsumer = 0;
	}
}
ConsumerClient& ConsumerManager::getConsumerClient()
{
	return *consumerClient1;
}
OmmConsumer & ConsumerManager::getOmmConsumer()
{
	return *ommConsumer;
}

void ConsumerManager::run()
{
	while (running)
	{
		ommConsumer->dispatch(1000);
	}
}

#ifdef WIN32
unsigned __stdcall ConsumerManager::ThreadFunc(void* pArguments)
{
	((ConsumerManager *)pArguments)->run();

	return 0;
}

#else
extern "C"
{
	void * ConsumerManager::ThreadFunc(void* pArguments)
	{
		((ConsumerManager *)pArguments)->run();
	}
}
#endif

void  ConsumerManager::start()
{
#ifdef WIN32
	_handle = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, this, 0, &_threadId);
	assert(_handle != 0);
	SetThreadPriority(_handle, THREAD_PRIORITY_NORMAL);
#else
	pthread_create(&_threadId, NULL, ThreadFunc, this);
	assert(_threadId != 0);
#endif

	running = true;
}

void  ConsumerManager::stop()
{
	if (running)
	{
		running = false;
#ifdef WIN32
		WaitForSingleObject(_handle, INFINITE);
		CloseHandle(_handle);
		_handle = 0;
		_threadId = 0;
#else
		pthread_join(_threadId, NULL);
		_threadId = 0;
#endif
	}

	running = false;
}

NIProviderManager::NIProviderManager(const EmaString& username, const EmaString& position, const EmaString& appID, const bool& dumpOutput) :
running(false)
{
	providerClient1 = new NIProviderClient();
	providerClient1->dumpOutput(dumpOutput);
	ommProvider = new OmmProvider(OmmNiProviderConfig().username(username).operationModel(OmmNiProviderConfig::UserDispatchEnum)
								.applicationId(appID)
								.position(position),*providerClient1,(void*)"NIProviderClilent");
}

NIProviderManager::~NIProviderManager()
{
	stop();

	if (ommProvider)
	{
		delete ommProvider;
		ommProvider = 0;
	}
}
NIProviderClient& NIProviderManager::getNIProviderCLient()
{
	return *providerClient1;
}
OmmProvider & NIProviderManager::getOmmNIProvider()
{
	return *ommProvider;
}

void NIProviderManager::run()
{
	while (running)
	{
		ommProvider->dispatch(1000);
	}
}

#ifdef WIN32
unsigned __stdcall NIProviderManager::ThreadFunc(void* pArguments)
{
	((NIProviderManager *)pArguments)->run();

	return 0;
}

#else
extern "C"
{
	void * NIProviderManager::ThreadFunc(void* pArguments)
	{
		((NIProviderManager *)pArguments)->run();
	}
}
#endif

void  NIProviderManager::start()
{
#ifdef WIN32
	_handle = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, this, 0, &_threadId);
	assert(_handle != 0);

	SetThreadPriority(_handle, THREAD_PRIORITY_NORMAL);
#else
	pthread_create(&_threadId, NULL, ThreadFunc, this);
	assert(_threadId != 0);
#endif

	running = true;
}

void NIProviderManager::stop()
{
	if (running)
	{
		running = false;
#ifdef WIN32
		WaitForSingleObject(_handle, INFINITE);
		CloseHandle(_handle);
		_handle = 0;
		_threadId = 0;
#else
		pthread_join(_threadId, NULL);
		_threadId = 0;
#endif
	}

	running = false;
}
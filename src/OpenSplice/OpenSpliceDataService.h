#include <AbstractDataService.h>

class OpenSpliceDataService: public AbstractDataService
{

	public:
	OpenSpliceDataService();
	OpenSpliceDataService(TopicQos reliable_topicqos);
	~OpenSpliceDataService();
	
	virtual DataWriter_ptr publish(TypeSupport *support);
	virtual DataReader_ptr subscribe(TypeSupport *support);
	virtual DataReader_ptr filteredSubscribe(TypeSupport *support, string fTopic, string fieldName, string filterContent,const StringSeq &expr);
	virtual void deleteWriter(DataWriter_ptr writer);
	virtual void deleteReader(DataReader_ptr reader);
};

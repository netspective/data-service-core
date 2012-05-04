#ifndef __OPENSPLICEDATASERVICE_H_
#define __OPENSPLICEDATASERVICE_H_

#include <DataService.h>
class OpenSpliceDataService: public DataService
{

	public:
	OpenSpliceDataService();
	OpenSpliceDataService(TopicQos reliable_topicqos);
	OpenSpliceDataService(TopicQos reliable_topicqos,string cate,string filename);
	~OpenSpliceDataService();
	
	virtual DataWriter_ptr publish(TypeSupport *support);
	virtual DataReader_ptr subscribe(TypeSupport *support);
	virtual DataReader_ptr filteredSubscribe(TypeSupport *support, string fTopic, string fieldName, string filterContent,const StringSeq &expr);
	virtual void deleteWriter(DataWriter_ptr writer);
	virtual void deleteReader(DataReader_ptr reader);
};
#endif

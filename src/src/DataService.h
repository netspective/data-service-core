#ifndef __ABSTRACTDDSSERVICE_H_
#define __ABSTRACTDDSSERVICE_H_

#include <stdio.h>
//#include <string.h>
#include <cstring>
#include <vector>
#include <LogService.h>
#include "ccpp_dds_dcps.h"

using namespace DDS;
using namespace std;



// TopicData is used to store all the objects needed to read/write a topic.

class TopicData 
{
	private:
    	Topic_var      topic;
    	//char*    typeName;
    	//char*    topicName;
	string typeName;
	string topicName;

    	Publisher_var   publisher;
    	PublisherQos    pubQos;
    	DataWriter_ptr  writer;
    	DataWriterQos   writerQos;

    	Subscriber_var  subscriber;
    	SubscriberQos   subQos;
    	DataReader_ptr  reader;
    	DataReaderQos   readerQos;

	public:
	TopicData();
    	~TopicData();
		
	void setTopic(const Topic_var _topic);
	void setTypeName(const string _typeName);
	void setTopicName(const string _topicName);

	void setPublisher_var(const Publisher_var _publisher);
	void setPublisherQos(const PublisherQos _pubQos);
	void setDataWriter_ptr(const DataWriter_ptr _writer);
	void setDataWriterQos(const DataWriterQos _writerQos);
	void setSubscriber_var(const Subscriber_var _subscriber);
	void setSubscriberQos(const SubscriberQos _subQos);
	void setDataReader_ptr(const DataReader_ptr _reader);
	void setDataReaderQos(const DataReaderQos _readerQos);


	Topic_var& getTopic();
        char* getTypeName();
        char* getTopicName();
        Publisher_var& getPublisher_var();
        PublisherQos& getPublisherQos();
        DataWriter_ptr& getDataWriter_ptr();
        DataWriterQos& getDataWriterQos();
        Subscriber_var& getSubscriber_var();
        SubscriberQos& getSubscriberQos();
        DataReader_ptr& getDataReader_ptr();
        DataReaderQos& getDataReaderQos();

	
};

// Check that the given handle is not null.If it is, print an error and exit.
void checkHandle(void* handle, const char *errormsg);

//Check that the status is ok.If not, print the given error message and exit
void checkStatus(ReturnCode_t status, const char *errormsg);

//QoS property
void getQos(TopicQos &tqos);

//Simple DDS is used to create publishers and subscribers quickly,using default settings.
class DataService  
{
	private:
    	DomainParticipantFactory_var  dpf;
	DomainParticipant_var participant;
    	vector<TopicData*> *topics;
    	TopicQos reliable_topic_qos;
    	TopicQos transient_topic_qos;
    	ContentFilteredTopic_var filteredTopic;
	

  	public:

	/*Funtions to access the member variables*/
	int setDomainParticipantFactory(const DomainParticipantFactory_var  _dpf);
	int setDomainParticipant_var(const DomainParticipant_var _participant);
	int setTopicQos(const TopicQos _reliable_topic_qos);
	int setContentFilteredTopic_var(const ContentFilteredTopic_var filteredTopic);
	int setTopicData(vector<TopicData*> &topics);
	
	DomainParticipantFactory_var& getDomainParticipantFactory();
	DomainParticipant_var& getDomainParticipant_var();
	TopicQos& getTopicQos();
	ContentFilteredTopic_var& getContentFilteredTopic_var();
	vector<TopicData*>* getTopicData();
	

	void InitQos();
	char* TypeNameToTopic(const char *typeName);

	//Logging Service 
    	LogService *logger;

    	/* Create a new DDS domain */
    	DataService(TopicQos);
    	DataService(TopicQos,string category,string filename);
    	DataService();

    	/* Delete the DDS domain */
    	~DataService();

    	/* Create a 'publisher' for a Type (like BloodPressureType) */
    	virtual DataWriter_ptr publish(TypeSupport *support)=0;

    	/* Create a 'subscriber' for a Type (like BloodPressureType) */
    	virtual DataReader_ptr subscribe(TypeSupport *support)=0;

    	/* Create a 'subscriber' for a Type (like BloodPressureType) with Content Filter*/
    	virtual DataReader_ptr filteredSubscribe(TypeSupport *support, string fTopic, string fieldName, string filterContent,const StringSeq &expr)=0;

    	/* Delete the writer used for publishing */
    	virtual void deleteWriter(DataWriter_ptr writer)=0;

    	/* Delete the reader used for subscribing */
    	virtual void deleteReader(DataReader_ptr reader)=0;
	
	/*Friend Class to making the other classes to access the private variables*/
	//friend class TopicData;

};
#endif



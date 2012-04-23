#ifndef __ABSTRACTDDSSERVICE_H_
#define __ABSTRACTDDSSERVICE_H_

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

#include "ccpp_dds_dcps.h"

using namespace DDS;
using namespace std;


// Check that the given handle is not null.If it is, print an error and exit.
void checkHandle(void* handle, const char *errormsg);

//Check that the status is ok.If not, print the given error message and exit
void checkStatus(ReturnCode_t status, const char *errormsg);


// TopicData is used to store all the objects needed to read/write a topic.

class TopicData {
  public:
    Topic_var      topic;
    const char*    typeName;
    char*          topicName;

    Publisher_var   publisher;
    PublisherQos    pubQos;
    DataWriter_ptr  writer;
    DataWriterQos   writerQos;

    Subscriber_var  subscriber;
    SubscriberQos   subQos;
    DataReader_ptr  reader;
    DataReaderQos   readerQos;

    TopicData();
    ~TopicData();
};


//QoS property
void getQos(TopicQos &tqos);

//Simple DDS is used to create publishers and subscribers quickly,using default settings.

class AbstractDataService 
{
  public:
    DomainParticipantFactory_var  dpf;
    DomainParticipant_var participant;
    vector<TopicData*> *topics;
    TopicQos reliable_topic_qos;
    TopicQos transient_topic_qos;
    ContentFilteredTopic_var filteredTopic;

    void InitQos();
    char* TypeNameToTopic(const char *typeName);

    /* Create a new DDS domain */
    AbstractDataService(TopicQos);
    AbstractDataService();

    /* Delete the DDS domain */
    ~AbstractDataService();

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

};

#endif



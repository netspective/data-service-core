#include <OpenSpliceDataService.h>
#include <sstream> 

OpenSpliceDataService::OpenSpliceDataService(): DataService()
{

}

OpenSpliceDataService::OpenSpliceDataService(TopicQos reliable_topicqos): DataService(reliable_topicqos)
{

}
OpenSpliceDataService::OpenSpliceDataService(TopicQos reliable_topicqos,string loginfo,string filename): DataService(reliable_topicqos,loginfo,filename)
{

}

OpenSpliceDataService::~OpenSpliceDataService()
{

}


DataWriter_ptr OpenSpliceDataService::publish(TypeSupport *typesupport) 
{

	ReturnCode_t status;
	checkHandle(typesupport, "publish");
    	TopicData *topic = new TopicData();
	getTopicData()->push_back(topic);
   	topic->setTypeName(typesupport->get_type_name());

    	status = typesupport->register_type(getDomainParticipant_var().in(), topic->getTypeName());
    	checkStatus(status, "register_type");
    	topic->setTopicName(TypeNameToTopic(topic->getTypeName()));
    	topic->setTopic(getDomainParticipant_var()->create_topic(
        topic->getTopicName(),
        topic->getTypeName(), 
        getTopicQos(), 
        NULL,STATUS_MASK_NONE));
  	checkHandle(topic->getTopic().in(), "create_topic");
    	status = getDomainParticipant_var()->get_default_publisher_qos(topic->getPublisherQos());
    	checkStatus(status, "get_default_publisher_qos");
    	topic->setPublisher_var(getDomainParticipant_var()->create_publisher(topic->getPublisherQos(), NULL, STATUS_MASK_NONE));
    	checkHandle(topic->getPublisher_var().in(), "create_publisher");
    	topic->setDataWriter_ptr(topic->getPublisher_var()->create_datawriter(
        topic->getTopic().in(), 
        DATAWRITER_QOS_USE_TOPIC_QOS,  
        NULL, 
        STATUS_MASK_NONE));
    	checkHandle(topic->getDataWriter_ptr(), "create_datawriter");
	
	return topic->getDataWriter_ptr();
	
}





void OpenSpliceDataService::deleteWriter(DataWriter_ptr writer) 
{
	ReturnCode_t status;
	int i;
	TopicData *topic;
	
	/* Find the TopicData for this writer */
    	for (i = 0; i < getTopicData()->size(); i++) 
	{
        	topic = getTopicData()->at(i);
        	if (topic->getDataWriter_ptr() == writer) 
		{
            		break;
        	}
    	}

    	if (topic->getDataWriter_ptr() != writer) 
	{
        	fprintf(stderr, "Error: deleteWriter: bad writer\n");
       		// exit(0);
    	}

    	/* Remove the TopicData from the topics vector */
    	getTopicData()->erase(getTopicData()->begin() + i);

    	status = topic->getPublisher_var()->delete_datawriter(writer);
    	checkStatus(status, "delete_datawriter");

    	status = getDomainParticipant_var()->delete_publisher(topic->getPublisher_var().in() );
    	checkStatus(status, "delete_publisher");

    	status = getDomainParticipant_var()->delete_topic(topic->getTopic().in() );
    	checkStatus(status, "delete_topic");
    	delete topic; 
}


DataReader_ptr OpenSpliceDataService::subscribe(TypeSupport *typesupport) 
{
	ReturnCode_t status;
    	checkHandle(typesupport, "publish");
    	TopicData *topic = new TopicData();
    	getTopicData()->push_back(topic);
    	topic->setTypeName(typesupport->get_type_name());
    	status = typesupport->register_type(getDomainParticipant_var().in(), topic->getTypeName());
    	checkStatus(status, "register_type");
    	topic->setTopicName(TypeNameToTopic(topic->getTypeName()));
    	topic->setTopic(getDomainParticipant_var()->create_topic(
        topic->getTopicName(),
        topic->getTypeName(), 
        getTopicQos(), 
        NULL,STATUS_MASK_NONE));
  	checkHandle(topic->getTopic().in(), "create_topic");
    	status = getDomainParticipant_var()->get_default_subscriber_qos(topic->getSubscriberQos());
    	checkStatus(status, "get_default_subscriber_qos");
    	topic->setSubscriber_var(getDomainParticipant_var()->create_subscriber(topic->getSubscriberQos(), NULL, STATUS_MASK_NONE));
    	checkHandle(topic->getSubscriber_var().in(), "create_subscriber");

    	topic->setDataReader_ptr(topic->getSubscriber_var()->create_datareader(
        topic->getTopic().in(), 
        DATAREADER_QOS_USE_TOPIC_QOS,  
        (DataReaderListener*) NULL, 
        STATUS_MASK_NONE));
    	checkHandle(topic->getDataReader_ptr(), "create_datareader");
    
	return topic->getDataReader_ptr();
}


DataReader_ptr OpenSpliceDataService::filteredSubscribe(TypeSupport *typesupport, string fTopic, string fieldName, string filterContent,const StringSeq &expr)
{
	ReturnCode_t status;
    	checkHandle(typesupport, "publish");
    	TopicData *topic = new TopicData();
    	getTopicData()->push_back(topic);
    	topic->setTypeName(typesupport->get_type_name());
    	status = typesupport->register_type(getDomainParticipant_var().in(), topic->getTypeName());
    	checkStatus(status, "register_type");
    	topic->setTopicName(TypeNameToTopic(topic->getTypeName()));

    	topic->setTopic(getDomainParticipant_var()->create_topic(
        topic->getTopicName(),
        topic->getTypeName(),
        getTopicQos(),
        NULL,STATUS_MASK_NONE));
    	checkHandle(topic->getTopic().in(), "create_topic");
    	status = getDomainParticipant_var()->get_default_subscriber_qos(topic->getSubscriberQos());
    	checkStatus(status, "get_default_subscriber_qos");

    	topic->setSubscriber_var(getDomainParticipant_var()->create_subscriber(topic->getSubscriberQos(), NULL, STATUS_MASK_NONE));
    	checkHandle(topic->getSubscriber_var().in(), "create_subscriber");
	
	ostringstream buf;
    	buf << fieldName <<" = '" << filterContent << "'";
    	CORBA::String_var sFilter = CORBA::string_dup(buf.str().c_str());

    	setContentFilteredTopic_var(getDomainParticipant_var()->create_contentfilteredtopic(fTopic.c_str(),topic->getTopic(), sFilter.in(), expr));
    	topic->setDataReader_ptr(topic->getSubscriber_var()->create_datareader(getContentFilteredTopic_var().in(),DATAREADER_QOS_USE_TOPIC_QOS, NULL, STATUS_MASK_NONE));

	checkHandle(topic->getDataReader_ptr(), "create_datareader");
    	return topic->getDataReader_ptr();
}


void OpenSpliceDataService::deleteReader(DataReader_ptr reader) 
{
	ReturnCode_t status;
    	int i;
    	TopicData *topic;

	/* Find the TopicData for this reader */
    	for (i = 0; i < getTopicData()->size(); i++) 
	{
        	topic = getTopicData()->at(i);
        	if (topic->getDataReader_ptr() == reader) 
		{
           		break;
        	}
    	}

    	if (topic->getDataReader_ptr() != reader) 
	{
        	fprintf(stderr, "Error: deleteReader: bad reader\n");
       		// exit(0);
    	}

    	/* Remove the TopicData from the topics vector */
    	getTopicData()->erase(getTopicData()->begin() + i);

    	status = topic->getSubscriber_var()->delete_datareader(reader);
    	checkStatus(status, "delete_datareader");

    	status = getDomainParticipant_var()->delete_subscriber(topic->getSubscriber_var().in() );
    	checkStatus(status, "delete_subscriber");

    	status = getDomainParticipant_var()->delete_topic(topic->getTopic().in() );
    	checkStatus(status, "delete_topic");

    	delete topic; 
}

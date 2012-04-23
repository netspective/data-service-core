#include <OpenSpliceDataService.h>
#include <sstream> 

OpenSpliceDataService::OpenSpliceDataService(): AbstractDataService()
{

}

OpenSpliceDataService::OpenSpliceDataService(TopicQos reliable_topicqos): AbstractDataService(reliable_topicqos)
{

}


OpenSpliceDataService::~OpenSpliceDataService()
{

}


DataWriter_ptr OpenSpliceDataService::publish(TypeSupport *typesupport) {
    ReturnCode_t status;
    checkHandle(typesupport, "publish");
    TopicData *topic = new TopicData();
    topics->push_back(topic);
    topic->typeName = typesupport->get_type_name();
    status = typesupport->register_type(participant.in(), topic->typeName);
    checkStatus(status, "register_type");
    topic->topicName = TypeNameToTopic(topic->typeName);

    topic->topic = participant->create_topic(
        topic->topicName,
        topic->typeName, 
        reliable_topic_qos, 
        NULL,                  // No TopicListener is needed
        STATUS_MASK_NONE);

    checkHandle(topic->topic.in(), "create_topic");

    status = participant->get_default_publisher_qos(topic->pubQos);
    checkStatus(status, "get_default_publisher_qos");
    topic->publisher = participant->create_publisher(topic->pubQos, NULL, STATUS_MASK_NONE);
    checkHandle(topic->publisher.in(), "create_publisher");

    topic->writer = topic->publisher->create_datawriter(
        topic->topic.in(), 
        DATAWRITER_QOS_USE_TOPIC_QOS,  
        NULL, 
        STATUS_MASK_NONE);

    checkHandle(topic->writer, "create_datawriter");
    return topic->writer;
}


void OpenSpliceDataService::deleteWriter(DataWriter_ptr writer) {
    ReturnCode_t status;
    int i;
    TopicData *topic;

    /* Find the TopicData for this writer */
    for (i = 0; i < topics->size(); i++) {
        topic = topics->at(i);
        if (topic->writer == writer) {
            break;
        }
    }

    if (topic->writer != writer) {
        fprintf(stderr, "Error: deleteWriter: bad writer\n");
       // exit(0);
    }

    /* Remove the TopicData from the topics vector */
    topics->erase(topics->begin() + i);

    status = topic->publisher->delete_datawriter(writer);
    checkStatus(status, "delete_datawriter");

    status = participant->delete_publisher(topic->publisher.in() );
    checkStatus(status, "delete_publisher");

    status = participant->delete_topic(topic->topic.in() );
    checkStatus(status, "delete_topic");

    delete topic; 
}


DataReader_ptr OpenSpliceDataService::subscribe(TypeSupport *typesupport) {
    ReturnCode_t status;
    checkHandle(typesupport, "publish");
    TopicData *topic = new TopicData();
    topics->push_back(topic);
    topic->typeName = typesupport->get_type_name();
    status = typesupport->register_type(participant.in(), topic->typeName);
    checkStatus(status, "register_type");
    topic->topicName = TypeNameToTopic(topic->typeName);

    topic->topic = participant->create_topic(
        topic->topicName,
        topic->typeName, 
        reliable_topic_qos, 
        NULL,                  // No TopicListener is needed
        STATUS_MASK_NONE);

    checkHandle(topic->topic.in(), "create_topic");

    status = participant->get_default_subscriber_qos(topic->subQos);
    checkStatus(status, "get_default_subscriber_qos");
    topic->subscriber = participant->create_subscriber(topic->subQos, NULL, STATUS_MASK_NONE);
    checkHandle(topic->subscriber.in(), "create_subscriber");

    topic->reader = topic->subscriber->create_datareader(
        topic->topic.in(), 
        DATAREADER_QOS_USE_TOPIC_QOS,  
        (DataReaderListener*) NULL, 
        STATUS_MASK_NONE);

    checkHandle(topic->reader, "create_datareader");
    return topic->reader;
}
DataReader_ptr OpenSpliceDataService::filteredSubscribe(TypeSupport *typesupport, string fTopic, string fieldName, string filterContent,const StringSeq &expr)
{
    ReturnCode_t status;
    checkHandle(typesupport, "publish");
    TopicData *topic = new TopicData();
    topics->push_back(topic);
    topic->typeName = typesupport->get_type_name();
    status = typesupport->register_type(participant.in(), topic->typeName);
    checkStatus(status, "register_type");
    topic->topicName = TypeNameToTopic(topic->typeName);

    topic->topic = participant->create_topic(
        topic->topicName,
        topic->typeName,
        reliable_topic_qos,
        NULL,                  // No TopicListener is needed
        STATUS_MASK_NONE);

    checkHandle(topic->topic.in(), "create_topic");

    status = participant->get_default_subscriber_qos(topic->subQos);
    checkStatus(status, "get_default_subscriber_qos");


    topic->subscriber = participant->create_subscriber(topic->subQos, NULL, STATUS_MASK_NONE);
    checkHandle(topic->subscriber.in(), "create_subscriber");

    ostringstream buf;
    buf << fieldName <<" = '" << filterContent << "'";
    CORBA::String_var sFilter = CORBA::string_dup(buf.str().c_str());
    filteredTopic = participant->create_contentfilteredtopic(fTopic.c_str(),topic->topic, sFilter.in(), expr);
    topic->reader = topic->subscriber->create_datareader(filteredTopic.in(),DATAREADER_QOS_USE_TOPIC_QOS, NULL, STATUS_MASK_NONE);

    checkHandle(topic->reader, "create_datareader");
    return topic->reader;
}


void OpenSpliceDataService::deleteReader(DataReader_ptr reader) {
    ReturnCode_t status;
    int i;
    TopicData *topic;

    /* Find the TopicData for this reader */
    for (i = 0; i < topics->size(); i++) {
        topic = topics->at(i);
        if (topic->reader == reader) {
            break;
        }
    }

    if (topic->reader != reader) {
        fprintf(stderr, "Error: deleteReader: bad reader\n");
       // exit(0);
    }

    /* Remove the TopicData from the topics vector */
    topics->erase(topics->begin() + i);

    status = topic->subscriber->delete_datareader(reader);
    checkStatus(status, "delete_datareader");

    status = participant->delete_subscriber(topic->subscriber.in() );
    checkStatus(status, "delete_subscriber");

    status = participant->delete_topic(topic->topic.in() );
    checkStatus(status, "delete_topic");

    delete topic; 
}

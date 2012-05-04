#include <DataService.h>

using namespace DDS;

/* Setting up data for member variables */
void TopicData::setTopic(const Topic_var _topic)
{
	topic=_topic;
}

void TopicData::setTypeName(const string _typeName)
{
	
	typeName=_typeName;
	
	
}
void TopicData::setTopicName(const string _topicName)
{

	topicName=_topicName;
}

void TopicData::setPublisher_var(const Publisher_var _publisher)
{
	publisher=_publisher;

}
void TopicData::setPublisherQos(const PublisherQos _pubQos)
{
	pubQos=_pubQos;
}
void TopicData::setDataWriter_ptr(const DataWriter_ptr _writer)
{
	writer=_writer;
}
void TopicData::setDataWriterQos(const DataWriterQos _writerQos)
{
	writerQos=_writerQos;
}
void TopicData::setSubscriber_var(const Subscriber_var _subscriber)
{
	subscriber=_subscriber;
}
void TopicData::setSubscriberQos(const SubscriberQos _subQos)
{
	subQos=_subQos;
}
void TopicData::setDataReader_ptr(const DataReader_ptr _reader)
{
	reader=_reader;
}
void TopicData::setDataReaderQos(const DataReaderQos _readerQos)
{
	readerQos=_readerQos;
}


/* Getting data from member variables */
Topic_var& TopicData::getTopic()
{
	return topic;
}
char* TopicData::getTypeName()
{
	return (char *)typeName.c_str();
}
char* TopicData::getTopicName()
{
	return (char *)topicName.c_str();
}
Publisher_var& TopicData::getPublisher_var()
{
	return publisher;

}
PublisherQos& TopicData::getPublisherQos()
{
	return pubQos;
}
DataWriter_ptr& TopicData::getDataWriter_ptr()
{
	return writer;
}
DataWriterQos& TopicData::getDataWriterQos()
{
	return writerQos;
}
Subscriber_var& TopicData::getSubscriber_var()
{
	return subscriber;
}
SubscriberQos& TopicData::getSubscriberQos()
{
	return subQos;
}
DataReader_ptr& TopicData::getDataReader_ptr()
{
	return reader;
}
DataReaderQos& TopicData::getDataReaderQos()
{
	return readerQos;
}

/*Data Service Member access functions*/
int DataService::setDomainParticipantFactory(const DomainParticipantFactory_var  _dpf)
{
	dpf=_dpf;
}
int DataService::setDomainParticipant_var(const DomainParticipant_var _participant)
{
	participant=_participant;
}
int DataService::setTopicQos(const TopicQos _reliable_topic_qos)
{
	reliable_topic_qos=_reliable_topic_qos;
}
int DataService::setContentFilteredTopic_var(const ContentFilteredTopic_var _filteredTopic)
{
	filteredTopic=_filteredTopic;
}
int DataService::setTopicData(vector<TopicData*> &_topics)
{
	*topics=_topics;

}


DomainParticipantFactory_var& DataService::getDomainParticipantFactory()
{
	return dpf;
}
DomainParticipant_var& DataService::getDomainParticipant_var()
{
	return participant;
}
TopicQos& DataService::getTopicQos()
{
	return reliable_topic_qos;
}
ContentFilteredTopic_var& DataService::getContentFilteredTopic_var()
{
	return filteredTopic;
}
vector<TopicData*>* DataService::getTopicData()
{
	return topics;
}



void checkHandle(void* handle, const char *errormsg) {
    if (handle == NULL) {
        fprintf(stderr, "Error. Invalid handle. %s\n", errormsg);
        //exit(0);
    }
}

void checkStatus(ReturnCode_t status, const char* errormsg) {
    if (status != DDS::RETCODE_OK && status != DDS::RETCODE_NO_DATA) {
        fprintf(stderr, "Error, bad status %d, %s\n", status, errormsg);
       // exit(0);
    }
}

TopicData::TopicData() {
    topic = NULL;
    publisher = NULL;
    writer = NULL;
    typeName = "";
}

TopicData::~TopicData() {
}

DataService::DataService() {

	//LogServiceFactory myFactory;
	//logger=myFactory.Create();
	logger = LogServiceFactory::Create();

    	DomainId_t domain = NULL;
    	topics = new vector<TopicData*>(10);
    	dpf = DomainParticipantFactory::get_instance();
    	checkHandle(dpf.in(), "DomainParticipantFactory::get_instance");
    	participant = dpf->create_participant(domain, PARTICIPANT_QOS_DEFAULT,NULL, STATUS_MASK_NONE);
    	checkHandle(participant.in(), "You need to run 'ospl start' to start OpenSpliceDDS");
    	InitQos();
}

DataService::DataService(TopicQos reliable_topicqos) 
{
	//LogServiceFactory myFactory;
    	//logger=myFactory.Create();
	logger = LogServiceFactory::Create();

    	DomainId_t domain = NULL;
    	topics = new vector<TopicData*>(10);
    	dpf = DomainParticipantFactory::get_instance();
    	checkHandle(dpf.in(), "DomainParticipantFactory::get_instance");
    	participant = dpf->create_participant(domain, PARTICIPANT_QOS_DEFAULT,NULL, STATUS_MASK_NONE);
    	checkHandle(participant.in(), "You need to run 'ospl start' to start OpenSpliceDDS");
    	//InitQos();
	ReturnCode_t status;
    	status = participant->get_default_topic_qos(reliable_topicqos);
	memcpy(&reliable_topic_qos,&reliable_topicqos, sizeof(reliable_topicqos));

}


DataService::DataService(TopicQos reliable_topicqos,string category,string filename) 
{

	//LogServiceFactory myFactory;
	//logger=myFactory.Create(category,filename);
	logger = LogServiceFactory::Create(category,filename);
	DomainId_t domain = NULL;
    	topics = new vector<TopicData*>(10);
    	dpf = DomainParticipantFactory::get_instance();
    	checkHandle(dpf.in(), "DomainParticipantFactory::get_instance");
    	participant = dpf->create_participant(domain, PARTICIPANT_QOS_DEFAULT,NULL, STATUS_MASK_NONE);
    	checkHandle(participant.in(), "You need to run 'ospl start' to start OpenSpliceDDS");
    	//InitQos();
        ReturnCode_t status;
    	status = participant->get_default_topic_qos(reliable_topicqos);
        memcpy(&reliable_topic_qos,&reliable_topicqos, sizeof(reliable_topicqos));

}


DataService::~DataService() {
    ReturnCode_t status;

    status = dpf->delete_participant( participant.in() );
    checkStatus(status, "delete_participant");
    delete topics;
}

void getQos(TopicQos &tQos)
{
	tQos.durability.kind=VOLATILE_DURABILITY_QOS;
        tQos.reliability.kind=BEST_EFFORT_RELIABILITY_QOS;
        tQos.history.depth=10;
        tQos.durability_service.history_kind = KEEP_LAST_HISTORY_QOS;
        tQos.durability_service.history_depth= 1024;
}

/* Create the reliable_qos and transient_qos */
void DataService::InitQos() {
    ReturnCode_t status;
    status = participant->get_default_topic_qos(reliable_topic_qos);

}

char* DataService::TypeNameToTopic(const char *typeName) {
    const char *topicName = strrchr(typeName, ':');
    if (topicName == NULL) {
        topicName = typeName;
    }
    else {
        topicName++;
    }
    return strdup(topicName);
}




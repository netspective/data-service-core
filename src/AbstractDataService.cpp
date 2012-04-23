#include <AbstractDataService.h>

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
    typeName = NULL;
}

TopicData::~TopicData() {
}

AbstractDataService::AbstractDataService() {
    DomainId_t domain = NULL;
    topics = new vector<TopicData*>(10);
    dpf = DomainParticipantFactory::get_instance();
    checkHandle(dpf.in(), "DomainParticipantFactory::get_instance");
    participant = dpf->create_participant(domain, PARTICIPANT_QOS_DEFAULT, 
                                          NULL, STATUS_MASK_NONE);
    checkHandle(participant.in(), "You need to run 'ospl start' to start OpenSpliceDDS");
    InitQos();
}


AbstractDataService::AbstractDataService(TopicQos reliable_topicqos) {
    DomainId_t domain = NULL;
    topics = new vector<TopicData*>(10);
    dpf = DomainParticipantFactory::get_instance();
    checkHandle(dpf.in(), "DomainParticipantFactory::get_instance");
    participant = dpf->create_participant(domain, PARTICIPANT_QOS_DEFAULT,
                                          NULL, STATUS_MASK_NONE);
    checkHandle(participant.in(), "You need to run 'ospl start' to start OpenSpliceDDS");
    //InitQos();

	ReturnCode_t status;
    status = participant->get_default_topic_qos(reliable_topicqos);
	memcpy(&reliable_topic_qos,&reliable_topicqos, sizeof(reliable_topicqos));

}


AbstractDataService::~AbstractDataService() {
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
void AbstractDataService::InitQos() {
    ReturnCode_t status;
    status = participant->get_default_topic_qos(reliable_topic_qos);

}

char* AbstractDataService::TypeNameToTopic(const char *typeName) {
    const char *topicName = strrchr(typeName, ':');
    if (topicName == NULL) {
        topicName = typeName;
    }
    else {
        topicName++;
    }
    return strdup(topicName);
}




# SimpleDDS Library

SimpleDDS library is intended to greatly simplify the use of DDS. As such, SimpleDDS have functions that reduces by several times the line of code that need to be written for a DDS application. 

##Compiling And Installation
Clone the SimpleDDS library from github

	git clone git@github.com:netspective/data-service-core.git
	
	cd data-service-core
	
Compiling SimpleDDS Library

	make

Installing SimpleDDS 

	make install
	
	or

	make install PREFIX=<installation_path>

## OpenSpliceDataService Simplified Functions

Following are SimpleDDS of the functions and their descriptions

## Classes 

AbstractDataService class contains common functions for the DDS libraries.

OpenSpliceDataService class contains the opensplice dds implementation overridded from the AbstractDataService

#### OpenSpliceDataService Constructors

SYNOPSIS

	OpenSpliceDataService(TopicQos);

	OpenSpliceDataService();

DESCRIPTION

There are two constructors available for the SimpleDDS. Constructor with argument TopicQos will get the QoS policy from the passed argument, constructor without argument creates the default 

#### DDS/DCPS Specialized Interfaces 
IDL Pre-processor plays a role in generating code for DDS/DCPS specialized interfaces TypeSupport, DataReader and DataWriter from application data definitions defined in IDL for all supported languages.

#### Creating Publisher 

SYNOPSIS

	DataWriter_ptr publish(TypeSupport *support);

DESCRIPTION

	The publish function creating topic, create publisher and datawriter using the typesupport argument.
 
RETURN VALUE

	It returns the creater datawriter.


#### Creating Subscriber

SYNOPSIS

	DataReader_ptr subscribe(TypeSupport *support);

DESCRIPTION

The subcriber function join the topic, create subcriber and datareader using the typesupport argument.
 
RETURN VALUE

It returns the creater datareader.

#### Creating Filtered Subscriber

SYNOPSIS

	DataReader_ptr filteredSubscribe(TypeSupport *support, string fTopic, string fieldName, string filterContent,const StringSeq &expr);


DESCRIPTION

The subcriber function join the topic, create subcriber and datareader using the typesupport argument, can create content filtering topic.
 
RETURN VALUE

It returns the creater datareader.

#### Delete Publisher

SYNOPSIS

	void deleteWriter(DataWriter_ptr writer);

DESCRIPTION

The deletewriter function delete topic, delete publisher and delete datawriter using the DataWriter argument.
 
#### Delete Subscriber

SYNOPSIS

	void deleteReader(DataReader_ptr reader);

DESCRIPTION

The deletereader function delete topic, delete subscriber and delete datareader using the DataReader argument.

#### Handle Null Checker 

SYNOPSIS

	void checkHandle(void* handle, const char *errormsg);

DESCRIPTION

Check that the given handle is not null.If it is, print an error and exit.

#### Handle Status Checker 

SYNOPSIS

	void checkStatus(ReturnCode_t status, const char *errormsg);

DESCRIPTION

Check that the status is ok.If not, print the given error message and exit.









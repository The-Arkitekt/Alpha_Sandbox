#include <iostream>

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastrtps/subscriber/SampleInfo.h>
#include <fastdds/dds/core/status/SubscriptionMatchedStatus.hpp>

#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>

template <class MsgType, class PubSubType>
class Subscriber {
private:
    eprosima::fastdds::dds::DomainParticipant* participant_;
    eprosima::fastdds::dds::Subscriber* subscriber_;
    eprosima::fastdds::dds::Topic* topic_;
    eprosima::fastdds::dds::DataReader* reader_;
    eprosima::fastdds::dds::TypeSupport type_;



    class SubListener : public eprosima::fastdds::dds::DataReaderListener {
    public:

        SubListener()
            : matched_(0)
            , samples_(0) {
        }

        ~SubListener() override {
        }

        void on_data_available(
            eprosima::fastdds::dds::DataReader* reader) override {

            eprosima::fastdds::dds::SampleInfo info;
            if (reader->take_next_sample(&msg_, &info) == ReturnCode_t::RETCODE_OK) {
                if (info.instance_state == eprosima::fastdds::dds::ALIVE_INSTANCE_STATE) {
                    samples_++;
                    
                    // add message to buffer
                    _msgBuf.push_back(msg_);
                }
            }
        }

        void on_subscription_matched(
            eprosima::fastdds::dds::DataReader* reader,
            const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override {
            
            if (info.current_count_change == 1) {
                matched_ = info.total_count;
                std::cout << "Subscriber matched." << std::endl;
            }
            else if (info.current_count_change == -1) {
                matched_ = info.total_count;
                std::cout << "Subscriber unmatched." << std::endl;
            }
            else {
                std::cout << info.current_count_change
                    << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
            }
        }

        MsgType msg_;
        std::list<MsgType> _msgBuf;
        int matched_;
        std::atomic_int samples_;
    }
    listener_;

public:
	Subscriber(const char* topic, const char* messageType, PubSubType* type)
        : _topicName(topic)
        , _messageName(messageType)
        , participant_(nullptr)
        , subscriber_(nullptr)
        , topic_(nullptr)
        , reader_(nullptr)
        , type_(type) {
	};

    ~Subscriber() {
        if (reader_ != nullptr) {
            subscriber_->delete_datareader(reader_);
        }
        if (topic_ != nullptr) {
            participant_->delete_topic(topic_);
        }
        if (subscriber_ != nullptr) {
            participant_->delete_subscriber(subscriber_);
        }
        eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }

	const char* getTopic() { return _topicName; };
	const char* getMessageType() { return _messageName; };
    int getNumMessages() { return listener_._msgBuf.size(); };

    MsgType popOldestMessage() {
        MsgType ret = listener_._msgBuf.front();
        listener_._msgBuf.pop_front();
        return ret;
    }

	bool init() {
		if (_initialized) {
			return false;
		}

        eprosima::fastdds::dds::DomainParticipantQos pqos = eprosima::fastdds::dds::PARTICIPANT_QOS_DEFAULT;
        pqos.name("Participant_sub");
        auto factory = eprosima::fastdds::dds::DomainParticipantFactory::get_instance();

        participant_ = factory->create_participant(0, pqos);

        if (participant_ == nullptr) {
            return false;
        }

        //REGISTER THE TYPE
        type_.register_type(participant_);

        //CREATE THE SUBSCRIBER
        eprosima::fastdds::dds::SubscriberQos sqos = eprosima::fastdds::dds::SUBSCRIBER_QOS_DEFAULT;

        subscriber_ = participant_->create_subscriber(sqos, nullptr);

        if (subscriber_ == nullptr) {
            return false;
        }

        //CREATE THE TOPIC
        eprosima::fastdds::dds::TopicQos tqos = eprosima::fastdds::dds::TOPIC_QOS_DEFAULT;

        topic_ = participant_->create_topic(
            _topicName,
            _messageName,
            tqos);

        if (topic_ == nullptr) {
            return false;
        }

        // CREATE THE READER
        eprosima::fastdds::dds::DataReaderQos rqos = eprosima::fastdds::dds::DATAREADER_QOS_DEFAULT;
        rqos.reliability().kind = eprosima::fastdds::dds::RELIABLE_RELIABILITY_QOS;

        reader_ = subscriber_->create_datareader(topic_, rqos, &listener_);

        if (reader_ == nullptr) {
            return false;
        }
		_initialized = true;
		return true;
	};

protected:
	bool _initialized = false;
	const char* _topicName;
	const char* _messageName;
};
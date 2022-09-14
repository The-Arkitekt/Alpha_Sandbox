#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <iostream>
#include <atomic>

#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>

#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/PublisherAttributes.h>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>

template <class MsgType, class PubSubType>
class Publisher {

private:
	eprosima::fastdds::dds::DomainParticipant* participant_;
	eprosima::fastdds::dds::Publisher* publisher_;
	eprosima::fastdds::dds::Topic* topic_;
	eprosima::fastdds::dds::DataWriter* writer_;
	eprosima::fastdds::dds::TypeSupport type_;

	class PubListener : public eprosima::fastdds::dds::DataWriterListener
	{
	public:

		PubListener()
			: matched_(0)
		{
		}

		~PubListener() override
		{
		}

		void on_publication_matched(
			eprosima::fastdds::dds::DataWriter* writer,
			const eprosima::fastdds::dds::PublicationMatchedStatus& info) override {
			if (info.current_count_change == 1) {
				matched_ = info.current_count;
				std::cout << "Publisher matched." << std::endl;
			}
			else if (info.current_count_change == -1) {
				matched_ = info.current_count;
				std::cout << "Publisher unmatched." << std::endl;
			}
			else {
				std::cout << info.current_count_change
					<< " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
			}
		}
		std::atomic_int matched_;
	}
	listener_;

public:
	Publisher(const char* topic, const char* messageType, PubSubType* type)
		: initialized_(false)
		, topicName_(topic)
		, messageName_(messageType)
		, participant_(nullptr)
		, publisher_(nullptr)
		, topic_(nullptr)
		, writer_(nullptr)
		, type_(type) {
	};

	~Publisher() {
		if (writer_ != nullptr) {
			publisher_->delete_datawriter(writer_);
		}
		if (publisher_ != nullptr) {
			participant_->delete_publisher(publisher_);
		}
		if (topic_ != nullptr) {
			participant_->delete_topic(topic_);
		}
		eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->delete_participant(participant_);
	}

	int getNumSubscribers() { return listener_.matched_; };

	bool init() {
		if (initialized_) {
			return false;
		}

		eprosima::fastdds::dds::DomainParticipantQos pqos = eprosima::fastdds::dds::PARTICIPANT_QOS_DEFAULT;
		pqos.name("Participant_pub");
		auto factory = eprosima::fastdds::dds::DomainParticipantFactory::get_instance();

		participant_ = factory->create_participant(0, pqos);

		if (participant_ == nullptr) {
			return false;
		}

		//REGISTER THE TYPE
		type_.register_type(participant_);

		//CREATE THE PUBLISHER
		eprosima::fastdds::dds::PublisherQos pubqos = eprosima::fastdds::dds::PUBLISHER_QOS_DEFAULT;

		publisher_ = participant_->create_publisher(
			pubqos,
			nullptr);

		if (publisher_ == nullptr) {
			return false;
		}

		//CREATE THE TOPIC
		eprosima::fastdds::dds::TopicQos tqos = eprosima::fastdds::dds::TOPIC_QOS_DEFAULT;

		topic_ = participant_->create_topic(
			topicName_,
			messageName_,
			tqos);

		if (topic_ == nullptr) {
			return false;
		}

		// CREATE THE WRITER
		eprosima::fastdds::dds::DataWriterQos wqos = eprosima::fastdds::dds::DATAWRITER_QOS_DEFAULT;

		writer_ = publisher_->create_datawriter(
			topic_,
			wqos,
			&listener_);

		if (writer_ == nullptr) {
			return false;
		}

		initialized_ = true;
		return true;
	}

	bool publish(MsgType msg) {
		if (!initialized_) {
			return false;
		}
		if (listener_.matched_ > 0) {
			writer_->write(&msg);
			return true;
		}
		return false;
	}

protected:
	bool initialized_;
	const char* topicName_;
	const char* messageName_;
};

#endif //PUBLISHER_H
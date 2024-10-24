#include <QCoreApplication>
#include <mqtt/async_client.h>
#include <mqtt/connect_options.h>
#include <mqtt/message.h>
#include <iostream>

const std::string SERVER_ADDRESS("tcp://broker.hivemq.com:1883");
const std::string CLIENT_ID("Qt_MQTT_Client");
const std::string TOPIC("testtopic");
const int QOS = 1;

class MqttClient {
public:
    MqttClient() : client_(SERVER_ADDRESS, CLIENT_ID) {
        mqtt::connect_options connOpts;
        connOpts.set_keep_alive_interval(20);
        connOpts.set_clean_session(true);

        client_.connect(connOpts)->wait();

        client_.subscribe(TOPIC, QOS)->wait();
    }

    void publish(const std::string& message) {
        mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, message);
        pubmsg->set_qos(QOS);
        client_.publish(pubmsg)->wait();
        std::cout << "Message published: " << message << std::endl;
    }

private:
    mqtt::async_client client_;
};

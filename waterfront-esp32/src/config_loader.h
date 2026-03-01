// MQTT config
struct MqttConfig {
    String broker;
    int port;
    String username;
    String password;
    String clientIdPrefix;
    bool useTLS;
    String caCertPath;
    String clientCertPath;
    String clientKeyPath;
};

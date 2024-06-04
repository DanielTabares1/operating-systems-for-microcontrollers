load('api_timer.js');
load('api_mqtt.js');

Timer.set(1000, Timer.REPEAT, function() {
    let ok = MQTT.pub('test/topic', 'hello from an updated firmware');
    print('mqtt message sent?', ok);
}, null);
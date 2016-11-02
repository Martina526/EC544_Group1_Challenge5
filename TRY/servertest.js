var Particle = require('particle-api-js');
var particle = new Particle();
var token;
var EventSource = require('eventsource');
var express = require('express');
var app = require('express')();

var http = require('http').Server(app);

var io = require('socket.io')(http);



particle.login({username: 'ec544group1@gmail.com', password: 'group1544'}).then(   
    function(data){       
        token = data.body.access_token;
        var deviceID = new Array();
        deviceID[0] = '330034001347353236343033';//sensor_1
        deviceID[1] = '2b0017001547353236343033';//sensor_2
        deviceID[2] = '2f003c000b47353235303037';//sensor_3
        deviceID[3] = '240028001847353236343033';//sensor_4
        
        particle.getEventStream({ deviceId: deviceID[3], name: 'output', auth: token }).then(function(stream) {
            stream.on('event', function(data1) {               
                //Move this cumulation into ino file on photon: var wheelOffset = output*5 * sin(90);
                var url = 'https://api.spark.io/v1/devices/xxx/events/controlWheel?access_token='+token;
                var es = new EventSource(url);
                //var es = new EventSource('http://demo-eventsource.rhcloud.com/') 
          });
        });
        particle.getEventStream({ deviceId: deviceID[3], name: 'towardsWall', auth: token }).then(function(stream) {
            stream.on('event', function(data2) {
                var distance = double(data2);
                if(distance < 50){
                    var url = 'https://api.spark.io/v1/devices/xxx/events/dontHit?access_token='+token;
                    var es = new EventSource(url);
                }
          });
        });
        particle.getEventStream({ deviceId: deviceID[0], name: 'speed', auth: token }).then(function(stream) {
            stream.on('event', function(data3) {
                  
                  console.log(data3.data);
                 
                io.emit('speedsensor', data3.data);
                 
               
               });   
                  
               
          });
      
    }
);





var connected = 0;

//BLE stuff:
'use strict';


//Rfduino data when sent as a float comes in as an ArrayBuffer
//use this to convert back to a number
var arrayBufferToFloat = function (ab) {
    var a = new Float32Array(ab);
    return a[0];
};

// App function OBJECT usage = app.NameOfFunction
var app = {

	//first function to be called to intialize all app components 
    initialize: function() {
        this.bindEvents();
    },

    //Setup listeners on the app
    bindEvents: function() {
        document.addEventListener('deviceready', this.onDeviceReady, false); //when app is ready
        //window.addEventListener('orientationchange', this.updateOrientation);
        refreshButton.addEventListener('touchstart', this.refreshDeviceList, false); //When the button is clicked
        deviceList.addEventListener('touchstart', this.connect, false);  //When an item in the list is clicked
    },
   
    onDeviceReady: function() {
       app.refreshDeviceList();
    },
    
    //manage the Rfduino Device list
    refreshDeviceList: function() {	
       document.getElementById('deviceList').innerHTML = 'No Rfduino Found'; // empties the list
       rfduino.discover(1, app.onDiscoverDevice, app.onError); // Try to find Rfduino 1) WHen discovered, 2) if Not
    },
    
    //When an Rfduino Device is found
    onDiscoverDevice: function(device) {
 		document.getElementById('deviceList').innerHTML = '';      
 		var listItem = document.createElement('li'),
            html = '<b>' + device.name + '</b><br/>' +
                'RSSI: ' + device.rssi + '&nbsp;|&nbsp;' +
                'Advertising: ' + device.advertising + '<br/>' +
                device.uuid;

        listItem.setAttribute('uuid', device.uuid);
        listItem.innerHTML = html;
        document.getElementById('deviceList').appendChild(listItem);
    },

    connect: function(e) {
        var uuid = e.target.getAttribute('uuid'), 
    	onConnect = function() {
            connected = 1;
            rfduino.onData(app.onData, app.onError);
            app.NextPage();
        };
    	rfduino.connect(uuid, onConnect, app.onError); // connect to Rfduino

    },

    writeData: function(data){
        rfduino.write(data);
    },

    onData: function(data) {
        var s = arrayBufferToFloat(data).toString();
        if(s == '1'){
            document.getElementById("DataComing").style.background = 'blue';
        }else{
            document.getElementById("DataComing").style.background = 'white';
        }

    },

    disconnect: function() {
        rfduino.disconnect(app.showConnectPage, app.onError); //need to change to function that will reset app 
        // location.reload();
    },

    showConnectPage: function() {
        document.getElementById('connectPage').style.display = 'block'; 
        document.getElementById('ControlPage').style.display = 'none';

    },

    NextPage: function() {
        document.getElementById('connectPage').style.display = 'none'; // hide first page 
        document.getElementById('ControlPage').style.display = 'block';


    },

    // updateOrientation: function() {
    //     var currentState = connected;
    //     navigator.app.loadUrl('file:///android_asset/www/index.html');
    //     if(currentState){
    //         app.NextPage();
    //     }else{
    //         app.connectPage();
    //     }
    // },

    onError: function(reason) {        
       notification.alert(reason); // real apps should use notification.alert
    },

};



function setup() {

	//initialize all BLE stuff
	app.initialize();

	screenWidth = window.innerWidth;
	screenHeight = window.innerHeight;

	document.getElementById("container").style.height=(screenHeight*0.7) +"px";
    app.showConnectPage();
    //app.NextPage();


}

function toggle(data) {
    // if(data == 1){
    //     document.getElementById('DataComing').style.background = 'blue'; 
    // }
    // else{
    //     document.getElementById('DataComing').style.background = 'white'; 
    // }
    var num = data.toString();
	app.writeData(num);
}

function end() {
	app.disconnect();
}






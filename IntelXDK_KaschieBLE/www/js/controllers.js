/* global app */
/* global ble */
/* global console */
/* global angular */

angular.module('myApp.controllers', [])

app.controller('AppController', function($scope, display, $ionicHistory, $ionicSideMenuDelegate, $ionicPlatform, $rootScope) {  
    $scope.display = display;
    $ionicPlatform.onHardwareBackButton(function(event){
        $ionicHistory.goBack();
    });
    $scope.toggleLeft = function(){
        $ionicSideMenuDelegate.toggleLeft();
    };
});
app.controller('IndexController', function($rootScope, $scope, display) {
    $rootScope.title = 'Home';    
    
});
app.controller('SearchController', function($scope, display){    
     $scope.$parent.title = 'Search'; 
});

app.controller('BleCtrl', function($scope, $interval, $timeout, $state, blePerpheralsService) {
    
    $scope.deviceID ="";
    var seconds = 5;
    var tempArray = [];
    $scope.scanStatus = {
        phase   :  "",
        text    :  ""
    };
    var timeReduction;
    
    $scope.isScanBtnDisabled = false;

	/*Description: Add discovered device to tempArray*/
	var onDiscoverDevice = function(device) {
	    console.log(JSON.stringify(device));
	 	//Add device to array
	 	tempArray.push(device);
	};
    
    /*Description: Set device list and make visible*/
	var setDeviceList = function(){
		$interval.cancel(timeReduction)
		$scope.scanStatus.phase = "complete";
		$scope.scanStatus.text = "Scan Complete";
		$scope.blePeripherals = tempArray;
		angular.element(document.querySelectorAll("#peripheral-list")).removeClass("hidden");
	};
    
    $scope.scan = function(serviceId) {
        seconds = 5;
		$scope.scanStatus.phase = "in-progress";
		$scope.scanStatus.text = "Scanning for devices....(5s left)";
		tempArray = [];
		angular.element(document.querySelectorAll("#peripheral-list")).addClass("hidden");
		if(serviceId !== undefined){
			if((serviceId.length !== 0) || (serviceId.replace(/\s/g, '').length)) { //Not spaces or empty
				console.log("Scan for Service UUID "+serviceId);
				blePerpheralsService.setServiceId(serviceId);
				//BLE Cordova plugin
				ble.scan([serviceId.toUpperCase()], seconds, onDiscoverDevice, blePerpheralsService.onError);
			}
		}
		else {
			console.log("Scan for all");
			//BLE Cordova plugin
			ble.scan([], seconds, onDiscoverDevice, blePerpheralsService.onError);
		}
		$scope.isScanBtnDisabled = true;

		var secondsLeft = seconds;

		//Set interval to visibly display the remaining seconds
		var timeReduction = $interval(function() {
			secondsLeft = secondsLeft - 1;
			if(secondsLeft > 0) {
				$scope.scanStatus.text = "Scanning for devices....(" + secondsLeft + "s left)";
			}
		}, 1000)

		//Set Timeout for applying device info to cards
		$timeout(setDeviceList, (seconds*1000));

	
        
        
    }
    
    /*Description: Connect to BLE peripheral*/
	$scope.connect = function(id, name) {
		blePerpheralsService.setSelectedDeviceId(id);
		blePerpheralsService.setSelectedDeviceName(name);
		var onConnect = function() {
			$scope.navigateTo("connectionble");
		}
		ble.connect(blePerpheralsService.getSelectedDeviceId(), onConnect, blePerpheralsService.onError);
	}

	/*Description: Transition to desired state/View*/
	$scope.navigateTo = function(stateName) {
		$state.go(stateName);
	}

})

angular.module('myApp.controllers', ['ionic'])

app.controller('ConnectionCtrl', function($scope, $state, $ionicPopup, $ionicModal, blePerpheralsService, $timeout) {
	
	/*Ionic Modal Events and functions*/
	$ionicModal.fromTemplateUrl('templates/communication-req-modal.html', {
		scope: $scope,
		animation: 'slide-in-up'
	}).then(function(modal) {
		$scope.modal = modal;
	});

	$scope.openModal = function() {
		$scope.modal.show();
	};

	$scope.$on('modal.shown', function() {
  		console.log('Modal is shown!');
  		$scope.serviceUuid = blePerpheralsService.getServiceId();
	});

	$scope.closeModal = function(serviceUuid, characteristicUuid) {
		blePerpheralsService.setServiceId(serviceUuid);
		blePerpheralsService.setCharacteristicId(characteristicUuid);
		$scope.modal.hide();
	};

	//Cleanup the modal when we're done with it!
	$scope.$on('$destroy', function() {
		$scope.modal.remove();
	});

	$scope.$on('$ionicView.enter', function(){ //This is fired twice in a row
        console.log("Connection view entered.");
		$scope.deviceName = blePerpheralsService.getSelectedDeviceName();
        $scope.openModal();
    });

	$scope.$on('$ionicView.beforeLeave', function(){ //This is fired twice in a row
        console.log("Before Connection view left.");
        $scope.disconnect();
    });    
    
    
        
        
        
    
	/*
    Description: Convert String to ArrayBuffer 
	*/
	var stringToBytes = function(string) {
	   var array = new Uint8Array(string.length);
	   for (var i = 0, l = string.length; i < l; i++) {
	       array[i] = string.charCodeAt(i);
	    }
	    return array.buffer;
	}

	/*
	    Description: Convert ArrayBuffer to String
	*/
	var bytesToString = function(buffer) {
	    return String.fromCharCode.apply(null, new Uint8Array(buffer));
	}

	/*onRead callback*/
	var onRead = function(data) {
		console.log("data read");
        var str = bytesToString(data);
        console.log(str);
        //Add str to element on connection state/view
        $scope.$apply(function(){ //Access scope from outside function
        	$scope.dataMessage = str;
        });
	}

	/*Description: Read Data from a BLE connected peripheral*/
	$scope.readData = function() {
        //read(device_id, service_uuid, characteristic_uuid, success_function, failure_function)
        ble.read(blePerpheralsService.getSelectedDeviceId(), blePerpheralsService.getServiceId(), blePerpheralsService.getCharacteristicId(), onRead, blePerpheralsService.onError);
    }

    /*onWrite callback*/
	var onWrite = function() {
	    console.log("data written to BLE peripheral");
	}

    /*Description: Write data to BLE peripheral*/
   $scope.writeData = function() {
        console.log("writeData");
        var str = "1";
        console.log(stringToBytes(str));
        ble.write(blePerpheralsService.getSelectedDeviceId(), blePerpheralsService.getServiceId(), blePerpheralsService.getCharacteristicId(), stringToBytes(str), onWrite, blePerpheralsService.onError);
    }
   
   $scope.writeDatanull = function() {
        console.log("writeData");
        var str = "0";
        console.log(stringToBytes(str));
        ble.write(blePerpheralsService.getSelectedDeviceId(), blePerpheralsService.getServiceId(), blePerpheralsService.getCharacteristicId(), stringToBytes(str), onWrite, blePerpheralsService.onError);
    }
    $scope.writeDataRECHTS = function() {
        console.log("writeData");
        var str = "1";
        console.log(stringToBytes(str));
        ble.write(blePerpheralsService.getSelectedDeviceId(), blePerpheralsService.getServiceId(), blePerpheralsService.getCharacteristicId(), stringToBytes(str), onWrite, blePerpheralsService.onError);
    }
    
    $scope.writeDataLINKS = function() {
        console.log("writeData");
        var str = "0";
        console.log(stringToBytes(str));
        ble.write(blePerpheralsService.getSelectedDeviceId(), blePerpheralsService.getServiceId(), blePerpheralsService.getCharacteristicId(), stringToBytes(str), onWrite, blePerpheralsService.onError);
    
    }
    
          $scope.writeDataSTOPP = function() {
        console.log("writeData");
        var str = "2";
        console.log(stringToBytes(str));
        ble.write(blePerpheralsService.getSelectedDeviceId(), blePerpheralsService.getServiceId(), blePerpheralsService.getCharacteristicId(), stringToBytes(str), onWrite, blePerpheralsService.onError);
    }
    
        $scope.writeData50P = function() {
        console.log("writeData");
        var str = "3";
        console.log(stringToBytes(str));
        ble.write(blePerpheralsService.getSelectedDeviceId(), blePerpheralsService.getServiceId(), blePerpheralsService.getCharacteristicId(), stringToBytes(str), onWrite, blePerpheralsService.onError);
    }
    
        $scope.writeData100P = function() {
        console.log("writeData");
        var str = "4";
        console.log(stringToBytes(str));
        ble.write(blePerpheralsService.getSelectedDeviceId(), blePerpheralsService.getServiceId(), blePerpheralsService.getCharacteristicId(), stringToBytes(str), onWrite, blePerpheralsService.onError);
    }
    
        $scope.writeDataREF = function() {
        console.log("writeData");
        var str = "5";
        console.log(stringToBytes(str));
        ble.write(blePerpheralsService.getSelectedDeviceId(), blePerpheralsService.getServiceId(), blePerpheralsService.getCharacteristicId(), stringToBytes(str), onWrite, blePerpheralsService.onError);
    }
      
    
        
           $scope.showPopup = function() {
     var alertPopup = $ionicPopup.alert({
       title: 'Don\'t eat that!',
       template: 'It might taste good'
     });
     alertPopup.then(function(res) {
       console.log('Thank you for not eating my delicious ice cream cone');
     });
           };
        
        
        
	/*Description: Transition to home View and reset peripheral list*/
	var backToHome = function () {
		console.log("Connection disconnected");
		$scope.navigateTo("scanble");
		$scope.blePeripherals = [];
	};

	/*Description: Disconnect from the BLE peripheral*/
	$scope.disconnect = function() {
	    ble.disconnect(blePerpheralsService.getSelectedDeviceId(), backToHome, blePerpheralsService.onError);
	};

	$scope.navigateTo = function(stateName) {
		$state.go(stateName);
	}

});

app.module('mySuperApp', ['ionic'])
.controller('PopupCtrl',function($scope, $ionicPopup, $timeout) {

// Triggered on a button click, or some other target
$scope.showPopup = function() {
  $scope.data = {};

  // An elaborate, custom popup
  var myPopup = $ionicPopup.show({
    template: '<input type="password" ng-model="data.wifi">',
    title: 'Enter Wi-Fi Password',
    subTitle: 'Please use normal things',
    scope: $scope,
    buttons: [
      { text: 'Cancel' },
      {
        text: '<b>Save</b>',
        type: 'button-positive',
        onTap: function(e) {
          if (!$scope.data.wifi) {
            //don't allow the user to close unless he enters wifi password
            e.preventDefault();
          } else {
            return $scope.data.wifi;
          }
        }
      }
    ]
  });

  myPopup.then(function(res) {
    console.log('Tapped!', res);
  });

  $timeout(function() {
     myPopup.close(); //close the popup after 3 seconds for some reason
  }, 3000);
 };

 // A confirm dialog
 $scope.showConfirm = function() {
   var confirmPopup = $ionicPopup.confirm({
     title: 'Consume Ice Cream',
     template: 'Are you sure you want to eat this ice cream?'
   });

   confirmPopup.then(function(res) {
     if(res) {
       console.log('You are sure');
     } else {
       console.log('You are not sure');
     }
   });
 };

 // An alert dialog
 $scope.showAlert = function() {
   var alertPopup = $ionicPopup.alert({
     title: 'Don\'t eat that!',
     template: 'It might taste good'
   });

   alertPopup.then(function(res) {
     console.log('Thank you for not eating my delicious ice cream cone');
   });
 };
});


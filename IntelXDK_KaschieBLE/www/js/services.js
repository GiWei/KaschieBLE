

app.factory('display', ['$filter','$q','$ionicPopup','$ionicSideMenuDelegate','$ionicHistory', function($filter, $q, $ionicPopup, $ionicSideMenuDelegate, $ionicHistory) {
    return {
        alert : function(message, title, button, callback){
            if(!callback) var callback = function(){return;}
            if(!title) title = 'Attention';
            if(!button) button = 'Close';
            var alertPopup = $ionicPopup.alert({
                 title: title,
                 template: message,
                buttons: [
                  {
                    text: '<b>'+button+'</b>',
                    type: 'button-positive',
                  }    
                ]

               });
               alertPopup.then(callback);
        },
        goToPath : function(pathLink){
            var path = "index.html#" + pathLink;
            window.location = path;
        },
        getPageTemplate : function(pageId){
          var deffered = $q.defer();
          $("#template").load("views.html #" + pageId, function(){                    
              var template = $("#" + pageId).html();
              $(this).html('')
              deffered.resolve(template);
          });
          return deffered.promise;
        },
        menuIsOpen : function(){
            return ($ionicSideMenuDelegate.isOpenLeft()) ? true : false;
        },
        currentStateName : function(){
            return $ionicHistory.currentStateName();
        }
    }
        
}]);

app.factory('helper', ['$filter', function($filter) {
    return {
        toDateTime : function(secs)
            {
            var t = new Date(1970,0,1);
            t.setSeconds(secs);
            return t;
            }
        }
}]);

angular.module('myApp.services', [])

app.factory('blePerpheralsService', function() {

	var selectedDeviceId = "";
	var serviceId = "";
	var characteristicId = "";
	var selectedDeviceId = "";

	return {
		getSelectedDeviceId : function() {
			return selectedDeviceId;
		},
		setSelectedDeviceId : function(id) {
			selectedDeviceId = id;
		},
		getSelectedDeviceName : function() {
			return selectedDeviceName;
		},
		setSelectedDeviceName : function(name) {
			selectedDeviceName = name;
		},
		getServiceId : function() {
			return serviceId;
		},
		setServiceId : function(id) {
			serviceId = id;
		},
		getCharacteristicId : function() {
			return characteristicId;
		},
		setCharacteristicId : function(id) {
			characteristicId = id;
		},
		onError: function(reason) {
        	alert("ERROR: " + reason);
    	}
	}
});
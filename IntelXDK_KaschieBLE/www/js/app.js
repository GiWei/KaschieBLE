/*
 * Please see the included README.md file for license terms and conditions.
 */


// This file is a suggested starting place for your code.
// It is completely optional and not required.
// Note the reference that includes it in the index.html file.


/*jslint browser:true, devel:true, white:true, vars:true */
/*global $:false, intel:false app:false, dev:false, cordova:false */



// This file contains your event handlers, the center of your application.
// NOTE: see app.initEvents() in init-app.js for event handler initialization code.

// function myEventHandler() {
//     "use strict" ;
// // ...event handler code here...
// }


// ...additional event handlers here...

//angular.module('myApp', ['ionic', 'myApp.controllers', 'myApp.services'])

app.run(function($ionicPlatform) {
  $ionicPlatform.ready(function() {
    // Hide the accessory bar by default (remove this to show the accessory bar above the keyboard
    // for form inputs)
    if(window.cordova && window.cordova.plugins.Keyboard) {
      cordova.plugins.Keyboard.hideKeyboardAccessoryBar(true);
      cordova.plugins.Keyboard.disableScroll(true);
    }
    if(window.StatusBar) {
      StatusBar.styleLightContent();
    }

    if(!window.ble) {
      if(navigator.notification) {
        navigator.notification.alert(
          'The Bluetooth Low Energy (BLE) Central Plugin [com.megster.cordova.ble] is missing.',  // message
          alertDismissed,         // callback
          'Plugin Not Found',            // title
          'Ok'                  // buttonName
        );
      }
    }
  });
})
//angular.module('myApp', ['ionic', 'myApp.controllers', 'myApp.services'])
app.config(function($stateProvider, $urlRouterProvider) {

  // Ionic uses AngularUI Router which uses the concept of states
  // Learn more here: https://github.com/angular-ui/ui-router
  // Set up the various states which the app can be in.
  // Each state's controller can be found in controllers.js
  $stateProvider

  .state('home', {
    url: '/',
    templateUrl: 'templates/home.html',
    controller: 'HomeCtrl'
  })

  .state('connection', {
    url: '/connection',
    templateUrl: 'templates/connection.html',
    //parent: "home",
    controller: 'ConnectionCtrl'
  });

  $urlRouterProvider.otherwise('/');
})

app.run(['$state', function ($state) { //set as default state
   $state.transitionTo('home');
}]);
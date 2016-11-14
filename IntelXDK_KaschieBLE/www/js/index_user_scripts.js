/*jshint browser:true */
/*global $ */(function()
{
 "use strict";
 /*
   hook up event handlers 
 */
 function register_event_handlers()
 {
    
    
     /* button  #Home */
    $(document).on("click", "#Home", function(evt)
    {
        /* your code goes here */ 
         return false;
        alert("Go Back")
    });
    
        /* button  #Home */
    $(document).on("click", "#Home", function(evt)
    {
        /* your code goes here */ 
         return false;
    });
    
    }
 document.addEventListener("app.Ready", register_event_handlers, false);
})();

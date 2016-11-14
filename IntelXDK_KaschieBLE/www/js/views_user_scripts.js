/*jshint browser:true */
/*global $ */(function()
{
 "use strict";
 /*
   hook up event handlers 
 */
 function register_event_handlers()
 {
    
    
     /* button  #Alert */
    $(document).on("click", "#Alert", function(evt)
    {
      alert("Willkommen im Expert-Modus \n\n\nSie wollen Ihren Aufnahmen noch mehr Individualität verleihen?? Dann sind Sie hier genau richtig!! \n\nIm Experten Modus können Sie ihren Verfahrweg, sowie die Geschwindigkeit in dieser der Weg zurückgelegt werden soll, beliebig eintellen. \n\nUm diesen Modus verwenden zu können, müssen Sie zu beginn ersteinmal eine Referenzfahrt durchführen. Bei dieser Fahrt wird der Schlitten zu einem der beiden Endschalter bewegt um seine genaue Possition zu ermitteln. \n\nAnschließend kann die Anfangs- und Endposition in cm, sowie die gewünschte Geschwindigkeit in Prozent eingegeben und an den Genuino übertragen werden. \nEine genaue Skala zur Orientierung für die Positionen ist an der Schiene angebracht. \n\nViel Spaß bei Ihren Aufnahmen :)")
         return false;
    });
    
        /* button  Simple */
    $(document).on("click", "#Simple", function(evt)
    {
        /* your code goes here */ 
        alert("Simple")
         return false;
    });
    
    }
 document.addEventListener("app.Ready", register_event_handlers, false);
})();

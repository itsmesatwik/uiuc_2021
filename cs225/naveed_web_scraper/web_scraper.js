    function load() {
        /* Boilerplate code */
        document.getElementById("extensionFrame").style.display = "block";
        document.getElementById("extensionFrame").style.color = "black";
        function write(text) {
            document.getElementById("extensionFrame").innerHTML += "<span>"+text+"</span>";
        }
        function writeln(text) {
            document.getElementById("extensionFrame").innerHTML += "<span>"+text+"</span>\n";
        }
        function writeElement(elmnt) {
            document.getElementById("extensionFrame").innerHTML += "<center>\n"+elmnt+"\n</center>\n";
        }
        /* End boilerplate code */
        writeElement("<b>Look at this web scraper</b>");

        //Begin writing extension here.
    }
    function ask_api() {
        var lat = parseFloat(document.getElementById("lat").value);
        var long = parseFloat(document.getElementById("long").value);
        var time = document.getElementById("date").value
        var receiver = call_api_generic("https://data.police.uk/api/crimes-street/all-crime", {"poly":(lat+0.4)+','+(long+0.4)+':'+(lat-0.4)+','+(long+0.4)+':'+(lat-0.4)+','+(long-0.4)+':'+(lat+0.4)+','+(long-0.4),"date":time});
        if(receiver != "400" && receiver != "401" && receiver != "402" && receiver != "403" && receiver != "404" && receiver != "500" && receiver != "503"){
            document.getElementById('police_output').innerHTML = receiver;
        } else {
            document.getElementById('police_output').innerHTML = "Error occurred while processing API request: "+receiver;
        }
    }
    load();
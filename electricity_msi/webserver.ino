void handleRoot() {
    digitalWrite ( led, 1 );
    String out = "<html><head><title>Electricity - Aurora</title></head>";
    out += "<body style='background-color:gray'><center>";
    out += "<span style='display:block; width:100%; font-size:2em; font-family:Verdana; text-align:center'>Electricity - Aurora</span><br/>";
    out += "<a href='brighten'><span style='display:block; background-color:white; width:100%; height:3em;'>Brighter</span></a><br/>";
    out += "<a href='dim'><span style='display:block; background-color:black; foreground-color:white; width:100%; height:3em;'>Dimmer</span></a><br/>";
    out += "<a href='white'><span style='display:block; background-color:white; width:100%; height:3em;'></span></a><br/>";
    out += "<a href='green'><span style='display:block; background-color:green; width:100%; height:3em;'></span></a><br/>";
    out += "<a href='yellow'><span style='display:block; background-color:yellow; width:100%; height:3em;'></span></a><br/>";
    out += "<a href='orange'><span style='display:block; background-color:orange; width:100%; height:3em;'></span></a><br/>";
    out += "<a href='red'><span style='display:block; background-color:red; width:100%; height:3em;'></span></a><br/>";
    out += "<a href='pink'><span style='display:block; background-color:pink; width:100%; height:3em;'></span></a><br/>";
    out += "<a href='blue'><span style='display:block; background-color:blue; width:100%; height:3em;'></span></a><br/>";
    out += "<a href='black'><span style='display:block; background-color:black; width:100%; height:3em;'></span></a><br/>";
    out += "<a href='aiml'><span style='display:block; background-color:white; width:100%; height:3em;'>Aurora Gradient</span></a><br/>";
    out += "<a href='sinelon'><span style='display:block; background-color:white; width:100%; height:3em;'>Transition: sinelon</span></a><br/>";
    out += "<a href='bpm'><span style='display:block; background-color:white; width:100%; height:3em;'>Transition: bpm</span></a><br/>";
    out += "<a href='random'><span style='display:block; background-color:white; width:100%; height:3em;'>Transition: random</span></a><br/>";
    out += "<a href='gradient'><span style='display:block; background-color:white; width:100%; height:3em;'>Transition: gradient</span></a><br/>";
    out += "</center></body>";
    out += "</html>";
    webServer.send ( 200, "text/html", out );
    digitalWrite ( led, 0 );
}

void handleNotFound() {
    digitalWrite ( led, 1 );
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += webServer.uri();
    message += "\nMethod: ";
    message += ( webServer.method() == HTTP_GET ) ? "GET" : "POST";
    message += "\nArguments: ";
    message += webServer.args();
    message += "\n";
    for ( uint8_t i = 0; i < webServer.args(); i++ ) {
        message += " " + webServer.argName ( i ) + ": " + webServer.arg ( i ) + "\n";
    }
    webServer.send ( 404, "text/plain", message );
    digitalWrite ( led, 0 );
}

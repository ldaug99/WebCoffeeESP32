HTTP_POST = "POST";
HTTP_GET = "GET";

function updateHTML() {
	// Coffee maker state
	if (document.getElementById("coffeeMakerButton").value == "coffeeState=0") {
		document.getElementById("coffeeMakerButton").innerHTML = "Start coffee maker";
		document.getElementById("coffeeMakerButton").value = "coffeeState=1"
		document.getElementById("coffeeStateTxt").innerHTML = "Off"
	} else {
		document.getElementById("coffeeMakerButton").innerHTML = "Stop coffee maker";
		document.getElementById("coffeeMakerButton").value = "coffeeState=0"
		document.getElementById("coffeeStateTxt").innerHTML = "On"
	}

	// Coffee heater state
	if (document.getElementById("coffeeHeaterButton").value == "coffeeHeater=0") {
		document.getElementById("coffeeHeaterButton").innerHTML = "Start coffee maker";
		document.getElementById("coffeeHeaterButton").value = "coffeeHeater=1"
		document.getElementById("coffeeHeaterTxt").innerHTML = "Off"
	} else {
		document.getElementById("coffeeHeaterButton").innerHTML = "Stop coffee maker";
		document.getElementById("coffeeHeaterButton").value = "coffeeHeater=0"
		document.getElementById("coffeeHeaterTxt").innerHTML = "On"
	}
}

function apiReplyListener() {
	setTimeout(() => { location.reload(); }, 500);
}

function apiRequest(type, parameter) {
	var requestUrl = "/api/" + parameter;
	var xmlHTTP = new XMLHttpRequest();
	xmlHTTP.open(type, requestUrl, true);
	xmlHTTP.onload = apiReplyListener();
	xmlHTTP.send(null);
}
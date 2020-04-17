HTTP_POST = "POST";
HTTP_GET = "GET";

function updateHTML() {
	if (document.getElementById("coffeeButton").value == "coffeeState=0") {
		document.getElementById("coffeeButton").innerHTML = "Start coffee maker";
		document.getElementById("coffeeButton").value = "coffeeState=1"
		document.getElementById("coffeeState").innerHTML = "Off"
	} else {
		document.getElementById("coffeeButton").innerHTML = "Stop coffee maker";
		document.getElementById("coffeeButton").value = "coffeeState=0"
		document.getElementById("coffeeState").innerHTML = "On"
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

function goToIndex() {
	window.location.href = window.location.hostname + "/";
}

function goToSettings() {
	window.location.href = window.location.hostname + "/settings";
}
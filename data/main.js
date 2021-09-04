var connection = new WebSocket('ws://' + "192.168.1.135" + '/ws', ['arduino']);

connection.onopen = function () {
	connection.send('Received from Client');
	console.log('Connected');
};

connection.onerror = function (error) {
  console.log('WebSocket Error', error);
};

connection.onmessage = function (e) {
    console.log('Received from server: ', e.data);
    processReceived(e.data);
};

connection.onclose = function () {
  console.log('WebSocket connection closed');
};

function processReceived(data)
{
	json = JSON.parse(data)
	if(json.command == 'updateGPIO')
	{
		updateGPIO(json.id, json.status);
	}
	if(json.Time)
	{
		var TEXTO = document.getElementById('Millis')
		TEXTO.innerHTML = json.Time
		var counterDiv = document.getElementById('counterDiv');

	}
	if(json.JoyStick)
	{
		var TEXTOj = document.getElementById('JoyStick')
		TEXTOj.innerHTML = json.JoyStick
		var counterDiv = document.getElementById('counterDiv');

	}
	
}
function sendGPIO(id, status)
{
	let data = {
		command : "setGPIO",
		id: id,
		status: status
	}

  let json = JSON.stringify(data);
  connection.send(json);
}

function sendPWM(id, pwm)
{
	updateSliderText(id, pwm);

	let data = {
		command : "setPWM",
		id: id,
		pwm: pwm
	}

  let json = JSON.stringify(data);
  connection.send(json);
}

function sendAction(id)
{
	let data = {
		command : "doAction",
		id: id,
	}

  let json = JSON.stringify(data);
  connection.send(json);
}

function updateGPIO(id, status)
{
    document.getElementById('input-label-' + id).textContent = status;

	if(status == 'ON')
	{
        document.getElementById('input-label-' + id).classList.add('On-style');
		document.getElementById('input-label-' + id).classList.remove('Off-style');
	}
	else
    {
		document.getElementById('input-label-' + id).classList.add('Off-style');
		document.getElementById('input-label-' + id).classList.remove('On-style');
	}
}
function updateSliderText(id, value) {
	document.getElementById('slider-pwm-' + id).value = value; 
	document.getElementById('slider-text-pwm-'+ id).value = value; 
}
  var counterDiv = document.getElementById('Millis');

function updateCounterUI(counter)
{
	counterDiv.innerHTML = counter; 
}


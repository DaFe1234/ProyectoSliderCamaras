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
function updateSliderText(id, value) {
	document.getElementById('slider-pwm-1' + id).value = value; 
	document.getElementById('slider-text-pwm-1'+ id).value = value; 
}
function sendPWM(id, pwm)
{
	updateSliderText(id, pwm);
console.log(pwm)
	let data = {
		command:"setPWM",
		id:id,
		pwm:pwm
	}

  let json = JSON.stringify(data);
  connection.send(json);
}
function updateSliderText(id, value) {
	document.getElementById('slider-pwm-' + id).value = value; 
	document.getElementById('slider-text-pwm-'+ id).value = value; 
}
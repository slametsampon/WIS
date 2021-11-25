var config = {
    "prevNode": 11,
    "nextNode": 13,
    "idNode": 12,
    "mode": 1,
    "cyclic": 0,
    "onDelay": 300,
    "onDuration": 45
    }

function validateForm() {

    //on delay, 1440 = 60 * 24
    var data = document.forms["configForm"]["onDelay"];
    if (data.value == "") data.value = data.placeholder;
    let onDelay = parseInt(data.value);
    if (onDelay > 1440) {
        alert("On Delay, max 1440 (24 hr)");
        return false;
    }

    //on duration, 180 = 60 * 3
    var data = document.forms["configForm"]["onDuration"];
    if (data.value == "") data.value = data.placeholder;
    var onDuration = parseInt(data.value);
    if (onDuration > 180) {
        alert("On Delay, max 180 (3 hr)");
        return false;
    }
    
    //confirmation submit
    return confirm('Are you, sure you want to submit this parameters ?');
}

function fillUpParameter(data) {

    if (!SIMULATION) config = JSON.parse(data);
    console.log(config);

    let element = document.getElementById("id_node");
    element.innerHTML = config.idNode.toString();
    if (config.idNode <= 9) element.innerHTML = "0" + config.idNode.toString();

    element = document.getElementById("id_prevNode");
    element.placeholder = config.prevNode.toString();
    if(config.prevNode <= 9) element.placeholder = "0" + config.prevNode.toString();
    element.value = element.placeholder;
    
    element = document.getElementById("id_nextNode");
    element.placeholder = config.nextNode.toString();
    if (config.nextNode <= 9) element.placeholder = "0" + config.nextNode.toString();
    element.value = element.placeholder;
    
    element = document.getElementById("id_idNode");
    element.placeholder = config.idNode.toString();
    if (config.idNode <= 9) element.placeholder = "0" + config.idNode.toString();
    element.value = element.placeholder;
    
    element = document.getElementById("id_onDelay");
    element.placeholder = config.onDelay.toString();
    element.value = element.placeholder;

    element = document.getElementById("id_onDuration");
    element.placeholder = config.onDuration.toString();
    element.value = element.placeholder;

}

function setupWis(){
  if (SIMULATION) {
    userAccess(activeUser);
    fillUpParameter("just for simulation");
  }
    else {
        fetch('/getConfig')
            .then(response => {
                const contentType = response.headers.get('content-type');
                if (!contentType || !contentType.includes('application/json')) {
                    throw new TypeError("Oops, we haven't got JSON!");
                }
                return response.json();
            })
            .then(data => {
                fillUpParameter(data);
            })
            .catch(error => console.error(error));
    }
}

document.addEventListener('DOMContentLoaded', setupWis, false);

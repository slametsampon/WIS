var statusData = {
    "idNode": 12,
    "mode": 1,
    "status": 0,
    "onDelay": 121,
    "onDuration": 46
}
function fillUpStatus(data){
    let element;
    if (!SIMULATION) statusData = JSON.parse(data);

    //Status
    element = document.getElementById("id_node");
    element.innerHTML = statusData.idNode;

    if (statusData.status == 1) {
        element = document.getElementById("id_status");
        element.innerHTML ="Wait";
    }
    else if (statusData.status == 2) {
        element = document.getElementById("id_status");
        element.innerHTML = "Active";
    }
    else {
        element = document.getElementById("id_status");
        element.innerHTML = "Idle";
    }

    //operation mode - Auto, Manual-one shoot, manual-cyclic, manual-continuous
    if (statusData.mode == 1) {
        element = document.getElementById("id_mode");
        element.innerHTML = "Manual-One";
    }
    else if (statusData.mode == 2) {
        element = document.getElementById("id_mode");
        element.innerHTML = "Manual-Cyc";
    }
    else if (statusData.status == 3) {
        element = document.getElementById("id_mode");
        element.innerHTML = "Manual-Con";
    }
    else {
        element = document.getElementById("id_mode");
        element.innerHTML = "Auto";
    }    

    let onDelay = statusData.onDelay;
    document.getElementById('id_remaining_onDelay').innerHTML = onDelay;

    let onDuration = statusData.onDuration;
    document.getElementById('id_remaining_onDuration').innerHTML = onDuration;
}

function updateStatus() {
    var d = new Date();
    var t = d.toLocaleTimeString();
    document.getElementById('id_time').innerHTML = t;
    if (SIMULATION) fillUpStatus("Just for simulation");
    else {
        fetch('/getStatus')
            .then(response => {
                const contentType = response.headers.get('content-type');
                if (!contentType || !contentType.includes('application/json')) {
                    throw new TypeError("Oops, we haven't got JSON!");
                }
                return response.json();
            })
            .then(data => {
                fillUpStatus(data);
            })
            .catch(error => console.error(error));            
    }
}

function setupWis(){
    let ti = setInterval(updateStatus, 6000);
    updateStatus();
  if (SIMULATION) {
    userAccess(activeUser);
    fillUpStatus("just for simulation");
  }
}

document.addEventListener('DOMContentLoaded', setupWis, false);

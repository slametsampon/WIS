const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
    <title>WIS - Wireless Irrigation</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        * {
        box-sizing: border-box;
        }

        html {
            font-family: Arial;
            display: inline-block;
            text-align: Left;
        }

        h2 {
            font-size: 1.25rem;
        }

        h3 {
            font-size: 1.0rem;
        }

        p {
            font-size: 1.0rem;
        }

        body {
            max-width: 500px;
            margin: 0px auto;
            padding-bottom: 25px;
        }

        input[type=text], select, textarea {
            width: 100%;
            padding: 8px;
            border: 1px solid #ccc;
            border-radius: 4px;
            resize: vertical;
            width: 25%;
        }

        label {
            padding: 12px 12px 12px 0;
            display: inline-block;
            width: 22%;
        }

        input[type=submit] {
            background-color: #4CAF50;
            color: white;
            padding: 12px 20px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            float: left;
            width: 25%;
        }

        input[type=submit]:hover {
            background-color: #45a049;
        }

        .container {
        border-radius: 5px;
        background-color: #f2f2f2;
        padding: 20px;
        }

        /* Clear floats after the columns */
        .row:after {
        content: "";
        display: table;
        clear: both;
        }
    </style>
</head>

<body>
    <div class = "container">
        <h2>WIS - Wireless Irrigation System - #<span id='id_node'>60</span></h2>
        <form name="configForm" action="/config" method="POST" onsubmit="return validateForm()">
            <hr style="height:2px;border-width:0;color:gray;background-color:gray">
        
            <label for="id_idNode">ID Node</label>
            <input type="text" id="id_idNode" name="idNode" placeholder="9"><br>

            <h3>Select Mode :</h3>
            <input type="radio" id="id_modeA" name="mode" value="modeA" checked onclick="getSensorCfg()">
            <label for="id_modeA">Auto</label><br>
        
            <input type="radio" id="id_modeM" name="mode" value="modeM" onclick="getSensorCfg()">
            <label for="id_modeH">Manual</label><br>
        
            <input type="checkbox" id="id_cyclic" name="cyclic" value="cyclic">
            <label for="id_cyclic">Cyclic</label><br>
        
            <h3>Enter Time (Minute) :</h3>
            <label for="id_onDelay">On Delay</label>
            <input type="text" id="id_onDelay" name="onDelay" placeholder="120"><br><br>
        
            <label for="id_onDuration">On Duration</label>
            <input type="text" id="id_onDuration" name="onDuration" placeholder="15"><br><br>
        
            <input type="submit" class="button" value="Submit"><br><br><br>
            <hr style="height:2px;border-width:0;color:gray;background-color:gray">
        </form>
        <p>Mode :
            <span id='id_mode'>Auto/Manual-one/Manual-cyc/Manual-con</span>
        </p>
        <p>Status : 
            <span id='id_status'>Idle/Wait/Active</span>
        </p>
        <h3>Remaining (Minute) :</h3>
        <p>
            On Delay : 
            <span id='id_remaining_onDelay'>60</span>
        </p>
        <p>On Duration : 
            <span id='id_remaining_onDuration'>15</span>
        </p>
    </div>

    <script>
        var config = {
            "idNode": 9,
            "mode": 1,
            "cyclic": 0,
            "onDelay": 1440,
            "onDuration": 180
            }

        var statusData = {
            "mode": 1,
            "status": 0,
            "onDelay": 121,
            "onDuration": 46
        }
        const SIMULATION =  true;

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

            let element = document.getElementById("id_node");
            element.innerHTML = config.idNode.toString();
            if(config.idNode <= 9) element.innerHTML = "0" + config.idNode.toString();
            
            element = document.getElementById("id_onDelay");
            element.placeholder = config.onDelay.toString();
            element.value = element.placeholder;

            element = document.getElementById("id_onDuration");
            element.placeholder = config.onDuration.toString();
            element.value = element.placeholder;

            //Operation Mode (Stop = 0, Manual = 1, Auto = 2)
            if (config.mode == 1) {
                element = document.getElementById("id_modeM");
                element.checked = true;
            }
            else if (config.mode == 2) {
                element = document.getElementById("id_modeA");
                element.checked = true;
            }
            else {
                element = document.getElementById("id_modeM").checked=false;
                element = document.getElementById("id_modeA").checked = false;
            }
        }

        function fillUpStatus(data){
            let element;
            if (!SIMULATION) statusData = JSON.parse(data);

            //Status
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
            document.getElementById('id_remaining_onDelay').innerHTML = t;
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
            if (SIMULATION) fillUpParameter("just for simulation");
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
                        fillUpStatus(data);
                    })
                    .catch(error => console.error(error));
            }
            updateStatus();
        }

        document.addEventListener('DOMContentLoaded', setupWis, false);

    </script>
</body>
</html>
)rawliteral";

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


        .container {
        border-radius: 5px;
        background-color: #f2f2f2;
        padding: 20px;
        }

    </style>
</head>

<body>
    <div class = "container">
        <h2>WIS - Wireless Irrigation System - #<span id='id_node'>60</span></h2>
        <h3>Time :
            <span id='id_time'>Time</span>
        </h3>

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
        var statusData = {
            "idNode": 12,
            "mode": 1,
            "status": 0,
            "onDelay": 121,
            "onDuration": 46
        }
        const SIMULATION =  true;

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
        }

        document.addEventListener('DOMContentLoaded', setupWis, false);

    </script>
</body>
</html>
)rawliteral";

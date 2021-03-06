const char config_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
    <title>WIS - Configuration</title>
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
        <h2>WIS - Configuration - #<span id='id_node'>60</span></h2>
        <form name="configForm" action="/config" method="POST" onsubmit="return validateForm()">
            <hr style="height:2px;border-width:0;color:gray;background-color:gray">
        
            <label for="id_prevNode">Prev. Node</label>
            <input type="text" id="id_prevNode" name="prevNode" placeholder="8"><br>

            <label for="id_nextNode">Next Node</label>
            <input type="text" id="id_nextNode" name="prevNode" placeholder="10"><br><br>

            <label for="id_idNode">ID Node</label>
            <input type="text" id="id_idNode" name="idNode" placeholder="9"><br>

            <label for="id_mode">Mode :</label>
            <select name="mode" id="id_mode">
                <option value="manual">Manual</option>
                <option value="auto">Auto</option>
                <option value="repeater">Repeater</option>
            </select>
        
            <input type="checkbox" id="id_cyclic" name="cyclic" value="cyclic">
            <label for="id_cyclic">Cyclic</label><br>
        
            <h3>Enter Time (Minute) :</h3>
            <label for="id_onDelay">On Delay</label>
            <input type="text" id="id_onDelay" name="onDelay" placeholder="120"><br>
        
            <label for="id_onDuration">On Duration</label>
            <input type="text" id="id_onDuration" name="onDuration" placeholder="15"><br><br>
        
            <input type="submit" class="button" value="Submit"><br><br>
        </form>
    </div>

    <script>
        var config = {
            "prevNode": 11,
            "nextNode": 13,
            "idNode": 12,
            "mode": 1,
            "cyclic": 0,
            "onDelay": 300,
            "onDuration": 45
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
                        fillUpParameter(data);
                    })
                    .catch(error => console.error(error));
            }
        }

        document.addEventListener('DOMContentLoaded', setupWis, false);

    </script>
</body>
</html>
)rawliteral";

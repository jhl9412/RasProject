<?php
 $conn = mysqli_connect("localhost","root","kcci");
 mysqli_set_charset($conn,"utf8");
 mysqli_select_db($conn,"openWeather");
 $result = mysqli_query($conn, "select date, time, SeoulTemp, SeoulHumi, IndoorTemp, IndoorHumi from SeoulData");
 $data = array(array('openWeather','SeoulTemp','SeoulHumi','IndoorTemp','IndoorHumi'));
 if($result){
     while($row = mysqli_fetch_array($result)){
         array_push($data, array($row['date']."\n".$row[1], intval($row[2]),intval($row[3]),intval($row[4]),intval($row[5])));
     }
 }
 $options = array(
     'title' => 'Temperature(C),Humidity(%)',
     'width' => auto, 'height' => auto
 );
 ?>

<html>
<head>

 <meta http-equiv="refresh" content = "20">
 <h1 align = 'center'>Seoul / Indoor</h1>
</head>
 <script src="//www.google.com/jsapi"></script>
 <script>
     let data = <?= json_encode($data) ?>;
     let options = <?= json_encode($options) ?>;
     google.load('visualization', '1.0', {'packages':['corechart']});
     google.setOnLoadCallback(function() {
         let chart = new google.visualization.LineChart(document.querySelector('#chart_div'));
         chart.draw(google.visualization.arrayToDataTable(data), options);
     });
 </script>
 <div id="chart_div"></div>
</html>

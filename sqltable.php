<!DOCTYPE html>
<html>
<head>
	<meta charset  = "UTF-8">
	<meta http-equiv="refresh" content = "5">
	<style type = "text/css">
		.spec{
			text-align:center;
		}
		.con{
			text-align:left;
		}
	</style>
</head>
<body>
	<h1 align = "center">Seoul Weather</h1>
	<div class = "spec">
		# <b>Seoul / MyRoom</b><br>
		# 1 ~ 99 humi <br>
		# 1 ~ 99 temp
	</div>

	<table border = '1' style = "width = 30%" align = "center">
	<tr align = "center">
		<th>TIME</th>
		<th>SeoulT</th>
		<th>SeoulH</th>
		<th>RoomT</th>
		<th>RoomH</th>
	</tr>
	<?php
		$conn = mysqli_connect("localhost","root","kcci");
		mysqli_select_db($conn,"openWeather");
		$result = mysqli_query($conn, "select * from SeoulData");

		while($row = mysqli_fetch_array($result)){
			echo "<tr align = center>";
			echo '<td>'.$row['time'].'</td>';
			echo '<td>'.$row['SeoulTemp'].'</td>';
			echo '<td>'.$row['SeoulHumi'].'</td>';
			echo '<td>'.$row['IndoorTemp'].'</td>';
			echo '<td>'.$row['IndoorHumi'].'</td>';
			echo "</tr>";
			mysqli_close($conn);
		}
	?>
	</table>
</body>
</html>

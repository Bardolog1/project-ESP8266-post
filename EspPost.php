<?php

include'conexion.php';

if ($con) {
    echo "Conexion con base de datos exitosa! ";
    
    if(isset($_POST['temperatura'])) {
        $temperatura = $_POST['temperatura'];
        echo "Estación meteorológica";
        echo " Temperaura : ".$temperatura;
    }
 
    if(isset($_POST['humedad'])) { 
        $humedad = $_POST['humedad'];
        echo " humedad : ".$humedad;
        date_default_timezone_set('america/bogota');
        $fecha_actual = date("Y-m-d H:i:s");
        
        $consulta = "INSERT INTO Tb_DHT11(Temperatura, Humedad, fecha_actual) VALUES ('$temperatura','$humedad', '$fecha_actual')";
       // $consulta = "UPDATE DHT11 SET Temperatura='$temperatura',Humedad='$humedad' WHERE Id = 1";
        $resultado = mysqli_query($con, $consulta);
        if ($resultado){
            echo " Registo en base de datos OK! ";
        } else {
            echo " Falla! Registro BD";
        }
    }
    
    
} else {
    echo "Falla! conexion con Base de datos ";   
}


?>
void COMMUNICATION(){
  rpm_sensor.clearFields();

  rpm_sensor.addField("rpm_value", rpm);
  
  Serial.print("Writing: ");
  Serial.println(client.pointToLineProtocol(rpm_sensor));
  
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Wifi connection lost");
  }
  
  if (!client.writePoint(rpm_sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

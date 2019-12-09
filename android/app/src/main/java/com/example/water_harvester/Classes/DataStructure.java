package com.example.water_harvester.Classes;

public class DataStructure {
    private int ambientTemp = 0;
    private int insideTemp = 0;
    private int insideHum = 0;
    private int fanRPM = 0;

    public void setAmbientTemp(int value){
        ambientTemp = value;
    }

    public int getAmbientTemp(){
        return ambientTemp;
    }

    public void setInsideTemp(int value){
        insideTemp = value;
    }

    public int getInsideTemp(){
        return insideTemp;
    }

    public void setInsideHum(int value){
        insideHum = value;
    }

    public int getInsideHum(){
        return insideHum;
    }

    public void setFanRPM(int value){
        fanRPM = value;
    }

    public int getFanRPM(){
        return fanRPM;
    }
}
